#pragma once

#include <LogStream.h>

class ConsoleLogStream : public LogStream {

public:

    ConsoleLogStream ();

    void log (std::string message);

};