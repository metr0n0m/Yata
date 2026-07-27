/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "FilterDialog.h"
#include "ui_FilterDialog.h"

FilterDialog::FilterDialog(QWidget * parent)
    : QDialog(parent)
    , ui(new Ui::FilterDialog)
{
    ui->setupUi(this);
}

FilterDialog::~FilterDialog()
{
}

void FilterDialog::setKeyword(const QString & keyword)
{
    ui->keywordEdit->setText(keyword);
}

QString FilterDialog::keyword() const
{
    return ui->keywordEdit->text().trimmed();
}

bool FilterDialog::isRegex() const
{
    return ui->regexCheckBox->isChecked();
}

bool FilterDialog::isCaseSensitive() const
{
    return ui->caseSensitiveCheckBox->isChecked();
}
