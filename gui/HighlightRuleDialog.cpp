/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "HighlightRuleDialog.h"
#include "ui_HighlightRuleDialog.h"
#include <QRegExp>
#include <QMessageBox>
#include "app/YApplication.h"

HighlightRuleDialog::HighlightRuleDialog(QWidget * parent)
    : QDialog(parent)
    , ui(new Ui::HighlightRuleDialog)
{
    ui->setupUi(this);
    ui->colorWidget->setText(tr("Color:"));
    HighlightRule defaultRule;
    ui->colorWidget->setTextColor(defaultRule.color);
}

HighlightRuleDialog::~HighlightRuleDialog()
{
}

void HighlightRuleDialog::setRule(const HighlightRule & rule)
{
    m_rule = rule;
    ui->patternEdit->setText(rule.pattern);
    ui->regexCheckBox->setChecked(rule.isRegex);
    ui->colorWidget->setTextColor(rule.color);
}

HighlightRule HighlightRuleDialog::rule() const
{
    return m_rule;
}

void HighlightRuleDialog::on_buttonBox_accepted()
{
    QString pattern = ui->patternEdit->text().trimmed();
    if(pattern.isEmpty()) {
        QMessageBox::warning(this, YApplication::displayAppName(),
            tr("Pattern cannot be empty."));
        return;
    }
    if(ui->regexCheckBox->isChecked()) {
        QRegExp regex(pattern, Qt::CaseSensitive, QRegExp::RegExp2);
        if(!regex.isValid()) {
            QMessageBox::warning(this, YApplication::displayAppName(),
                tr("Invalid regular expression: ") + regex.errorString());
            return;
        }
    }
    m_rule.pattern  = pattern;
    m_rule.isRegex  = ui->regexCheckBox->isChecked();
    m_rule.color    = ui->colorWidget->textColor();
    accept();
}
