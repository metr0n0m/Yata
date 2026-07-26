/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "HighlightRule.h"
#include "session/SessionCommon.h"
#include <yaml-cpp/yaml.h>

const std::string HighlightRule::PATTERN_KEY        = "pattern";
const std::string HighlightRule::IS_REGEX_KEY       = "is-regex";
const std::string HighlightRule::CASE_SENSITIVE_KEY = "case-sensitive";
const std::string HighlightRule::ENABLED_KEY        = "enabled";
const std::string HighlightRule::COLOR_KEY          = "color";

YAML::Emitter & operator<<(YAML::Emitter & out, const HighlightRule & rule)
{
    out << YAML::BeginMap;
    out << YAML::Key << HighlightRule::PATTERN_KEY        << YAML::Value << rule.pattern.toStdString();
    out << YAML::Key << HighlightRule::IS_REGEX_KEY       << YAML::Value << rule.isRegex;
    out << YAML::Key << HighlightRule::CASE_SENSITIVE_KEY << YAML::Value << rule.caseSensitive;
    out << YAML::Key << HighlightRule::ENABLED_KEY        << YAML::Value << rule.enabled;
    out << YAML::Key << HighlightRule::COLOR_KEY          << YAML::Value << rule.color;
    out << YAML::EndMap;
    return out;
}

void operator>>(const YAML::Node & in, HighlightRule & rule)
{
    rule.pattern       = QString::fromStdString(getValue<std::string>(in, HighlightRule::PATTERN_KEY));
    rule.isRegex       = getValue<bool>(in, HighlightRule::IS_REGEX_KEY);
    rule.caseSensitive = getValue<bool>(in, HighlightRule::CASE_SENSITIVE_KEY);
    rule.enabled       = getValue<bool>(in, HighlightRule::ENABLED_KEY);
    rule.color         = getValue<TextColor>(in, HighlightRule::COLOR_KEY);
}
