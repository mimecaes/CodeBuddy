#include "CodeGenerator.h"
#include "KeywordLoader.h"
#include <regex>
#include <iostream>
#include <vector>

static string normalizePatternEscapes(const string& p_in) {
    string s = p_in;
    size_t pos = 0;
    while ((pos = s.find("\\s+", pos)) != string::npos) { s.replace(pos, 3, "[[:space:]]+"); pos += 11; }
    pos = 0;
    while ((pos = s.find("\\s*", pos)) != string::npos) { s.replace(pos, 3, "[[:space:]]*"); pos += 11; }
    pos = 0;
    while ((pos = s.find("\\s", pos)) != string::npos)    { s.replace(pos, 2, "[[:space:]]"); pos += 11; }
    pos = 0;
    while ((pos = s.find("\\d+", pos)) != string::npos) { s.replace(pos, 3, "[0-9]+"); pos += 6; }
    pos = 0;
    while ((pos = s.find("\\d", pos)) != string::npos)  { s.replace(pos, 2, "[0-9]"); pos += 5; }
    pos = 0;
    while ((pos = s.find("\\w+", pos)) != string::npos) { s.replace(pos, 3, "[A-Za-z0-9_]+"); pos += 13; }
    pos = 0;
    while ((pos = s.find("\\w", pos)) != string::npos)  { s.replace(pos, 2, "[A-Za-z0-9_]"); pos += 13; }
    return s;
}

static vector<Rule> loadRules(const nlohmann::json& keywords) {
    vector<Rule> rules;
    if (!keywords.contains("rules")) return rules;
    for (auto& el : keywords["rules"].items()) {
        const string ruleName = el.key();
        const string patternRaw = el.value()["pattern"].get<string>();
        const string tmpl = el.value()["template"].get<string>();
        const string pattern = normalizePatternEscapes(patternRaw);
        try {
            regex rx(pattern, regex_constants::ECMAScript | regex_constants::optimize);
            rules.push_back(Rule{ rx, tmpl, ruleName });
        } 
        catch (const regex_error& ex) {
            cerr << "Warning: invalid regex for rule '" << ruleName << "': " << ex.what()
                      << " pattern(raw): " << patternRaw << " pattern(norm): " << pattern << "\n";
        }
    }
    return rules;
}

static string normalize(const string& input) {
    string out;
    out.reserve(input.size());
    for (unsigned char uc : input) {
        if (isalnum(uc) || isspace(uc)) out.push_back(tolower(uc));
    }
    string::iterator new_end = unique(out.begin(), out.end(), [](char a, char b) {
        return isspace(a) && isspace(b);
    });
    out.erase(new_end, out.end());
    size_t start = out.find_first_not_of(' ');
    size_t end = out.find_last_not_of(' ');
    if (start == string::npos) return "";
    return out.substr(start, end - start + 1);
}

QString CodeGenerator::generateCppCode(const QStringList& instructions, const nlohmann::json& keywords) {
    QString code;
    if (keywords.contains("templates") && keywords["templates"].contains("main_begin")) {
        code += QString::fromStdString(keywords["templates"]["main_begin"].get<string>());
    } 
    else code += "#include <iostream>\nint main(){\n";

    vector<Rule> rules = loadRules(keywords);

    for (const QString& qline : instructions) {
        string original = qline.toStdString();
        string line = normalize(original);
        if (line.empty()) {
            code += "\n";
            continue;
        }

        int bestGroups = -1;
        string bestTemplate;
        string bestRuleName;
        smatch bestMatch;

        for (const Rule& r : rules) {
            smatch match;
            if (regex_search(line, match, r.pattern)) {
                int matchedGroups = 0;
                for (size_t g = 1; g < match.size(); ++g) if (match[g].matched) ++matchedGroups;
                if (matchedGroups > bestGroups) {
                    bestGroups = matchedGroups;
                    bestTemplate = r.templateStr;
                    bestRuleName = r.name;
                    bestMatch = match;
                }
            }
        }

        if (bestGroups >= 0) {
            string temp = bestTemplate;
            vector<string> matchValues;
            for (size_t i = 0; i < bestMatch.size(); ++i) {
                string val = bestMatch[i].str();
                if (keywords.contains("tipo_cpp") && keywords["tipo_cpp"].contains(val)) {
                    val = keywords["tipo_cpp"][val].get<string>();
                }
                matchValues.push_back(val);
            }
            for (size_t i = 1; i < matchValues.size(); ++i) {
                string key = "${" + to_string(i) + "}";
                size_t pos = temp.find(key);
                while (pos != string::npos) {
                    temp.replace(pos, key.length(), matchValues[i]);
                    pos = temp.find(key, pos + matchValues[i].length());
                }
            }
            temp = regex_replace(temp, regex(R"(\$\{\d+\})"), "");

            if (temp.find("cout") == string::npos) temp.erase(remove(temp.begin(), temp.end(), '\n'), temp.end());
            code += "// [rule:" + bestRuleName + "]\n";
            code += QString::fromStdString(temp);
            code += "\n";
        } else code += "// " + QString::fromStdString(original) + "\n";
    }

    if (keywords.contains("templates") && keywords["templates"].contains("main_end")) {
        code += QString::fromStdString(keywords["templates"]["main_end"].get<string>());
    } 
    else code += "return 0;}";

    return code;
}