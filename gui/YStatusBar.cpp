/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#include "YStatusBar.h"
#include <QLabel>
#include <QStringBuilder>

YStatusBar::YStatusBar(QWidget *parent):
    QStatusBar(parent),
    m_leftLabel(new QLabel(this)),
    m_filterLabel(new QLabel(this))
{
    m_leftLabel->setMinimumSize(QSize(1,1));
    addWidget(m_leftLabel.data());

    m_filterLabel->setMinimumSize(QSize(1,1));
    addPermanentWidget(m_filterLabel.data());
}

YStatusBar::~YStatusBar()
{
}

void YStatusBar::errorMessage(const QString & msg)
{
    QString format = QString("<p style=\"background-color:yellow\">") %
        msg % "</p>";
    m_leftLabel->setText(format);
}

void YStatusBar::clearErrorMessage()
{
    m_leftLabel->clear();
}

void YStatusBar::filterMessage(const QString & msg)
{
    QString format = QString("<p style=\"background-color:#c8e6ff; color:#00008b;\">") %
        msg % "</p>";
    m_filterLabel->setText(format);
}

void YStatusBar::clearFilterMessage()
{
    m_filterLabel->clear();
}
