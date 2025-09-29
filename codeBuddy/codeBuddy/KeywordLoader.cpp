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
        return j;
    }
    catch (const nlohmann::json::parse_error& ex) {
        throw runtime_error(string("JSON parse error: ") + ex.what());
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
    return "";
}

bool KeywordLoader::isControlOp(const nlohmann::json& j, const string& word) {
    if (!j.contains("control_ops") || !j["control_ops"].is_object()) return false;
    for (auto it = j["control_ops"].begin(); it != j["control_ops"].end(); ++it) {
        if (it.key() == word) return true;
    }
    return false;
}

bool KeywordLoader::isIOOp(const nlohmann::json& j, const string& word) {
    if (!j.contains("rules") || !j["rules"].is_object()) return false;
    static const vector<string> io_patterns = { "mostrar", "imprimir", "leer", "cin", "cout" };
    for (auto it = j["rules"].begin(); it != j["rules"].end(); ++it) {
        for (const auto& pat : io_patterns) {
            if (it.key().find(pat) != string::npos) {
                if (word == pat) return true;
            }
        }
    }
    return false;
}

bool KeywordLoader::isArrayOp(const nlohmann::json& j, const string& word) {
    if (!j.contains("rules") || !j["rules"].is_object()) return false;
    static const vector<string> array_patterns = { "lista", "arreglo", "vector" };
    for (auto it = j["rules"].begin(); it != j["rules"].end(); ++it) {
        for (const auto& pat : array_patterns) {
            if (it.key().find(pat) != string::npos) {
                if (word == pat) return true;
            }
        }
    }
    return false;
}