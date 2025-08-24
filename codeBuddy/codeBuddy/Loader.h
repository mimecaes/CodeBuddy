#pragma once
#include "Core.h"
#include <QString>
#include <QHash>
#include <optional>
#include <vector>

struct MetaConfig {
    bool lowercase{ true };
    bool stripDia{ true };
    bool collapseWS{ true };
    bool trimPunct{ true };

    std::vector<QString> assignSyns;
    QString intRegex, doubleRegex, charRegex, stringRegex;
    QString errUnknown, errArrSize, errMissingType, errInvalidCond, errUndeclared;
};

struct FlatSpec {
    TokenKind kind{ TokenKind::Unknown };
    QString   payload;
    int       nWords{ 1 };
};

struct DictConfig {
    MetaConfig               meta;
    QHash<QString, FlatSpec> flat;
};

class Loader {
public:
    static std::optional<DictConfig> loadFromFile(const QString& path);
};
