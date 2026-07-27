/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "PartialLayout.h"

#include "search/DocumentSearch.h"
#include "search/FileSearch.h"
#include "fileio/FileBlockReader.h"
#include "view/TailView.h"
#include "filter/FilterState.h"
#include "document/YTextDocument.h"

#include <QScrollBar>
#include <QTextBlock>
#include <QTextLayout>

const qint64 APPROXIMATE_CHARS_PER_LINE = 20;

PartialLayout::PartialLayout(TailView * tailView)
    : LayoutStrategy(tailView)
    , m_topScreenLine(0)
    , m_cachedLineNumberAddress(-1)
    , m_cachedLineNumber(1)
    , m_bottomDocument(new YTextDocument())
{
}

bool PartialLayout::onFileChanged(QString * error)
{
    m_blockReader.reset(new FileBlockReader(view()->filename()));

    const FilterState & fs = view()->filterState();
    if(fs.isActive()) {
        m_topScreenLine = 0;
        updateScrollBars();
        bool success = false;
        const QVector<qint64> & matches = fs.matchAddresses();
        if(matches.isEmpty()) {
            // No matches — show empty document
            QString empty;
            std::vector<qint64> emptyAddrs;
            document()->setText(empty, emptyAddrs);
            performLayout();
            success = true;
        } else if(view()->followTail()) {
            // Show last N matching lines
            int n = view()->numLinesOnScreen();
            int startIdx = qMax(0, matches.size() - n);
            success = updateDocumentFromFilter(startIdx);
            QScrollBar * vScrollBar = view()->verticalScrollBar();
            vScrollBar->setSliderPosition(vScrollBar->maximum());
        } else {
            success = updateDocumentFromFilter(0);
        }
        if(!success) {
            *error = m_blockReader->errorString();
            return false;
        }
        view()->viewport()->update();
        return true;
    }

    updateBottomDocument();
    bool success = false;
    if(view()->followTail()) {
        success = updateView(bottomScreenPosition());
        QScrollBar * vScrollBar = view()->verticalScrollBar();
        vScrollBar->setSliderPosition(vScrollBar->maximum());
    } else {
        success = updateView(topOfScreen());
    }
    if(!success) {
        *error = m_blockReader->errorString();
        return false;
    }
    return true;
}

void PartialLayout::resizeEvent()
{
    updateBottomDocument();
    updateView(topOfScreen());
}

int PartialLayout::topScreenLine() const
{
    return m_topScreenLine;
}

void PartialLayout::scrollTo(int newTopLine)
{
    int lineChange = newTopLine - topScreenLine();
    scrollBy(lineChange);
}

void PartialLayout::scrollToAddress(qint64 address)
{
    m_topScreenLine = 0;
    updateDocument(address, 0);
    updateScrollBars();
    view()->viewport()->update();
}

bool PartialLayout::searchFile(bool isForward)
{
    bool matchFound = false;
    FileSearch fileSearch(view()->filename());
    fileSearch.setCursor(document()->fileCursor());
    DocumentSearch * docSearch = view()->documentSearch();
    fileSearch.setSearchCriteria(
        docSearch->lastSearchString(),
        docSearch->searchWasRegex(),
        docSearch->searchWasCaseSensitive());
    if(fileSearch.searchFile(isForward, true)) {
        matchFound = true;
        YFileCursor cursor(fileSearch.cursor());
        document()->select(cursor);

        bool at_bottom = false;
        updateView(document()->fileCursor().lineAddress(), &at_bottom);
        if(!at_bottom) {
            int line_change = -(view()->numLinesOnScreen()/2);
            scrollBy(line_change);
        }
    }

    return matchFound;
}

bool PartialLayout::wrapAroundForDocumentSearch() const
{
    return false;
}

int PartialLayout::firstVisibleLineNumber() const
{
    if(!m_blockReader) { return 1; }
    const qint64 topAddr = topOfScreen();

    // Cache by address — only recount when position changed
    if(topAddr == m_cachedLineNumberAddress) {
        return m_cachedLineNumber + 1;
    }

    // Count newlines from file start to topAddr
    int lineNum = 0;
    qint64 pos = 0;
    while(pos < topAddr && pos < m_blockReader->size()) {
        qint64 next = m_blockReader->getStartPosition(pos, 1);
        if(next <= pos) { break; }
        lineNum++;
        pos = next;
    }
    m_cachedLineNumberAddress = topAddr;
    m_cachedLineNumber = lineNum;
    return lineNum + 1;
}

