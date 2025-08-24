#pragma once
#include <QString>
#include <string>
#include <vector>

enum class TokenKind {
    Unknown = 0,
    Op,
    Cmp,
    BoolOp,
    AssignSyn,
    KwIf, KwElse, KwWhile, KwDoWhile, KwFor,
    KwVar,
    KwAssign,
    KwArrayDecl,
    KwArrayOfType,
    KwArraySize,
    KwForList,
    KwPushList,
    KwProgramBegin, KwProgramEnd, KwComment, KwFuncDef, KwFuncCall,
    KwFileRead,
    KwFileName
};

struct Token {
    TokenKind kind{ TokenKind::Unknown };
    QString   lexeme;
    QString   payload;
    int       nWords{ 1 };
    int       pos{ 0 };
};

enum class IRKind {
    VarDecl,
    Assign,
    ExprStmt,
    Print,
    ArrayDecl,
    If,
    ProgramWrap
};

struct IRNode {
    IRKind       kind{ IRKind::ExprStmt };
    std::string  a;
    std::string  b;
    std::string  c;
    std::string  d;
};

struct TranslationResult {
    std::string               code;
    std::vector<std::string>  errors;
};
