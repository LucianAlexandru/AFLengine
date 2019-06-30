#pragma once

class Engine {

public:

    static void init ();

private:

    static void frameCallback (void);
    static void timerCallback (int);

};