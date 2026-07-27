/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef FILTERSCANTHREAD_H
#define FILTERSCANTHREAD_H

#include <QThread>
#include <QVector>
#include <QString>
#include <QtGlobal>

class FilterScanThread: public QThread {
    Q_OBJECT
public:
    FilterScanThread(const QString & filename,
                     const QString & pattern,
                     bool isRegex,
                     bool caseSensitive,
                     qint64 startPos,
                     QObject * parent = 0);
    ~FilterScanThread();

    void stop();

signals:
    void scanFinished(QVector<qint64> matchAddresses);

protected:
    void run();

private:
    QString m_filename;
    QString m_pattern;
    bool m_isRegex;
    bool m_caseSensitive;
    qint64 m_startPos;
    volatile bool m_stopped;
};

#endif // FILTERSCANTHREAD_H
