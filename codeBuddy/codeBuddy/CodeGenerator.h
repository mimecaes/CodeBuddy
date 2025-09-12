#pragma once
#include <QString>
#include <QStringList>
#include <vector>
#include <regex>
#include <string>
#include "json.hpp"
using namespace std;

struct Rule {
    regex pattern;
    string templateStr;
    string name;
};

class CodeGenerator {
public:
    static QString generateCppCode(const QStringList& instrucciones, const nlohmann::json& keywords);
};