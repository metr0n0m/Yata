/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef HIGHLIGHTRULES_H
#define HIGHLIGHTRULES_H

#include "HighlightRule.h"
#include <QObject>
#include <QList>

class HighlightRules: public QObject {
    Q_OBJECT
public:
    static HighlightRules & instance();

    void read();
    void write();

    const QList<HighlightRule> & rules() const;
    void setRules(const QList<HighlightRule> & rules);

signals:
    void rulesChanged();

private:
    HighlightRules();

private:
    QList<HighlightRule> m_rules;
};

#endif // HIGHLIGHTRULES_H
