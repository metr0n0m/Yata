/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "FilterState.h"

FilterState::FilterState()
    : m_active(false)
    , m_isRegex(false)
    , m_caseSensitive(false)
{
}

bool FilterState::isActive() const
{
    return m_active;
}

void FilterState::clear()
{
    m_active = false;
    m_pattern.clear();
    m_matchAddresses.clear();
}

void FilterState::setPattern(const QString & pattern, bool isRegex, bool caseSensitive)
{
    m_pattern = pattern;
    m_isRegex = isRegex;
    m_caseSensitive = caseSensitive;
    m_active = !pattern.isEmpty();
    m_matchAddresses.clear();
}

const QString & FilterState::pattern() const
{
    return m_pattern;
}

bool FilterState::isRegex() const
{
    return m_isRegex;
}

bool FilterState::caseSensitive() const
{
    return m_caseSensitive;
}

const QVector<qint64> & FilterState::matchAddresses() const
{
    return m_matchAddresses;
}

void FilterState::setMatchAddresses(const QVector<qint64> & addresses)
{
    m_matchAddresses = addresses;
}

void FilterState::appendMatchAddresses(const QVector<qint64> & addresses)
{
    m_matchAddresses += addresses;
}
