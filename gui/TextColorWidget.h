/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#ifndef TEXTCOLORWIDGET_H
#define TEXTCOLORWIDGET_H

#include <QWidget>
#include <QScopedPointer>

namespace Ui {
    class TextColorWidget;
}

class TextColor;

class TextColorWidget : public QWidget {
    Q_OBJECT

public:
    explicit TextColorWidget(QWidget *parent = 0);
    ~TextColorWidget();

    QString text() const;
    void setText(const QString & text) const;

    const TextColor & textColor() const;
    void setTextColor(const TextColor & tc);

signals:
    void textColorChanged();

private slots:
    void on_defaultCheckBox_clicked(bool checked);
    void currentColorChanged(const QColor & color);

private:
    void updateColorButtons();

    Ui::TextColorWidget *ui;
    QScopedPointer<TextColor> m_textColor;
};

#endif // TEXTCOLORWIDGET_H
