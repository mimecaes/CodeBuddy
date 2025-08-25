#include "Lexer.h"
#include <QStringList>

static bool isDigitStr(const QString& s) {
    if (s.isEmpty()) return false;
    int i = 0;
    if (s[0] == '+' || s[0] == '-') i = 1;
    if (i >= s.size()) return false;
    for (; i < s.size(); ++i) if (!s[i].isDigit()) return false;
    return true;
}

static QString stripPunctEdges(QString s) {
    auto isP = [](QChar c) { return c.isPunct(); };
    while (!s.isEmpty() && isP(s.front())) s.remove(0, 1);
    while (!s.isEmpty() && isP(s.back()))  s.chop(1);
    return s;
}

static QString normalizeText(const QString& in, bool lowercase, bool stripDia,
    bool collapseWS, bool trimPunct) {
    QString s = in;
    if (lowercase) s = s.toLower();

    if (stripDia) {
        s.replace(QChar(0x00E1), QLatin1Char('a'));
        s.replace(QChar(0x00E9), QLatin1Char('e'));
        s.replace(QChar(0x00ED), QLatin1Char('i'));
        s.replace(QChar(0x00F3), QLatin1Char('o'));
        s.replace(QChar(0x00FA), QLatin1Char('u'));
        s.replace(QChar(0x00F1), QLatin1Char('n'));
    }

    if (collapseWS) s = s.simplified(); else s = s.trimmed();

    if (trimPunct) {
        auto isP = [](QChar c) { return c.isPunct(); };
        while (!s.isEmpty() && isP(s.front())) s.remove(0, 1);
        while (!s.isEmpty() && isP(s.back()))  s.chop(1);
    }

    return s;
}


std::vector<Token> Lexer::tokenize(const QString& input, std::vector<std::string>&) const {
    const QString normAll = normalize(input);
    QStringList words = normAll.split(' ', Qt::SkipEmptyParts);
    for (QString& w : words) w = stripPunctEdges(w);

    int maxN = 1;
    for (auto it = cfg.flat.begin(); it != cfg.flat.end(); ++it) {
        if (it.value().nWords > maxN) maxN = it.value().nWords;
    }

    std::vector<Token> out;
    int i = 0;
    while (i < words.size()) {
        bool matched = false;

        if (i + 2 < words.size() && words[i] == "con" && isDigitStr(words[i + 1]) && words[i + 2].startsWith("elemento")) {
            Token t;
            t.kind = TokenKind::KwArraySize;
            t.lexeme = "con " + words[i + 1] + " elementos";
            t.payload = words[i + 1];
            t.nWords = 3;
            t.pos = i;
            out.push_back(t);
            i += 3;
            continue;
        }

        for (int n = std::min(maxN, words.size() - i); n >= 1; --n) {
            QString phrase;
            for (int k = 0; k < n; ++k) {
                if (k) phrase += ' ';
                phrase += words[i + k];
            }
            auto it = cfg.flat.find(phrase);
            if (it != cfg.flat.end()) {
                const FlatSpec& fs = it.value();
                Token t;
                t.kind = fs.kind;
                t.lexeme = phrase;
                t.payload = fs.payload;
                t.nWords = n;
                t.pos = i;

                if (t.kind == TokenKind::KwFileName) {
                    int next = i + n;
                    if (next < words.size()) {
                        QString fn = stripPunctEdges(words[next]);
                        t.payload = fn;
                        t.nWords = n + 1;
                    }
                }

                out.push_back(t);
                i += t.nWords;
                matched = true;
                break;
            }
        }

        if (!matched) {
            i += 1;
        }
    }

    return out;
}
