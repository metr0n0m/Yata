/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "FilterScanThread.h"
#include "fileio/FileBlockReader.h"
#include <QRegExp>

FilterScanThread::FilterScanThread(const QString & filename,
                                   const QString & pattern,
                                   bool isRegex,
                                   bool caseSensitive,
                                   qint64 startPos,
                                   QObject * parent)
    : QThread(parent)
    , m_filename(filename)
    , m_pattern(pattern)
    , m_isRegex(isRegex)
    , m_caseSensitive(caseSensitive)
    , m_startPos(startPos)
    , m_stopped(false)
{
}

FilterScanThread::~FilterScanThread()
{
    stop();
}

void FilterScanThread::stop()
{
    m_stopped = true;
    wait();
}

void FilterScanThread::run()
{
    QRegExp regex(m_pattern,
        m_caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive,
        m_isRegex ? QRegExp::RegExp2 : QRegExp::FixedString);

    FileBlockReader reader(m_filename);
    QVector<qint64> found;
    qint64 pos = m_startPos;

    while(!m_stopped && pos < reader.size()) {
        QString line;
        std::vector<qint64> lineAddresses;
        if(!reader.readChunk(&line, &lineAddresses, pos, 0, 1)) { break; }
        if(lineAddresses.empty()) { break; }

        if(regex.indexIn(line) != -1) {
            found.append(lineAddresses.front());
        }

        qint64 nextPos = reader.getStartPosition(pos, 1);
        if(nextPos <= pos) { break; }
        pos = nextPos;
    }

    if(!m_stopped) {
        emit scanFinished(found);
    }
}
