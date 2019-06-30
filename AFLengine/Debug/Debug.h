#pragma once

#include <LogStream.h>

#include <string>
#include <list>

class Debug {

    friend class Engine;

public:

    static Debug& getInstance ();
    void operator= (Debug const&) = delete;
    ~Debug ();

private:

    Debug ();
    Debug (Debug const&);

public:

    enum Level {
        Level_Debug     = 1,
        Level_Info      = 2,
        Level_Warning   = 4,
        Level_Error     = 8,
    };

    void log (Level level, std::string message, char* sourceFile, int line);

    void addLogStream (LogStreamRef logStream);

private:

    std::list<LogStreamRef> logStreams;

};

#define Debug_Log(level, ...)                                       \
{                                                                   \
    char message[128];                                              \
    sprintf_s(message, sizeof(message), __VA_ARGS__);               \
    Debug::getInstance().log(level, message, __FILE__, __LINE__);   \
}