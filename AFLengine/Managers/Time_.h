#pragma once

#include <ManagerDefines.h>
#include <ObjectDefines.h>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

class Time {

    friend class Engine;

public:

    static Time& getInstance ();
    void operator= (Time const&) = delete;
    ~Time ();

private:

    Time ();
    Time (Time const&);

    void init ();

    void update ();


public:

    double getDeltaTime ();
    double getUnscaledDeltaTime ();
    void addTimeScale (double deltaTimeScale);

    void togglePaused ();
    void goToNextFrame ();

    void toggleTimeInfo ();

private:

    double deltaTime, unscaledDeltaTime;
    double timeScale;

    unsigned int* tickCountMemory;
    int currentTickIndex;

    unsigned int frame;

    bool paused;
    bool forwardFrame;

    bool showTimeInfo;

    GUIBoxRef guiBox;
    GUITextRef guiText;


    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};

template <class Archive>
void Time::serialize (Archive &ar, const unsigned int version) {
    ar & boost::serialization::make_nvp ("Frame", frame);
    ar & boost::serialization::make_nvp ("TimeScale", timeScale);
}
