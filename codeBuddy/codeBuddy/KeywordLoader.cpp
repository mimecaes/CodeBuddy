#include "KeywordLoader.h"
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

nlohmann::json KeywordLoader::loadJson(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("No se pudo abrir el archivo JSON: " + filename);
    }

    stringstream ss;
    ss << file.rdbuf();
    const string content = ss.str();
    if (content.empty()) {
        throw runtime_error("Archivo JSON vacío: " + filename);
    }

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
        throw runtime_error("El JSON cargado no es un objeto válido.");
    }

    if (!j.contains("templates") || !j["templates"].is_object()) {
        throw runtime_error("Falta la sección 'templates' en el JSON.");
    }
    if (!j["templates"].contains("main_begin") || !j["templates"].contains("main_end")) {
        throw runtime_error("La sección 'templates' debe contener 'main_begin' y 'main_end'.");
    }

    if (!j.contains("maps") || !j["maps"].is_object() || !j["maps"].contains("binary_ops")) {
        throw runtime_error("Falta 'maps.binary_ops' en el JSON. Añade operaciones aritméticas.");
    }

    if (j.contains("rules") && !j["rules"].is_object()) {
        throw runtime_error("La sección 'rules' debe ser un objeto con reglas.");
    }
}

string KeywordLoader::getBinaryOp(const nlohmann::json& j, const string& palabra) {
    if (!j.contains("maps") || !j["maps"].contains("binary_ops")) return "";
    auto it = j["maps"]["binary_ops"].find(palabra);
    if (it != j["maps"]["binary_ops"].end())
        return it.value().get<string>();
    return "";
}

string KeywordLoader::getType(const nlohmann::json& j, const string& palabra) {
    if (!j.contains("types")) return "";
    auto it = j["types"].find(palabra);
    if (it != j["types"].end())
        return it.value().get<string>();
    return "";
}

bool KeywordLoader::isControlKeyword(const nlohmann::json& j, const string& palabra) {
    if (!j.contains("keywords") || !j["keywords"].contains("control")) return false;
    const auto& arr = j["keywords"]["control"];
    return find(arr.begin(), arr.end(), palabra) != arr.end();
}

bool KeywordLoader::isIOKeyword(const nlohmann::json& j, const string& palabra) {
    if (!j.contains("keywords") || !j["keywords"].contains("io")) return false;
    const auto& arr = j["keywords"]["io"];
    return find(arr.begin(), arr.end(), palabra) != arr.end();
}

bool KeywordLoader::isArrayKeyword(const nlohmann::json& j, const string& palabra) {
    if (!j.contains("keywords") || !j["keywords"].contains("arrays")) return false;
    const auto& arr = j["keywords"]["arrays"];
    return find(arr.begin(), arr.end(), palabra) != arr.end();
}