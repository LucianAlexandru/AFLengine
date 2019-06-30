#pragma once

#include <LogStream.h>

#include <string>
#include <fstream>

class FileLogStream : public LogStream {

public:

    FileLogStream (std::string fileName, bool showDebug);
    ~FileLogStream ();

    void log (std::string message);

private:

    std::ofstream logFile;

};