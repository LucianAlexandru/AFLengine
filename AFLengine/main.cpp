#include <gl/glut.h>

#include <Engine.h>
#include <WorldParser.h>

#include <Debug.h>
#include <ConsoleLogStream.h>
#include <FileLogStream.h>

#include <iostream>
#include <boost/program_options.hpp>

int main (int argc, char** argv) {

    glutInit (&argc, argv);

    Debug& debug = Debug::getInstance ();
    debug.addLogStream (LogStreamRef (new ConsoleLogStream ()));

    int windowWidth, windowHeight;
    std::string fileName;

    try
    {
        using namespace boost::program_options;

        options_description desc{ "Options" };
        desc.add_options ()
            ("help,h", "Help screen")
            ("file,f", value<std::string> ()->required (), "World description file")
            ("log,l", value<std::string> (), "Log file")
            ("debug,d", value<bool> ()->default_value (false), "Show debug messages in log file")
            ("width", value<int> ()->default_value (1280), "Window width")
            ("height", value<int> ()->default_value (720), "Window height");

        variables_map variableMap;
        store (parse_command_line (argc, argv, desc), variableMap);

        if (variableMap.count ("help")) {
            std::stringstream descriptionStringStream;
            descriptionStringStream << desc;
            std::cout << descriptionStringStream.str ();
            return 0;
        }

        notify (variableMap);

        if (variableMap.count ("file"))
            fileName = variableMap["file"].as<std::string> ();

        if (variableMap.count ("log"))
            debug.addLogStream (LogStreamRef (new FileLogStream (variableMap["log"].as<std::string> (), variableMap["debug"].as<bool> ())));

        if (variableMap.count ("width"))
            windowWidth = variableMap["width"].as<int> ();
        if (variableMap.count ("height"))
            windowHeight = variableMap["height"].as<int> ();
    }
    catch (const boost::program_options::error &ex)
    {
        Debug_Log (Debug::Level_Error, ex.what ());
        return -1;
    }

    glutInitWindowSize (windowWidth, windowHeight);
    glutInitWindowPosition (400, 50);

    glutCreateWindow (argv[0]);
    Engine::init ();
    
    WorldParser& parser = WorldParser::getInstance();
    parser.setFile (fileName);
    
    if (parser.load () == false)
        return 0;
    //parser.save ();

    glutMainLoop ();

    return 0;
}