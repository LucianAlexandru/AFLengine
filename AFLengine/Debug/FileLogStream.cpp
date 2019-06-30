#include <FileLogStream.h>
#include <Debug.h>

FileLogStream::FileLogStream (std::string fileName, bool showDebug) {

    levels = Debug::Level_Info
           | Debug::Level_Warning
           | Debug::Level_Error;

    if (showDebug)
        levels |= Debug::Level_Debug;

    logFile.open (fileName.c_str (), std::ios::app);
    if (!logFile.good ()) {
        Debug_Log (Debug::Level_Error, "Log file error: %s", fileName.c_str());
    }

}

FileLogStream::~FileLogStream () {
    logFile.close ();
}

void FileLogStream::log (std::string message) {
    logFile << message.c_str();
    logFile.flush ();
}