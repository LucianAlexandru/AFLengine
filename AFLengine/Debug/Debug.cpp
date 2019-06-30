#include <Debug.h>

#include "boost/date_time/posix_time/posix_time.hpp"

Debug& Debug::getInstance () {
    static Debug instance;
    return instance;
}

Debug::Debug () {
    logStreams.clear ();
}

Debug::~Debug () {
    logStreams.clear ();
}

void Debug::log (Level level, std::string message, char* sourceFile, int line) {

    std::stringstream messageStringStream;

    boost::posix_time::time_facet* facet = new boost::posix_time::time_facet ();
    facet->format ("%H:%M:%S.%f\t");
    messageStringStream.imbue (std::locale (std::locale::classic (), facet));

    boost::posix_time::ptime currentTime = boost::posix_time::microsec_clock::local_time ();
    messageStringStream << currentTime;

    switch (level) {

        case Level_Debug:
            messageStringStream <<  "[ Debug ]";
            break;

        case Level_Info:
            messageStringStream <<  "[  Info ]";
            break;

        case Level_Warning:
            messageStringStream <<  "[Warning]";
            break;

        case Level_Error:
            messageStringStream <<  "[ Error ]";
            break;

        default:
            return;
    }

    char buffer[100];
    sprintf_s (buffer, sizeof (buffer), "\t[%-25s - %3d]\t", sourceFile, line);

    messageStringStream << buffer << message << "\n";

    std::string messageString = messageStringStream.str ();

    for (auto logStream : logStreams) {
        
        if (logStream->levels & level) {
            logStream->log (messageString);
        }

    }

}

void Debug::addLogStream (LogStreamRef logStream) {
    logStreams.push_back (logStream);
}