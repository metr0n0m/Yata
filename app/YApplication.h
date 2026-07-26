/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef YAPPLICATION_H
#define YAPPLICATION_H

class QString;

class YApplication {
public:
    static QString displayAppName();
    static QString appVersion();
    static QString settingsPath();
    static QString sessionFilePath();
    static QString preferencesFilePath();
private:
    YApplication();
};

#endif // YAPPLICATION_H
