/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <QScopedPointer>

namespace Ui {
    class FilterDialog;
}

class FilterDialog: public QDialog {
    Q_OBJECT
public:
    explicit FilterDialog(QWidget * parent = 0);
    ~FilterDialog();

    void setKeyword(const QString & keyword);

    QString keyword() const;
    bool isRegex() const;
    bool isCaseSensitive() const;

private:
    QScopedPointer<Ui::FilterDialog> ui;
};

#endif // FILTERDIALOG_H
