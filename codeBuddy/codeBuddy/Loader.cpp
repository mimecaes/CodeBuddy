#include "Loader.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSet>
#include <stdexcept>

static std::runtime_error Err(const QString& m) {
    return std::runtime_error(m.toStdString());
}

DictConfig Loader::loadDictOrThrow(const QString& jsonPath) {
    QFile f(jsonPath);
    if (!f.open(QIODevice::ReadOnly))
        throw Err(QString("No pude abrir el archivo: %1").arg(jsonPath));

    const QByteArray bytes = f.readAll();
    f.close();

    QJsonParseError jerr;
    const QJsonDocument doc = QJsonDocument::fromJson(bytes, &jerr);
    if (jerr.error != QJsonParseError::NoError)
        throw Err(QString("Archivo JSON inválido (%1) en %2").arg(jerr.errorString(), jsonPath));
    if (!doc.isObject())
        throw Err("El archivo JSON raíz debe ser un objeto");

    const QJsonObject root = doc.object();

    if (!root.contains("meta") || !root["meta"].isObject())
        throw Err("Falta objeto 'meta'");
    const QJsonObject meta = root["meta"].toObject();
    const bool caseSensitive = meta["caseSensitive"].toBool(false);
    const bool strict = meta["strict"].toBool(false);
    if (!caseSensitive || !strict)
        throw Err("Config inválida: se requiere caseSensitive=true y strict=true");

    if (!root.contains("tokens") || !root["tokens"].isObject())
        throw Err("Falta objeto 'tokens'");
    const QJsonObject toks = root["tokens"].toObject();
    if (toks.isEmpty())
        throw Err("'tokens' está vacío");

    QHash<QString, KeywordSpec> dict;
    QSet<QString> seen;

    for (auto it = toks.begin(); it != toks.end(); ++it) {
        const QString phrase = it.key();
        const auto specVal = it.value();

        if (phrase.isEmpty())
            throw Err("Clave vacía en 'tokens'");
        if (!specVal.isObject())
            throw Err(QString("La entrada '%1' debe ser un objeto").arg(phrase));
        if (seen.contains(phrase))
            throw Err(QString("Duplicado en 'tokens': '%1'").arg(phrase));
        seen.insert(phrase);

        const QJsonObject specObj = specVal.toObject();
        KeywordSpec ks;
        ks.type = specObj["type"].toString();
        ks.cpp = specObj["cpp"].toString();

        if (ks.type.isEmpty())
            throw Err(QString("Falta 'type' en la entrada '%1'").arg(phrase));

        dict.insert(phrase, ks);
    }

    DictConfig cfg;
    cfg.caseSensitive = caseSensitive;
    cfg.strict = strict;
    cfg.tokens = std::move(dict);
    return cfg;
}
