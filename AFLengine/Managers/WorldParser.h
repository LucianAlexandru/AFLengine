#pragma once

#include <string>

class WorldParser {

public:

    static WorldParser& getInstance ();
    void operator= (WorldParser const&) = delete;
    ~WorldParser ();

private:

    WorldParser ();
    WorldParser (WorldParser const&);

public:

    bool load ();
    bool save ();

    void setFile (std::string fileName);

private:

    std::string fileName;

};