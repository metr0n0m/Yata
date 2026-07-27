/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef FILTERSTATE_H
#define FILTERSTATE_H

#include <QString>
#include <QVector>
#include <QtGlobal>

class FilterState {
public:
    FilterState();

    bool isActive() const;
    void clear();

    void setPattern(const QString & pattern, bool isRegex, bool caseSensitive);
    const QString & pattern() const;
    bool isRegex() const;
    bool caseSensitive() const;

    // List of byte addresses of matching lines
    const QVector<qint64> & matchAddresses() const;
    void setMatchAddresses(const QVector<qint64> & addresses);
    void appendMatchAddresses(const QVector<qint64> & addresses);

private:
    bool m_active;
    QString m_pattern;
    bool m_isRegex;
    bool m_caseSensitive;
    QVector<qint64> m_matchAddresses;
};

#endif // FILTERSTATE_H
