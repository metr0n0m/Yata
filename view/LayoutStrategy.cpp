/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#include "LayoutStrategy.h"

#include "view/TailView.h"
#include "document/YTextDocument.h"

LayoutStrategy::LayoutStrategy(TailView * tailView)
    : m_view(tailView)
{
}

LayoutStrategy::~LayoutStrategy()
{
}

void LayoutStrategy::performLayout()
{
    int textWidth = view()->viewport()->width() - view()->lineNumberAreaWidth();
    document()->layout(qMax(textWidth, 1));
}

TailView * LayoutStrategy::view()
{
    return m_view;
}

const TailView * LayoutStrategy::view() const
{
    return m_view;
}

YTextDocument * LayoutStrategy::document()
{
    return m_view->document();
}

const YTextDocument * LayoutStrategy::document() const
{
    return m_view->document();
}

