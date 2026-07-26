/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "LogHighlighter.h"
#include "highlight/HighlightRules.h"
#include "highlight/HighlightRule.h"
#include <QRegExp>
#include <QTextCharFormat>
#include <QList>

LogHighlighter::LogHighlighter(QTextDocument * document)
    : QSyntaxHighlighter(document)
{
    connect(&HighlightRules::instance(), SIGNAL(rulesChanged()), SLOT(rehighlight()));
}

void LogHighlighter::highlightBlock(const QString & text)
{
    const QList<HighlightRule> & rules = HighlightRules::instance().rules();
    for(int i = 0; i < rules.size(); i++) {
        const HighlightRule & rule = rules[i];
        if(rule.pattern.isEmpty() || !rule.enabled) { continue; }

        QTextCharFormat fmt;
        fmt.setForeground(rule.color.foreground());
        fmt.setBackground(rule.color.background());

        QRegExp regex(rule.pattern,
            Qt::CaseSensitive,
            rule.isRegex ? QRegExp::RegExp2 : QRegExp::FixedString);

        int pos = 0;
        while((pos = regex.indexIn(text, pos)) != -1) {
            setFormat(pos, regex.matchedLength(), fmt);
            pos += regex.matchedLength();
            if(regex.matchedLength() == 0) { break; }
        }
    }
}
