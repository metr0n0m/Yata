/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QObject>
#include <QScopedPointer>
#include "session/SessionCommon.h"

class QFont;
class TextColor;
namespace YAML { class Node; }

class Preferences: public QObject {
    Q_OBJECT
public:
    static Preferences * instance();

    void write();
    ParsingStatus::Enum read();
    const ParsingError * parsingError() const;

    const QFont & font() const;
    void setFont(const QFont & font);

    const TextColor & normalTextColor() const;
    void setNormalTextColor(const TextColor & color) const;

    const TextColor & selectedTextColor() const;
    void setSelectedTextColor(const TextColor & color);

    bool debugMenu() const;

    ~Preferences();

signals:
    void preferencesChanged();

private:
    Preferences();

    void readColor(TextColor * dest, const YAML::Node & src, const std::string & key);
private:
    QScopedPointer<QFont> m_font;
    QScopedPointer<TextColor> m_normalTextColor;
    QScopedPointer<TextColor> m_selectedTextColor;
    QScopedPointer<bool> m_debugMenu;
    QScopedPointer<ParsingError> m_parsingError;

    static Preferences * m_instance;
};

#endif // PREFERENCES_H
