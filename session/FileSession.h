#ifndef SESSION_H
#define SESSION_H

#include <string>

namespace YAML {
    class Emitter;
    class Node;
}

struct FileSession {
    std::string path;
    long long address;
    bool followTail;
    long long fileSize;
    unsigned int firstLineHash;

    static const std::string PATH_KEY;
    static const std::string ADDRESS_KEY;
    static const std::string FOLLOW_TAIL_KEY;
    static const std::string FILE_SIZE_KEY;
    static const std::string FIRST_LINE_HASH_KEY;

    FileSession(const std::string & p = std::string(), long long a = 0, bool f = false)
        : path(p)
        , address(a)
        , followTail(f)
        , fileSize(0)
        , firstLineHash(0)
    {
    }
};

YAML::Emitter & operator<<(YAML::Emitter & out, const FileSession & session);
void operator>>(const YAML::Node & in, FileSession & session);
#endif
