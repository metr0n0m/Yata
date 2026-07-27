/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "PatternFilterDialog.h"
#include "ui_PatternFilterDialog.h"
#include "highlight/HighlightRules.h"

PatternFilterDialog::PatternFilterDialog(QWidget * parent)
    : QDialog(parent)
    , ui(new Ui::PatternFilterDialog)
{
    ui->setupUi(this);

    m_rules = HighlightRules::instance().rules();
    for(int i = 0; i < m_rules.size(); i++) {
        const HighlightRule & rule = m_rules[i];
        if(!rule.enabled || rule.pattern.isEmpty()) { continue; }
        QString label = rule.pattern;
        if(rule.isRegex) { label += tr("  [regex]"); }
        if(rule.caseSensitive) { label += tr("  [Aa]"); }
        ui->patternList->addItem(label);
    }
    if(ui->patternList->count() > 0) {
        ui->patternList->setCurrentRow(0);
    }
}

PatternFilterDialog::~PatternFilterDialog()
{
}

HighlightRule PatternFilterDialog::selectedRule() const
{
    int row = ui->patternList->currentRow();
    // Map back to enabled rules
    int enabledIdx = 0;
    for(int i = 0; i < m_rules.size(); i++) {
        if(!m_rules[i].enabled || m_rules[i].pattern.isEmpty()) { continue; }
        if(enabledIdx == row) { return m_rules[i]; }
        enabledIdx++;
    }
    return HighlightRule();
}
