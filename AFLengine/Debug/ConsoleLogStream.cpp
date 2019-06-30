#include <ConsoleLogStream.h>
#include <Debug.h>

#include <iostream>

ConsoleLogStream::ConsoleLogStream () {
    levels = Debug::Level_Error;
}

void ConsoleLogStream::log (std::string message) {
    std::cout << message;
}