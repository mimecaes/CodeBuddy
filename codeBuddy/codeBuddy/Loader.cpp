#include "Loader.h"
#include "Core.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

static QString normalizeText(const QString& in, bool lowercase, bool stripDia, bool collapseWS, bool trimPunct) {
    QString s = in;

    if (lowercase) s = s.toLower();

    if (stripDia) {
        s.replace(QStringLiteral("á"), QStringLiteral("a"));
        s.replace(QStringLiteral("é"), QStringLiteral("e"));
        s.replace(QStringLiteral("í"), QStringLiteral("i"));
        s.replace(QStringLiteral("ó"), QStringLiteral("o"));
        s.replace(QStringLiteral("ú"), QStringLiteral("u"));
        s.replace(QStringLiteral("ñ"), QStringLiteral("n"));
    }

    if (collapseWS)  s = s.simplified();

    else if (trimPunct) s = s.trimmed();

    if (trimPunct) {
        auto isPunct = [](QChar c) { return c.isPunct(); };
        while (!s.isEmpty() && isPunct(s.front())) s.remove(0, 1);
        while (!s.isEmpty() && isPunct(s.back()))  s.chop(1);
    }

    return s;
}

std::optional<DictConfig> Loader::loadFromFile(const QString& path) {
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) return std::nullopt;

    const QByteArray data = f.readAll();
    const QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) return std::nullopt;

    const QJsonObject root = doc.object();
    DictConfig cfg;

    if (root.contains("meta") && root["meta"].isObject()) {
        const QJsonObject m = root["meta"].toObject();

        if (m.contains("normalization") && m["normalization"].isObject()) {
            const QJsonObject n = m["normalization"].toObject();
            cfg.meta.lowercase = n.value("lowercase").toBool(true);
            cfg.meta.stripDia = n.value("strip_diacritics").toBool(true);
            cfg.meta.collapseWS = n.value("collapse_whitespace").toBool(true);
            cfg.meta.trimPunct = n.value("trim_punctuation").toBool(true);
        }

        if (m.contains("assign_synonyms") && m["assign_synonyms"].isArray()) {
            for (const auto& v : m["assign_synonyms"].toArray())
                cfg.meta.assignSyns.push_back(v.toString());
        }

        if (m.contains("type_inference") && m["type_inference"].isObject()) {
            const QJsonObject ti = m["type_inference"].toObject();
            cfg.meta.intRegex = ti.value("int_regex").toString();
            cfg.meta.doubleRegex = ti.value("double_regex").toString();
            cfg.meta.charRegex = ti.value("char_regex").toString();
            cfg.meta.stringRegex = ti.value("string_regex").toString();
        }

        if (m.contains("errors") && m["errors"].isObject()) {
            const QJsonObject e = m["errors"].toObject();
            cfg.meta.errUnknown = e.value("unknown_instruction").toString();
            cfg.meta.errArrSize = e.value("missing_array_size").toString();
            cfg.meta.errMissingType = e.value("missing_type").toString();
            cfg.meta.errInvalidCond = e.value("invalid_condition").toString();
            cfg.meta.errUndeclared = e.value("undeclared_var").toString();
        }
    }

    auto normalize = [&](const QString& s) {
        return normalizeText(s,
            cfg.meta.lowercase,
            cfg.meta.stripDia,
            cfg.meta.collapseWS,
            cfg.meta.trimPunct);
    };

    if (root.contains("maps") && root["maps"].isObject()) {
        const QJsonObject maps = root["maps"].toObject();
        for (auto it = maps.begin(); it != maps.end(); ++it) {
            const QString key = it.key();
            if (!it.value().isObject()) continue;
            const QJsonObject obj = it.value().toObject();

            for (auto it2 = obj.begin(); it2 != obj.end(); ++it2) {
                const QString phrase = normalize(it2.key());
                const QString payload = it2.value().toString();

                FlatSpec fs;
                if (key == "binary_ops")  fs.kind = TokenKind::Op;
                else if (key == "comparators") fs.kind = TokenKind::Cmp;
                else if (key == "bool_ops")    fs.kind = TokenKind::BoolOp;
                else continue;

                fs.payload = payload;
                fs.nWords = phrase.split(' ').size();
                cfg.flat.insert(phrase, fs);
            }
        }
    }

    if (root.contains("types") && root["types"].isObject()) {
        const QJsonObject types = root["types"].toObject();
        for (auto it = types.begin(); it != types.end(); ++it) {
            const QString phrase = normalize(it.key());
            const QString cpp = it.value().toString();

            FlatSpec fs;
            fs.kind = TokenKind::KwVar;
            fs.payload = cpp;
            fs.nWords = phrase.split(' ').size();

            cfg.flat.insert(phrase, fs);
        }
    }

    if (root.contains("keywords") && root["keywords"].isObject()) {
        const QJsonObject kws = root["keywords"].toObject();

        auto addKw = [&](const QString& phrase, TokenKind k) {
            FlatSpec fs;
            fs.kind = k;
            fs.payload.clear();
            fs.nWords = phrase.split(' ').size();
            cfg.flat.insert(phrase, fs);
            };

        for (auto it = kws.begin(); it != kws.end(); ++it) {
            const QString k = it.key();
            if (!it.value().isArray()) continue;
            const QJsonArray arr = it.value().toArray();

            for (const auto& v : arr) {
                const QString phrase = normalize(v.toString());

                if (k == "control") {
                    if (phrase == "si") addKw(phrase, TokenKind::KwIf);
                    else if (phrase == "sino") addKw(phrase, TokenKind::KwElse);
                    else if (phrase == "mientras") addKw(phrase, TokenKind::KwWhile);
                    else if (phrase == "repetir hasta") addKw(phrase, TokenKind::KwDoWhile);
                    else if (phrase == "para") addKw(phrase, TokenKind::KwFor);
                    else addKw(phrase, TokenKind::Unknown);

                }
                else if (k == "variable") {
                    if (phrase == "crear variable") addKw(phrase, TokenKind::KwVar);
                    else if (phrase == "asignar valor" || phrase == "valor inicial")
                        addKw(phrase, TokenKind::KwAssign);

                }
                else if (k == "arrays") {
                    if (phrase == "crear lista" || phrase == "crear arreglo" || phrase == "crear vector") {
                        FlatSpec fs; fs.kind = TokenKind::KwArrayDecl; fs.payload = "vector";
                        fs.nWords = phrase.split(' ').size();
                        cfg.flat.insert(phrase, fs);
                    }
                    else if (phrase.startsWith("lista de") || phrase.startsWith("vector de")) {
                        FlatSpec fs; fs.kind = TokenKind::KwArrayOfType; fs.payload = "vector";
                        fs.nWords = phrase.split(' ').size();
                        cfg.flat.insert(phrase, fs);
                    }
                    else if (phrase.startsWith("con ")) {
                        FlatSpec fs; fs.kind = TokenKind::KwArraySize; fs.payload.clear();
                        fs.nWords = phrase.split(' ').size();
                        cfg.flat.insert(phrase, fs);
                    }
                    else if (phrase == "recorrer la lista") {
                        addKw(phrase, TokenKind::KwForList);
                    }
                    else if (phrase == "agregar a la lista") {
                        addKw(phrase, TokenKind::KwPushList);
                    }

                }
                else if (k == "io") {
                    if (phrase == "mostrar" || phrase == "imprimir") {
                        FlatSpec fs; fs.kind = TokenKind::Unknown; fs.payload = "cout"; fs.nWords = 1;
                        cfg.flat.insert(phrase, fs);
                    }
                    else if (phrase == "leer" || phrase == "ingresar valor" ||
                        phrase == "ingresar" || phrase == "ingrese" ||
                        phrase == "pedir al usuario" || phrase == "por consola") {
                        FlatSpec fs; fs.kind = TokenKind::Unknown; fs.payload = "cin";
                        fs.nWords = phrase.split(' ').size();
                        cfg.flat.insert(phrase, fs);
                    }
                    else if (phrase == "mensaje") {
                        FlatSpec fs; fs.kind = TokenKind::Unknown; fs.payload = "STRING_IN_COUT"; fs.nWords = 1;
                        cfg.flat.insert(phrase, fs);
                    }

                }
                else if (k == "program") {
                    if (phrase == "comenzar programa") addKw(phrase, TokenKind::KwProgramBegin);
                    else if (phrase == "terminar programa" || phrase == "fin")
                        addKw(phrase, TokenKind::KwProgramEnd);
                    else if (phrase == "comentario")        addKw(phrase, TokenKind::KwComment);
                    else if (phrase == "definir funcion")   addKw(phrase, TokenKind::KwFuncDef);
                    else if (phrase == "llamar funcion")    addKw(phrase, TokenKind::KwFuncCall);
                    else if (phrase == "leer archivo")      addKw(phrase, TokenKind::KwFileRead);
                    else if (phrase == "archivo llamado")   addKw(phrase, TokenKind::KwFileName);
                }
            }
        }
    }

    if (root.contains("meta") && root["meta"].isObject()) {
        const QJsonObject mm = root["meta"].toObject();
        if (mm.contains("assign_synonyms") && mm["assign_synonyms"].isArray()) {
            for (const auto& v : mm["assign_synonyms"].toArray()) {
                QString phrase = normalize(v.toString());

                FlatSpec fs;
                fs.kind = TokenKind::AssignSyn;
                fs.payload = "=";
                fs.nWords = phrase.split(' ').size();

                cfg.flat.insert(phrase, fs);
            }
        }
    }

    return cfg;
}
