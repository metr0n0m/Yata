/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef FILEFILTER_H
#define FILEFILTER_H

#include "FilterState.h"
#include <QString>
#include <QVector>
#include <QtGlobal>

class FileBlockReader;

class FileFilter {
public:
    // Scan entire file, populate state with matching line addresses.
    // Returns number of matches found.
    static int scanAll(const QString & filename, FilterState * state);

    // Scan only from startPos to end of file, append to state.
    // Used for tail updates — only new content scanned.
    static int scanFrom(const QString & filename, qint64 startPos, FilterState * state);

private:
    static int scan(FileBlockReader & reader, qint64 startPos, FilterState * state);
};

#endif // FILEFILTER_H
