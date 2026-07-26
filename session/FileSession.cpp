/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2026 James Smith & metr0n0m
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "FileSession.h"
#include "SessionCommon.h"
#include <yaml-cpp/yaml.h>

const std::string FileSession::PATH_KEY           = "path";
const std::string FileSession::ADDRESS_KEY         = "address";
const std::string FileSession::FOLLOW_TAIL_KEY     = "follow-tail";
const std::string FileSession::FILE_SIZE_KEY       = "file-size";
const std::string FileSession::FIRST_LINE_HASH_KEY = "first-line-hash";

YAML::Emitter & operator<<(YAML::Emitter & out, const FileSession & session)
{
    out << YAML::BeginMap;
    out << YAML::Key << FileSession::PATH_KEY           << YAML::Value << session.path;
    out << YAML::Key << FileSession::ADDRESS_KEY         << YAML::Value << session.address;
    out << YAML::Key << FileSession::FOLLOW_TAIL_KEY     << YAML::Value << session.followTail;
    out << YAML::Key << FileSession::FILE_SIZE_KEY       << YAML::Value << session.fileSize;
    out << YAML::Key << FileSession::FIRST_LINE_HASH_KEY << YAML::Value << (int)session.firstLineHash;
    out << YAML::EndMap;
    return out;
}

void operator>>(const YAML::Node & in, FileSession & session)
{
    session.path          = getValue<std::string>(in, FileSession::PATH_KEY);
    session.address       = getValue<long long>(in, FileSession::ADDRESS_KEY);
    session.followTail    = getValue<bool>(in, FileSession::FOLLOW_TAIL_KEY);
    session.fileSize      = getValue<long long>(in, FileSession::FILE_SIZE_KEY);
    session.firstLineHash = (unsigned int)getValue<int>(in, FileSession::FIRST_LINE_HASH_KEY);
}
