/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QScopedPointer>
#include <QString>
#include <QTextStream>
#include "MainWindow.h"
#include "SessionLoader.h"
#include "YApplication.h"
#include "preferences/Preferences.h"
#include "session/SessionCommon.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/yata.png"));

    Preferences * prefs = Preferences::instance();
    ParsingStatus::Enum prefStatus = prefs->read();
    QScopedPointer<MainWindow> win(new MainWindow);

    if(prefStatus == ParsingStatus::Error) {
        const ParsingError * err = prefs->parsingError();
        QString msg;
        QTextStream(&msg)
            << QObject::tr("Could not read preferences file:\n")
            << QString::fromStdString(err ? err->message : "unknown error")
            << QObject::tr("\n\nDefault preferences will be used.");
        QMessageBox::warning(win.data(), YApplication::displayAppName(), msg);
    }

    QDir dir;
    dir.mkpath(YApplication::settingsPath());

    SessionLoader::readSession(win.data());

    if(argc > 1) {
        for(int i = 1; i < argc; i++) {
            win->addFile(argv[i]);
        }
        if(win->fileCount() > 0) {
            win->setCurrentFileIndex(win->fileCount() - 1);
        }
    }

    win->show();
    return app.exec();
}
