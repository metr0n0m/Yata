/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef LOGHIGHLIGHTER_H
#define LOGHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class QTextDocument;

class LogHighlighter: public QSyntaxHighlighter {
    Q_OBJECT
public:
    explicit LogHighlighter(QTextDocument * document);

protected:
    void highlightBlock(const QString & text);
};

#endif // LOGHIGHLIGHTER_H
