/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#ifndef YSTATUSBAR_H
#define YSTATUSBAR_H

#include <QStatusBar>
#include "app/YObjectPointer.h"

class QLabel;

class YStatusBar: public QStatusBar {
    Q_OBJECT

public:
    explicit YStatusBar(QWidget *parent = 0);
    ~YStatusBar();

public slots:
    void errorMessage(const QString & msg);
    void clearErrorMessage();
    void filterMessage(const QString & msg);
    void clearFilterMessage();
    void lineInfo(int currentLine, int totalLines);
    void clearLineInfo();

private:
    YObjectPointer<QLabel> m_leftLabel;
    YObjectPointer<QLabel> m_filterLabel;
    YObjectPointer<QLabel> m_lineLabel;
};

#endif // YSTATUSBAR_H
