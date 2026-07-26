/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef SESSION_LOADER_H
#define SESSION_LOADER_H

#include <string>

class MainWindow;

class SessionLoader {
public:
    static void readSession(MainWindow * win);
    static void writeSession(MainWindow * win);
private:
    static std::string nativeSessionPath();
};

#endif