void PartialLayout::updateAfterKeyPress()
{
    updateView();
}

void PartialLayout::vScrollBarAction(int action)
{
    int line_change = 0;
    int page_step = view()->numLinesOnScreen() - PAGE_STEP_OVERLAP;

    switch(action) {
    case QAbstractSlider::SliderSingleStepAdd:
        line_change = 1;
        break;
    case QAbstractSlider::SliderSingleStepSub:
        line_change = -1;
        break;
    case QAbstractSlider::SliderPageStepAdd:
        line_change = page_step;
        break;
    case QAbstractSlider::SliderPageStepSub:
        line_change = -page_step;
        break;
    default:
        break;
    }

    if(line_change) {
        scrollBy(line_change);
    } else if(view()->filterState().isActive()) {
        // Scrollbar moved directly — update from filter index
        int idx = view()->verticalScrollBar()->sliderPosition();
        const QVector<qint64> & matches = view()->filterState().matchAddresses();
        idx = qMax(0, qMin(idx, matches.size() - 1));
        updateDocumentFromFilter(idx);
    } else {
        updateView();
    }
}

void PartialLayout::updateScrollBars()
{
    int maxLines;
    if(view()->filterState().isActive()) {
        maxLines = qMax(0, view()->filterState().matchAddresses().size() - view()->numLinesOnScreen());
    } else {
        const qint64 bottom_screen_pos = bottomScreenPosition();
        maxLines = static_cast<int>(bottom_screen_pos / APPROXIMATE_CHARS_PER_LINE);
    }
    view()->updateScrollBars(maxLines);
}

int PartialLayout::filterMatchCount() const
{
    return view()->filterState().matchAddresses().size();
}

bool PartialLayout::updateView(qint64 new_line_address /*=-1*/, bool * at_bottom /*=0*/)
{
    int bottomScreenTopBlockLine = 0;
    const qint64 bottom_screen_pos = bottomScreenPosition(&bottomScreenTopBlockLine);

    QScrollBar * verticalScrollBar = view()->verticalScrollBar();
    qint64 file_pos = 0;
    if(new_line_address != -1) {
        // Scroll bar did not move
        file_pos = new_line_address;
    } else {
        // Scroll bar moved
        m_topScreenLine = 0;
        if(verticalScrollBar->sliderPosition() >= verticalScrollBar->maximum()) {
            file_pos = bottom_screen_pos;
        } else {
            file_pos = static_cast<qint64>(verticalScrollBar->sliderPosition()) * APPROXIMATE_CHARS_PER_LINE;
        }
    }

    file_pos = m_blockReader->getStartPosition(file_pos, 0);

    // Don't change the line further if we're at the bottom
    if(file_pos >= bottom_screen_pos) {
        file_pos = bottom_screen_pos;
        m_topScreenLine = bottomScreenTopBlockLine;
        if(at_bottom) { *at_bottom = true; }
        verticalScrollBar->setSliderPosition(verticalScrollBar->maximum());
    } else {
        if(at_bottom) { *at_bottom = false; }
    }

    bool success = updateDocument(file_pos, 0);
    if(success) { view()->viewport()->update(); }

    return success;
}

bool PartialLayout::scrollBy(int line_change)
{
    QScrollBar * verticalScrollBar = view()->verticalScrollBar();

    // Filter mode: scroll by match index
    if(view()->filterState().isActive()) {
        if(line_change != 0) {
            int idx = verticalScrollBar->sliderPosition() + line_change;
            const QVector<qint64> & matches = view()->filterState().matchAddresses();
            idx = qMax(0, qMin(idx, matches.size() - 1));
            verticalScrollBar->setSliderPosition(idx);
            updateDocumentFromFilter(idx);
        }
        view()->viewport()->update();
        return true;
    }

    if(line_change != 0) {
        if(line_change < 0) {
            if (!scrollUp(-line_change)) { return false; }
        } else {
            if (!scrollDown(line_change)) { return false; }
        }
        verticalScrollBar->setSliderPosition(topOfScreen() / APPROXIMATE_CHARS_PER_LINE + m_topScreenLine);
    }

    view()->viewport()->update();
    return true;
}

