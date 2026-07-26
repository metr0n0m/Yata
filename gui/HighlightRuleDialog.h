/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef HIGHLIGHTRULEDIALOG_H
#define HIGHLIGHTRULEDIALOG_H

#include "highlight/HighlightRule.h"
#include <QDialog>
#include <QScopedPointer>

namespace Ui {
    class HighlightRuleDialog;
}

class HighlightRuleDialog: public QDialog {
    Q_OBJECT
public:
    explicit HighlightRuleDialog(QWidget * parent = 0);
    ~HighlightRuleDialog();

    void setRule(const HighlightRule & rule);
    HighlightRule rule() const;

private slots:
    void on_buttonBox_accepted();
    void updatePreview();

private:
    QScopedPointer<Ui::HighlightRuleDialog> ui;
    HighlightRule m_rule;
};

#endif // HIGHLIGHTRULEDIALOG_H
