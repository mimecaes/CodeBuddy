#pragma once
#include "Core.h"
#include "Loader.h"
#include <vector>
#include <QString>

class Lexer {
public:
    explicit Lexer(const DictConfig& cfg) : cfg(cfg) {}
    std::vector<Token> tokenize(const QString& input, std::vector<std::string>& errors) const;
private:
    const DictConfig& cfg;
    QString normalize(const QString& s) const;
};
