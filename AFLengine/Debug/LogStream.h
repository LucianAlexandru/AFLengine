#pragma once

#include <memory>
#include <string>

class Debug;

class LogStream {

    friend class Debug;

public:

    virtual void log (std::string message) = 0;

    void setLevels (unsigned int levels);

protected:

    unsigned int levels;

};

typedef std::shared_ptr<LogStream> LogStreamRef;