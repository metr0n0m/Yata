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
#include "preferences/Preferences.h"

#include "app/YApplication.h"
#include <QLabel>

static QString htmlEscape(const QString & s)
{
    QString r = s;
    r.replace(QLatin1Char('&'),  QLatin1String("&amp;"));
    r.replace(QLatin1Char('<'),  QLatin1String("&lt;"));
    r.replace(QLatin1Char('>'),  QLatin1String("&gt;"));
    r.replace(QLatin1Char('"'),  QLatin1String("&quot;"));
    return r;
}
#include <QMessageBox>
#include <QTableWidgetItem>

// Column indices
static const int COL_PATTERN = 0;
static const int COL_CASE    = 1;
static const int COL_REGEX   = 2;
static const int COL_PREVIEW = 3;

HighlightingDialog::HighlightingDialog(QWidget * parent)
    : QDialog(parent)
    , ui(new Ui::HighlightingDialog)
{
    ui->setupUi(this);

    QTableWidget * t = ui->rulesTable;
    t->horizontalHeader()->setResizeMode(COL_PATTERN, QHeaderView::Stretch);
    t->horizontalHeader()->setResizeMode(COL_CASE,    QHeaderView::ResizeToContents);
    t->horizontalHeader()->setResizeMode(COL_REGEX,   QHeaderView::ResizeToContents);
    t->horizontalHeader()->setResizeMode(COL_PREVIEW, QHeaderView::ResizeToContents);
    t->verticalHeader()->hide();

    connect(t, SIGNAL(itemChanged(QTableWidgetItem*)),
            this, SLOT(on_rulesTable_itemChanged(QTableWidgetItem*)));
    connect(t, SIGNAL(currentCellChanged(int,int,int,int)),
            this, SLOT(on_rulesTable_currentCellChanged(int,int,int,int)));

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
    t->blockSignals(true);
    t->setRowCount(0);
    for(int i = 0; i < m_rules.size(); i++) {
        t->insertRow(i);
        addRow(i, m_rules[i]);
    }
    t->blockSignals(false);
}

void HighlightingDialog::addRow(int row, const HighlightRule & rule)
{
    QTableWidget * t = ui->rulesTable;
    t->blockSignals(true);

    // Pattern — with enabled checkbox built into the item
    QTableWidgetItem * patternItem = new QTableWidgetItem(rule.pattern);
    patternItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
    patternItem->setCheckState(rule.enabled ? Qt::Checked : Qt::Unchecked);
    t->setItem(row, COL_PATTERN, patternItem);

    // Case sensitive
    QTableWidgetItem * caseItem = new QTableWidgetItem(rule.caseSensitive ? tr("Aa") : QString());
    caseItem->setTextAlignment(Qt::AlignCenter);
    caseItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    t->setItem(row, COL_CASE, caseItem);

    // Regex
    QTableWidgetItem * regexItem = new QTableWidgetItem(rule.isRegex ? tr(".*") : QString());
    regexItem->setTextAlignment(Qt::AlignCenter);
    regexItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    t->setItem(row, COL_REGEX, regexItem);

    // Preview — QLabel with actual rule colors on top of Preferences normal background
    const TextColor & normalColor = Preferences::instance()->normalTextColor();
    QString fg  = rule.color.foreground().name();
    QString bg  = rule.color.background().name();
    QString nbg = normalColor.background().name();
    QString nfg = normalColor.foreground().name();

    QLabel * preview = new QLabel;
    QString html = QString("<span style=\"color:%1; background-color:%2;\">%3</span>"
                           "<span style=\"color:%4; background-color:%5;\"> in log</span>")
        .arg(fg).arg(bg).arg(htmlEscape(rule.pattern))
        .arg(nfg).arg(nbg);
    preview->setText(html);

    QPalette pal = preview->palette();
    pal.setColor(QPalette::Window, normalColor.background());
    preview->setPalette(pal);
    preview->setAutoFillBackground(true);
    preview->setMargin(3);
    t->setCellWidget(row, COL_PREVIEW, preview);

    t->blockSignals(false);
}

void HighlightingDialog::on_rulesTable_itemChanged(QTableWidgetItem * item)
{
    if(!item || item->column() != COL_PATTERN) { return; }
    int row = item->row();
    if(row >= 0 && row < m_rules.size()) {
        m_rules[row].enabled = (item->checkState() == Qt::Checked);
    }
}

void HighlightingDialog::on_rulesTable_currentCellChanged(
    int currentRow, int /*currentColumn*/, int /*previousRow*/, int /*previousColumn*/)
{
    Q_UNUSED(currentRow);
    updateButtons();
}

void HighlightingDialog::on_addButton_clicked()
{
    HighlightRuleDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted) {
        HighlightRule rule = dialog.rule();

        // Reject duplicates — same pattern + same case sensitivity
        for(int i = 0; i < m_rules.size(); i++) {
            if(m_rules[i].pattern == rule.pattern &&
               m_rules[i].caseSensitive == rule.caseSensitive) {
                QMessageBox::warning(this, YApplication::displayAppName(),
                    tr("A rule for \"%1\" with the same case sensitivity already exists.")
                    .arg(rule.pattern));
                return;
            }
        }

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
        HighlightRule rule = dialog.rule();

        // Reject duplicates excluding current row
        for(int i = 0; i < m_rules.size(); i++) {
            if(i == row) { continue; }
            if(m_rules[i].pattern == rule.pattern &&
               m_rules[i].caseSensitive == rule.caseSensitive) {
                QMessageBox::warning(this, YApplication::displayAppName(),
                    tr("A rule for \"%1\" with the same case sensitivity already exists.")
                    .arg(rule.pattern));
                return;
            }
        }

        m_rules[row] = rule;
        ui->rulesTable->blockSignals(true);
        addRow(row, rule);
        ui->rulesTable->blockSignals(false);
        ui->rulesTable->selectRow(row);
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
    int row = ui->rulesTable->currentRow();
    bool hasSelection = (row >= 0 && row < m_rules.size());
    ui->editButton->setEnabled(hasSelection);
    ui->deleteButton->setEnabled(hasSelection);
}
