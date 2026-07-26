/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "HighlightRules.h"
#include "app/YApplication.h"
#include "session/SessionCommon.h"
#include <yaml-cpp/yaml.h>
#include <fstream>

static const std::string RULES_KEY = "rules";

HighlightRules::HighlightRules()
{
}

HighlightRules & HighlightRules::instance()
{
    static HighlightRules s_instance;
    return s_instance;
}

void HighlightRules::read()
{
    m_rules.clear();
    try {
        std::ifstream in(YApplication::highlightsFilePath().toStdString().c_str());
        if(!in) { return; }
        YAML::Parser parser(in);
        YAML::Node document;
        if(parser.GetNextDocument(document)) {
            if(const YAML::Node * rules = document.FindValue(RULES_KEY)) {
                for(YAML::Iterator itr = rules->begin(); itr != rules->end(); ++itr) {
                    HighlightRule rule;
                    *itr >> rule;
                    if(!rule.pattern.isEmpty()) {
                        m_rules.push_back(rule);
                    }
                }
            }
        }
    } catch(YAML::Exception &) {
        // Corrupt file — start with empty rules
    }
}

void HighlightRules::write()
{
    YAML::Emitter emitter;
    emitter << YAML::BeginMap;
    emitter << YAML::Key << RULES_KEY << YAML::Value;
    emitter << YAML::BeginSeq;
    for(int i = 0; i < m_rules.size(); i++) {
        emitter << m_rules[i];
    }
    emitter << YAML::EndSeq;
    emitter << YAML::EndMap;

    std::ofstream out(YApplication::highlightsFilePath().toStdString().c_str());
    out << emitter.c_str();
}

const QList<HighlightRule> & HighlightRules::rules() const
{
    return m_rules;
}

void HighlightRules::setRules(const QList<HighlightRule> & rules)
{
    m_rules = rules;
    write();
    emit rulesChanged();
}
