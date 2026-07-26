/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef HIGHLIGHTINGDIALOG_H
#define HIGHLIGHTINGDIALOG_H

#include "highlight/HighlightRule.h"
#include <QDialog>
#include <QList>
#include <QScopedPointer>

namespace Ui {
    class HighlightingDialog;
}

class HighlightingDialog: public QDialog {
    Q_OBJECT
public:
    explicit HighlightingDialog(QWidget * parent = 0);
    ~HighlightingDialog();

private slots:
    void on_addButton_clicked();
    void on_editButton_clicked();
    void on_deleteButton_clicked();
    void on_rulesTable_cellChanged(int row, int column);
    void on_buttonBox_accepted();

private:
    void populateTable();
    void addRow(int row, const HighlightRule & rule);
    void updateButtons();

    QScopedPointer<Ui::HighlightingDialog> ui;
    QList<HighlightRule> m_rules;
};

#endif // HIGHLIGHTINGDIALOG_H
