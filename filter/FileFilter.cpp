/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "FileFilter.h"
#include "fileio/FileBlockReader.h"
#include <QRegExp>
#include <QVector>

int FileFilter::scanAll(const QString & filename, FilterState * state)
{
    FileBlockReader reader(filename);
    return scan(reader, 0, state);
}

int FileFilter::scanFrom(const QString & filename, qint64 startPos, FilterState * state)
{
    FileBlockReader reader(filename);
    return scan(reader, startPos, state);
}

int FileFilter::scan(FileBlockReader & reader, qint64 startPos, FilterState * state)
{
    QRegExp regex(state->pattern(),
        state->caseSensitive() ? Qt::CaseSensitive : Qt::CaseInsensitive,
        state->isRegex() ? QRegExp::RegExp2 : QRegExp::FixedString);

    QVector<qint64> found;
    qint64 pos = startPos;

    while(pos < reader.size()) {
        QString line;
        std::vector<qint64> lineAddresses;
        // Read one line at a time
        if(!reader.readChunk(&line, &lineAddresses, pos, 0, 1)) { break; }
        if(lineAddresses.empty()) { break; }

        if(regex.indexIn(line) != -1) {
            found.append(lineAddresses.front());
        }

        // Advance to next line
        qint64 nextPos = reader.getStartPosition(pos, 1);
        if(nextPos <= pos) { break; }
        pos = nextPos;
    }

    state->appendMatchAddresses(found);
    return found.size();
}
