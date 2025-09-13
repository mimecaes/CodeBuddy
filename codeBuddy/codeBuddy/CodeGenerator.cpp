#include "CodeGenerator.h"
#include "KeywordLoader.h"
#include <regex>
#include <iostream>
#include <vector>

struct PreStep {
    regex rx;
    string replace;
    string name;
};

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

static vector<PreStep> loadPreprocess(const nlohmann::json& keywords) {
    vector<PreStep> steps;
    if (!keywords.contains("preprocess") || !keywords["preprocess"].is_array()) return steps;
    for (auto& el : keywords["preprocess"]) {
        if (!el.contains("pattern") || !el.contains("replace")) continue;
        const string name = el.contains("name") ? el["name"].get<string>() : string{};
        const string patternRaw = el["pattern"].get<string>();
        const string repl = el["replace"].get<string>();
        const string pattern = normalizePatternEscapes(patternRaw);
        try {
            regex rx(pattern, regex_constants::ECMAScript | regex_constants::optimize);
            steps.push_back(PreStep{ rx, repl, name });
        } 
        catch (const regex_error& ex) {
            cerr << "Warning: invalid preprocess regex '" << name << "': " << ex.what()
                 << " pattern(raw): " << patternRaw << " pattern(norm): " << pattern << "\n";
        }
    }
    return steps;
}

static string applyPreprocess(const vector<PreStep>& steps, const string& in) {
    string out = in;
    for (const auto& s : steps) {
        try {
            out = regex_replace(out, s.rx, s.replace);
        } 
        catch (const regex_error& ex) {
            cerr << "Warning: preprocess replace failed: " << ex.what() << "\n";
        }
    }
    return out;
}

static string normalize(const string& input) {
    string out;
    out.reserve(input.size());
    for (unsigned char uc : input) {
        if (isalnum(uc) || isspace(uc) || uc == ',' || uc == '"' || uc == '=') out.push_back((char)tolower(uc));
    }
    auto new_end = unique(out.begin(), out.end(), [](char a, char b) {
        return isspace(a) && isspace(b);
    });
    out.erase(new_end, out.end());
    size_t start = out.find_first_not_of(' ');
    size_t end = out.find_last_not_of(' ');
    if (start == string::npos) return "";
    return out.substr(start, end - start + 1);
}

static bool tryMapLookup(const nlohmann::json& keywords, const string& table, const string& key, string& out) {
    if (keywords.contains("maps") && keywords["maps"].is_object()
        && keywords["maps"].contains(table) && keywords["maps"][table].is_object()
        && keywords["maps"][table].contains(key)) {
        out = keywords["maps"][table][key].get<string>();
        return true;
    }
    if (keywords.contains(table) && keywords[table].is_object() && keywords[table].contains(key)) {
        out = keywords[table][key].get<string>();
        return true;
    }
    return false;
}

static string renderMaps(const string& in, const smatch& m, const nlohmann::json& keywords) {
    static const regex map_rx(R"(\$\{map:([A-Za-z0-9_]+),(\d+)\})", regex_constants::ECMAScript);
    string out = in;
    smatch mm;
    size_t searchStart = 0;
    while (regex_search(out.cbegin() + searchStart, out.cend(), mm, map_rx)) {
        string table = mm[1].str();
        int idx = stoi(mm[2].str());
        string key = (idx < (int)m.size() ? m[idx].str() : "");
        string repl = key;
        string mapped;
        if (!key.empty() && tryMapLookup(keywords, table, key, mapped)) repl = mapped;
        auto pos = mm.position(0) + searchStart;
        out.replace(pos, mm.length(0), repl);
        searchStart = pos + repl.size();
    }
    return out;
}

static string renderIf(const string& in, const smatch& m) {
    static const regex if_rx(R"(\$\{if:(\d+):([^}]*)\})", regex_constants::ECMAScript);
    string out = in;
    smatch mm;
    size_t searchStart = 0;
    while (regex_search(out.cbegin() + searchStart, out.cend(), mm, if_rx)) {
        int idx = stoi(mm[1].str());
        string body = mm[2].str();
        string repl = (idx < (int)m.size() && m[idx].matched) ? body : string();
        auto pos = mm.position(0) + searchStart;
        out.replace(pos, mm.length(0), repl);
        searchStart = pos + repl.size();
    }
    return out;
}

QString CodeGenerator::generateCppCode(const QStringList& instructions, const nlohmann::json& keywords) {
    QString code;
    if (keywords.contains("templates") && keywords["templates"].contains("main_begin")) {
        code += QString::fromStdString(keywords["templates"]["main_begin"].get<string>());
    } 
    else code += "#include <iostream>\nint main(){\n";

    vector<Rule> rules = loadRules(keywords);
    vector<PreStep> preprocess = loadPreprocess(keywords);

    for (const QString& qline : instructions) {
        string original = qline.toStdString();
        string pre = applyPreprocess(preprocess, original);
        string line = normalize(pre);

        if (line.empty()) {
            code += "\n";
            continue;
        }

        int bestGroups = -1;
        string bestTemplate;
        string bestRuleName;
        smatch bestMatch;

        auto tryMatch = [&](bool exact) {
            for (const Rule& r : rules) {
                smatch match;
                bool ok = exact ? regex_match(line, match, r.pattern)
                                : regex_search(line, match, r.pattern);
                if (!ok) continue;

                int matchedGroups = 0;
                for (size_t g = 1; g < match.size(); ++g)
                    if (match[g].matched) ++matchedGroups;

                if (matchedGroups > bestGroups) {
                    bestGroups = matchedGroups;
                    bestTemplate = r.templateStr;
                    bestRuleName = r.name;
                    bestMatch = match;
                }
            }
        };

        tryMatch(true);
        if (bestGroups < 0) tryMatch(false);

        if (bestGroups >= 0) {
            string temp = bestTemplate;
            vector<string> matchValues;
            for (size_t i = 0; i < bestMatch.size(); ++i) {
                string val = bestMatch[i].str();
                string mapped;
                if (tryMapLookup(keywords, "tipo_cpp", val, mapped)) {
                    val = mapped;
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

            temp = renderMaps(temp, bestMatch, keywords);
            temp = renderIf(temp, bestMatch);

            temp = regex_replace(temp, regex(R"(\$\{\d+\})"), "");
            if (temp.find("cout") == string::npos) temp.erase(remove(temp.begin(), temp.end(), '\n'), temp.end());
            code += "// [rule:" + bestRuleName + "]\n";
            code += QString::fromStdString(temp);
            code += "\n";
        }   else code += "// " + QString::fromStdString(original) + "\n";
    }

    if (keywords.contains("templates") && keywords["templates"].contains("main_end")) {
        code += QString::fromStdString(keywords["templates"]["main_end"].get<string>());
    } 
    else code += "return 0;";

    return code;
}