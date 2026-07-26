/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "gui/MainWindow.h"
#include "search/SearchInfo.h"
#include "SessionLoader.h"
#include "view/TailView.h"
#include "app/YApplication.h"
#include "session/AppSession.h"
#include "session/FileSession.h"
#include "session/SearchSession.h"
#include "session/SessionIO.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>

// FNV-32 hash of the first HASH_BYTES bytes of a file.
// Fast, no dependencies, good enough to detect file replacement.
static const qint64 HASH_BYTES = 256;

static unsigned int firstLineHash(const QString & filename)
{
    QFile f(filename);
    if(!f.open(QIODevice::ReadOnly)) { return 0; }

    QByteArray data = f.read(HASH_BYTES);
    f.close();

    // FNV-1a 32-bit
    unsigned int hash = 2166136261u;
    for(int i = 0; i < data.size(); i++) {
        hash ^= (unsigned char)data[i];
        hash *= 16777619u;
    }
    return hash;
}

// Returns true if the saved session address is still valid for this file.
// Any anomaly (file replaced, truncated, rotated) returns false — open as new.
static bool isSessionAddressValid(const FileSession & fs)
{
    const QString path = QString::fromStdString(fs.path);
    QFileInfo info(path);

    // File must exist and be readable
    if(!info.exists() || !info.isFile()) { return false; }

    const qint64 currentSize = info.size();

    // Address beyond current file — file was truncated or replaced
    if(fs.address >= currentSize) { return false; }

    // Saved hash must match — detects full rewrites and log rotation
    // (skip check if hash was never saved, i.e. old session format)
    if(fs.firstLineHash != 0) {
        if(firstLineHash(path) != fs.firstLineHash) { return false; }
    }

    // File shrank relative to saved size — truncated or rotated
    if(currentSize < fs.fileSize) { return false; }

    return true;
}

void SessionLoader::readSession(MainWindow * win)
{
    SessionIO sessionIO;
    AppSession appSession;

    std::string sessionFile = nativeSessionPath();
    sessionIO.readSession(&appSession, sessionFile);

    SearchInfo::Container searchList;
    for(size_t i = 0; i < appSession.searchCount(); i++) {
        const SearchSession & searchSession = appSession.searchAt(i);
        searchList.push_back(SearchCriteria(
            QString::fromStdString(searchSession.expression),
            searchSession.isCaseSensitive,
            searchSession.isRegex));
    }

    SearchInfo::instance().populateSearchList(&searchList);

    for(size_t i = 0; i < appSession.fileCount(); i++) {
        const FileSession & fileSession = appSession.fileAt(i);
        win->addFile(QString::fromStdString(fileSession.path));

        // Restore scroll position and follow-tail only if the file
        // passes all integrity checks — otherwise open as new.
        TailView * view = win->tailViewAt(i);
        if(view) {
            view->setFollowTail(fileSession.followTail);
            if(!fileSession.followTail && isSessionAddressValid(fileSession)) {
                view->scrollToAddress(fileSession.address);
            }
        }
    }

    win->setCurrentFileIndex(appSession.currentIndex());
    win->setCurrentOpenDirectory(QString::fromStdString(appSession.lastOpenDirectory()));

    QByteArray qGeometry;
    const AppSession::GContainer & geometry = appSession.geometry();
    for(AppSession::GContainer::const_iterator itr = geometry.begin();
        itr != geometry.end(); ++itr) {
        qGeometry.push_back(*itr);
    }

    win->restoreGeometry(qGeometry);
}

void SessionLoader::writeSession(MainWindow * win)
{
    SessionIO sessionIO;
    AppSession appSession;
    appSession.setCurrentIndex(win->currentFileIndex());

    const SearchInfo & searchInfo = SearchInfo::instance();

    for(SearchInfo::const_iterator sc = searchInfo.begin(); sc != searchInfo.end(); ++sc) {
        SearchSession searchSession(
            sc->expression.toStdString(), sc->isRegex, sc->isCaseSensitive);
        appSession.addSearch(searchSession);
    }

    std::vector<FileSession> sessions;
    win->fileSessions(&sessions);

    // Compute firstLineHash for each file before saving
    for(std::vector<FileSession>::iterator itr = sessions.begin();
        itr != sessions.end(); ++itr) {
        itr->firstLineHash = firstLineHash(QString::fromStdString(itr->path));
    }

    for(std::vector<FileSession>::const_iterator itr = sessions.begin();
        itr != sessions.end(); ++itr) {
        appSession.addFile(*itr);
    }

    QByteArray qGeometry = win->saveGeometry();

    AppSession::GContainer geometry;
    for(int i = 0; i < qGeometry.size(); i++) {
        geometry.push_back(qGeometry[i]);
    }

    appSession.setLastOpenDirectory(win->currentOpenDirectory().toStdString());
    appSession.setGeometry(geometry);

    std::string sessionFile = nativeSessionPath();
    sessionIO.writeSession(appSession, sessionFile);
}

std::string SessionLoader::nativeSessionPath()
{
    QString sessionFile = YApplication::sessionFilePath();
    sessionFile = QDir::toNativeSeparators(sessionFile);
    return sessionFile.toStdString();
}
