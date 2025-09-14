#include "KeywordLoader.h"
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

nlohmann::json KeywordLoader::loadJson(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) throw runtime_error("Could not open JSON file: " + filename);

    stringstream ss;
    ss << file.rdbuf();
    const string content = ss.str();
    if (content.empty()) throw runtime_error("Empty JSON file: " + filename);

    try {
        nlohmann::json j = nlohmann::json::parse(content);
        validateJson(j);
        return j;
    } catch (const nlohmann::json::parse_error& ex) {
        throw runtime_error(string("JSON parse error: ") + ex.what());
    }
}

void KeywordLoader::validateJson(const nlohmann::json& j) {
    if (!j.is_object()) {
        throw runtime_error("Loaded JSON is not a valid object.");
    }

    if (!j.contains("templates") || !j["templates"].is_object()) {
        throw runtime_error("Missing 'templates' section in JSON.");
    }
    if (!j["templates"].contains("main_begin") || !j["templates"].contains("main_end")) {
        throw runtime_error("The 'templates' section must contain 'main_begin' and 'main_end'.");
    }

    if (!j.contains("maps") || !j["maps"].is_object() || !j["maps"].contains("binary_ops")) {
        throw runtime_error("Missing 'maps.binary_ops' in JSON. Add arithmetic operator mappings.");
    }

    if (j.contains("rules") && !j["rules"].is_object()) {
        throw runtime_error("The 'rules' section must be an object containing rules.");
    }
}

string KeywordLoader::getBinaryOp(const nlohmann::json& j, const string& word) {
    if (!j.contains("maps") || !j["maps"].contains("binary_ops")) return "";
    auto it = j["maps"]["binary_ops"].find(word);
    if (it != j["maps"]["binary_ops"].end())
        return it.value().get<string>();
    return "";
}

string KeywordLoader::getType(const nlohmann::json& j, const string& word) {
    if (j.contains("maps") && j["maps"].contains("tipo_cpp")) {
        auto it = j["maps"]["tipo_cpp"].find(word);
        if (it != j["maps"]["tipo_cpp"].end())
            return it.value().get<string>();
    }

    if (j.contains("tipo_cpp")) {
        auto it = j["tipo_cpp"].find(word);
        if (it != j["tipo_cpp"].end())
            return it.value().get<string>();
    }

    if (j.contains("types")) {
        auto it = j["types"].find(word);
        if (it != j["types"].end())
            return it.value().get<string>();
    }
    return "";
}

bool KeywordLoader::isControlKeyword(const nlohmann::json& j, const string& word) {
    if (!j.contains("keywords") || !j["keywords"].contains("control")) return false;
    const auto& arr = j["keywords"]["control"];
    return find(arr.begin(), arr.end(), word) != arr.end();
}

bool KeywordLoader::isIOKeyword(const nlohmann::json& j, const string& word) {
    if (!j.contains("keywords") || !j["keywords"].contains("io")) return false;
    const auto& arr = j["keywords"]["io"];
    return find(arr.begin(), arr.end(), word) != arr.end();
}

bool KeywordLoader::isArrayKeyword(const nlohmann::json& j, const string& word) {
    if (!j.contains("keywords") || !j["keywords"].contains("arrays")) return false;
    const auto& arr = j["keywords"]["arrays"];
    return find(arr.begin(), arr.end(), word) != arr.end();
}