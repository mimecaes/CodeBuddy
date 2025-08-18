#pragma once
#include <QHash>
#include <QString>

struct KeywordSpec {
    QString type;  // keyword
    QString cpp;   // codigo c++
};

struct DictConfig {
    bool caseSensitive = true;
    bool strict = true;
    QHash<QString, KeywordSpec> tokens;  //pasamos de una frase a un objeto en memoria
};

class Loader {
public:
    static DictConfig loadDictOrThrow(const QString& jsonPath);
};
