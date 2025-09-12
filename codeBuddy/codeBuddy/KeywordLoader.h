    #pragma once
#include <string>
#include "json.hpp"
#include <stdexcept>
using namespace std;

class KeywordLoader {
public:
    static nlohmann::json loadJson(const string& filename);
    static void validateJson(const nlohmann::json& j);

    static string getBinaryOp(const nlohmann::json& j, const string& palabra);
    static string getType(const nlohmann::json& j, const string& palabra);
    static bool isControlKeyword(const nlohmann::json& j, const string& palabra);
    static bool isIOKeyword(const nlohmann::json& j, const string& palabra);
    static bool isArrayKeyword(const nlohmann::json& j, const string& palabra);
};