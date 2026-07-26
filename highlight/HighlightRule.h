/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef HIGHLIGHTRULE_H
#define HIGHLIGHTRULE_H

#include "preferences/TextColor.h"
#include <QString>

namespace YAML {
    class Emitter;
    class Node;
}

struct HighlightRule {
    QString pattern;
    bool isRegex;
    bool caseSensitive;
    bool enabled;
    TextColor color;

    static const std::string PATTERN_KEY;
    static const std::string IS_REGEX_KEY;
    static const std::string CASE_SENSITIVE_KEY;
    static const std::string ENABLED_KEY;
    static const std::string COLOR_KEY;

    HighlightRule()
        : isRegex(false)
        , caseSensitive(false)
        , enabled(true)
        , color(QPalette::Text, QPalette::Base)
    {
    }
};

YAML::Emitter & operator<<(YAML::Emitter & out, const HighlightRule & rule);
void operator>>(const YAML::Node & in, HighlightRule & rule);

#endif // HIGHLIGHTRULE_H
