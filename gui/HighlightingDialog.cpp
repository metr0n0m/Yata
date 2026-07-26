/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "HighlightingDialog.h"
#include "ui_HighlightingDialog.h"
#include "HighlightRuleDialog.h"
#include "highlight/HighlightRules.h"

#include <QCheckBox>
#include <QLabel>
#include <QTableWidgetItem>

// Column indices
static const int COL_ENABLED = 0;
static const int COL_PATTERN = 1;
static const int COL_REGEX   = 2;
static const int COL_PREVIEW = 3;

HighlightingDialog::HighlightingDialog(QWidget * parent)
    : QDialog(parent)
    , ui(new Ui::HighlightingDialog)
{
    ui->setupUi(this);

    QTableWidget * t = ui->rulesTable;
    t->horizontalHeader()->setStretchLastSection(true);
    t->horizontalHeader()->setResizeMode(COL_PATTERN, QHeaderView::Stretch);
    t->horizontalHeader()->setResizeMode(COL_ENABLED, QHeaderView::ResizeToContents);
    t->horizontalHeader()->setResizeMode(COL_REGEX,   QHeaderView::ResizeToContents);
    t->verticalHeader()->hide();

    m_rules = HighlightRules::instance().rules();
    populateTable();
    updateButtons();
}

HighlightingDialog::~HighlightingDialog()
{
}

void HighlightingDialog::populateTable()
{
    QTableWidget * t = ui->rulesTable;
    // Block cellChanged during population
    disconnect(t, SIGNAL(cellChanged(int,int)), this, SLOT(on_rulesTable_cellChanged(int,int)));
    t->setRowCount(0);
    for(int i = 0; i < m_rules.size(); i++) {
        t->insertRow(i);
        addRow(i, m_rules[i]);
    }
    connect(t, SIGNAL(cellChanged(int,int)), this, SLOT(on_rulesTable_cellChanged(int,int)));
}

void HighlightingDialog::addRow(int row, const HighlightRule & rule)
{
    QTableWidget * t = ui->rulesTable;

    // Enabled checkbox centered in cell
    QWidget * checkWidget = new QWidget;
    QHBoxLayout * checkLayout = new QHBoxLayout(checkWidget);
    QCheckBox * checkBox = new QCheckBox;
    checkBox->setChecked(rule.enabled);
    checkLayout->addWidget(checkBox);
    checkLayout->setAlignment(Qt::AlignCenter);
    checkLayout->setContentsMargins(0, 0, 0, 0);
    t->setCellWidget(row, COL_ENABLED, checkWidget);
    connect(checkBox, SIGNAL(toggled(bool)), this, SLOT(on_rulesTable_cellChanged(int,int)));
    // Store row index in checkbox for slot
    checkBox->setProperty("row", row);

    // Pattern
    t->setItem(row, COL_PATTERN, new QTableWidgetItem(rule.pattern));

    // Regex
    QTableWidgetItem * regexItem = new QTableWidgetItem(rule.isRegex ? tr("yes") : QString());
    regexItem->setTextAlignment(Qt::AlignCenter);
    t->setItem(row, COL_REGEX, regexItem);

    // Preview — QLabel with actual colors
    QLabel * preview = new QLabel(rule.pattern);
    QPalette pal = preview->palette();
    pal.setColor(QPalette::WindowText, rule.color.foreground());
    pal.setColor(QPalette::Window, rule.color.background());
    preview->setPalette(pal);
    preview->setAutoFillBackground(true);
    preview->setMargin(2);
    t->setCellWidget(row, COL_PREVIEW, preview);
}

void HighlightingDialog::on_rulesTable_cellChanged(int row, int /*column*/)
{
    // Sync enabled checkbox state back to m_rules
    QWidget * sender_obj = qobject_cast<QWidget*>(sender());
    if(!sender_obj) { return; }
    QCheckBox * cb = qobject_cast<QCheckBox*>(sender_obj);
    if(!cb) { return; }
    int ruleRow = cb->property("row").toInt();
    if(ruleRow >= 0 && ruleRow < m_rules.size()) {
        m_rules[ruleRow].enabled = cb->isChecked();
    }
    Q_UNUSED(row);
}

void HighlightingDialog::on_addButton_clicked()
{
    HighlightRuleDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted) {
        HighlightRule rule = dialog.rule();
        m_rules.append(rule);
        int row = ui->rulesTable->rowCount();
        ui->rulesTable->insertRow(row);
        addRow(row, rule);
        ui->rulesTable->selectRow(row);
        updateButtons();
    }
}

void HighlightingDialog::on_editButton_clicked()
{
    int row = ui->rulesTable->currentRow();
    if(row < 0 || row >= m_rules.size()) { return; }

    HighlightRuleDialog dialog(this);
    dialog.setRule(m_rules[row]);
    if(dialog.exec() == QDialog::Accepted) {
        m_rules[row] = dialog.rule();
        // Rebuild only this row
        disconnect(ui->rulesTable, SIGNAL(cellChanged(int,int)),
                   this, SLOT(on_rulesTable_cellChanged(int,int)));
        addRow(row, m_rules[row]);
        connect(ui->rulesTable, SIGNAL(cellChanged(int,int)),
                this, SLOT(on_rulesTable_cellChanged(int,int)));
    }
}

void HighlightingDialog::on_deleteButton_clicked()
{
    int row = ui->rulesTable->currentRow();
    if(row < 0 || row >= m_rules.size()) { return; }
    m_rules.removeAt(row);
    ui->rulesTable->removeRow(row);
    updateButtons();
}

void HighlightingDialog::on_buttonBox_accepted()
{
    HighlightRules::instance().setRules(m_rules);
    accept();
}

void HighlightingDialog::updateButtons()
{
    bool hasSelection = ui->rulesTable->currentRow() >= 0;
    bool hasRows = ui->rulesTable->rowCount() > 0;
    ui->editButton->setEnabled(hasSelection);
    ui->deleteButton->setEnabled(hasSelection && hasRows);
}
