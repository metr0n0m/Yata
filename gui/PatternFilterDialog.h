/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef PATTERNFILTERDIALOG_H
#define PATTERNFILTERDIALOG_H

#include "highlight/HighlightRule.h"
#include <QDialog>
#include <QScopedPointer>

namespace Ui {
    class PatternFilterDialog;
}

class PatternFilterDialog: public QDialog {
    Q_OBJECT
public:
    explicit PatternFilterDialog(QWidget * parent = 0);
    ~PatternFilterDialog();

    // Returns selected HighlightRule, or empty rule if none selected
    HighlightRule selectedRule() const;

private:
    QScopedPointer<Ui::PatternFilterDialog> ui;
    QList<HighlightRule> m_rules;
};

#endif // PATTERNFILTERDIALOG_H
