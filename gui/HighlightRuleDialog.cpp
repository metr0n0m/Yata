/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "HighlightRuleDialog.h"
#include "ui_HighlightRuleDialog.h"
#include "preferences/Preferences.h"
#include <QRegExp>
#include <QMessageBox>
#include <QPalette>
#include "app/YApplication.h"

static QString htmlEscape(const QString & s)
{
    QString r = s;
    r.replace(QLatin1Char('&'),  QLatin1String("&amp;"));
    r.replace(QLatin1Char('<'),  QLatin1String("&lt;"));
    r.replace(QLatin1Char('>'),  QLatin1String("&gt;"));
    r.replace(QLatin1Char('"'),  QLatin1String("&quot;"));
    return r;
}

HighlightRuleDialog::HighlightRuleDialog(QWidget * parent)
    : QDialog(parent)
    , ui(new Ui::HighlightRuleDialog)
{
    ui->setupUi(this);

    HighlightRule defaultRule;
    ui->colorWidget->setTextColor(defaultRule.color);

    connect(ui->colorWidget, SIGNAL(textColorChanged()), SLOT(updatePreview()));
    connect(ui->patternEdit, SIGNAL(textChanged(QString)), SLOT(updatePreview()));

    updatePreview();
}

HighlightRuleDialog::~HighlightRuleDialog()
{
}

void HighlightRuleDialog::setRule(const HighlightRule & rule)
{
    m_rule = rule;
    ui->patternEdit->setText(rule.pattern);
    ui->caseSensitiveCheckBox->setChecked(rule.caseSensitive);
    ui->regexCheckBox->setChecked(rule.isRegex);
    ui->colorWidget->setTextColor(rule.color);
    updatePreview();
}

HighlightRule HighlightRuleDialog::rule() const
{
    return m_rule;
}

void HighlightRuleDialog::updatePreview()
{
    const TextColor & normalColor = Preferences::instance()->normalTextColor();
    const TextColor & ruleColor   = ui->colorWidget->textColor();

    QPalette pal = ui->previewText->palette();
    pal.setColor(QPalette::Window,     normalColor.background());
    pal.setColor(QPalette::WindowText, normalColor.foreground());
    ui->previewText->setPalette(pal);

    QString pattern = ui->patternEdit->text();
    if(pattern.isEmpty()) { pattern = "pattern"; }

    QString fg  = ruleColor.foreground().name();
    QString bg  = ruleColor.background().name();
    QString highlighted = QString("<span style=\"color:%1; background-color:%2;\">%3</span>")
        .arg(fg).arg(bg).arg(htmlEscape(pattern));

    ui->previewText->setText(
        QString("normal text %1 normal text").arg(highlighted));
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
    m_rule.pattern       = pattern;
    m_rule.caseSensitive = ui->caseSensitiveCheckBox->isChecked();
    m_rule.isRegex       = ui->regexCheckBox->isChecked();
    m_rule.color         = ui->colorWidget->textColor();
    accept();
}
