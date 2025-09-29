#pragma once
#include <string>
#include "json.hpp"
#include <stdexcept>
using namespace std;

class KeywordLoader {
public:
    static nlohmann::json loadJson(const string& filename);
    static string getBinaryOp(const nlohmann::json& j, const string& word);
    static string getType(const nlohmann::json& j, const string& word);
    static bool isControlOp(const nlohmann::json& j, const string& word);
    static bool isIOOp(const nlohmann::json& j, const string& word);
    static bool isArrayOp(const nlohmann::json& j, const string& word);
};