bool PartialLayout::scrollUp(int line_change)
{
    if (m_topScreenLine >= line_change) {
        m_topScreenLine -= line_change;
        return true;
    }

    line_change -= m_topScreenLine;
    if(!updateDocument(topOfScreen(), -line_change)) {
        m_topScreenLine = 0;
        return false;
    }
    QTextBlock firstBlock = document()->begin();
    qint64 file_pos = document()->blockAddress(firstBlock);
    if(file_pos == 0) {
        m_topScreenLine = 0;
    } else {
        int oldTopBlock = document()->blockLayoutLines().at(line_change);
        m_topScreenLine = oldTopBlock - line_change;
    }

    return true;
}

bool PartialLayout::scrollDown(int line_change)
{
    QTextBlock firstBlock = document()->begin();
    if(m_topScreenLine + line_change < firstBlock.layout()->lineCount()) {
        m_topScreenLine += line_change;
        keepToLowerBound(firstBlock, &m_topScreenLine);
        return true;
    }
    int topLineOfNewBlock = 0;
    QTextBlock newTopBlock = document()->blockLayoutLines().findContainingBlock(m_topScreenLine + line_change, &topLineOfNewBlock);
    if(!newTopBlock.isValid()) { return false; }

    m_topScreenLine += line_change - topLineOfNewBlock;
    qint64 file_pos = keepToLowerBound(newTopBlock, &m_topScreenLine);
    return updateDocument(file_pos, 0);
}

qint64 PartialLayout::keepToLowerBound(const QTextBlock & block, int * blockLine)
{
    qint64 file_pos = document()->blockAddress(block);

    int bottomScreenTopBlockLine = 0;
    const qint64 bottom_screen_pos = bottomScreenPosition(&bottomScreenTopBlockLine);

    if(file_pos >= bottom_screen_pos) {
        file_pos = bottom_screen_pos;
        if(*blockLine > bottomScreenTopBlockLine) {
            *blockLine = bottomScreenTopBlockLine;
        }
    }

    return file_pos;
}

bool PartialLayout::updateDocumentFromFilter(int startMatchIndex)
{
    const QVector<qint64> & matches = view()->filterState().matchAddresses();
    const int n = view()->numLinesOnScreen();
    const int end = qMin(startMatchIndex + n, matches.size());

    QString data;
    std::vector<qint64> lineAddresses;

    for(int i = startMatchIndex; i < end; i++) {
        QString line;
        std::vector<qint64> lineAddr;
        if(!m_blockReader->readChunk(&line, &lineAddr, matches[i], 0, 1)) { return false; }
        if(!lineAddr.empty()) {
            lineAddresses.push_back(lineAddr.front());
        }
        data += line;
    }

    document()->setText(data, lineAddresses);
    performLayout();
    return true;
}

bool PartialLayout::updateDocument(qint64 start_pos, qint64 lines_after_start)
{
    QString data;
    std::vector<qint64> lineAddresses;
    qint64 num_lines = view()->numLinesOnScreen() + 1;
    bool success = m_blockReader->readChunk(&data, &lineAddresses, start_pos, lines_after_start, num_lines);
    document()->setText(data, lineAddresses);
    performLayout();
    return success;
}

qint64 PartialLayout::topOfScreen() const
{
    const BlockDataVector<qint64> & lineAddresses = document()->lineAddresses();
    return lineAddresses.empty() ? 0 : lineAddresses.at(0);
}

void PartialLayout::updateBottomDocument()
{
    FileBlockReader bottomReader(view()->filename());
    QString data;
    std::vector<qint64> lineAddresses;
    const int numLines = view()->numLinesOnScreen();
    bottomReader.readChunk(
        &data, &lineAddresses, bottomReader.size(), -numLines, numLines);
    m_bottomDocument->setText(data, lineAddresses);
    m_bottomDocument->markDirty();
    m_bottomDocument->layout(view()->viewport()->width());
    updateScrollBars();
}

qint64 PartialLayout::bottomScreenPosition(int * blockLine /*=0*/) const
{
    int lines = 0;
    qint64 line_address = 0;

    QTextBlock block = m_bottomDocument->lastBlock();
    const int linesOnScreen = view()->numLinesOnScreen();
    while(block.isValid()) {
        lines += block.layout()->lineCount();
        if(lines >= linesOnScreen) {
            line_address = m_bottomDocument->blockAddress(block);
            break;
        }
        block = block.previous();
    }

    if(blockLine) {
        *blockLine = lines - linesOnScreen;
    }

    // In case YTextDocument::blockAddress() returns -1 (this could happen if resizing the TailView widget hasn't
    // occurred yet), just return the last character's address so follow tail on start up can happen correctly.
    if(line_address < 0) {
        line_address = m_blockReader->size();
    }

    return line_address;
}
