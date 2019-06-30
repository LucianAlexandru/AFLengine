#include <WorldParser.h>

#include <Engine.h>
#include <Scene.h>
#include <Time_.h>
#include <Graphics.h>
#include <Physics.h>
#include <ObjectFactory.h>
#include <Debug.h>

#include <Transform.h>

#include <fstream>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

WorldParser& WorldParser::getInstance () {
    static WorldParser instance;
    return instance;
}

WorldParser::WorldParser () {}

WorldParser::~WorldParser () {}

bool WorldParser::load () {

    std::ifstream ifs (fileName.c_str ());
    if (!ifs.good ()) {
        Debug_Log (Debug::Level_Error, "Input file error: %s", fileName.c_str());
        return false;
    }
    boost::archive::xml_iarchive ia (ifs);
    ia >> boost::serialization::make_nvp ("Time", Time::getInstance ());
    ia >> boost::serialization::make_nvp ("Physics", Physics::getInstance ());
    ia >> boost::serialization::make_nvp ("FactoryID", ObjectFactory::lastObjectID);
    ia >> boost::serialization::make_nvp ("Graphics", Graphics::getInstance ());
    ia >> boost::serialization::make_nvp ("Scene", Scene::getInstance ());
    
    Engine::init ();

    Debug_Log (Debug::Level_Info, "File loaded successfully: %s", fileName.c_str());
    return true;
}

bool WorldParser::save () {

    std::ofstream ofs (fileName.c_str());
    if (!ofs.good ()) {
        Debug_Log (Debug::Level_Error, "Output file error: %s", fileName.c_str());
        return false;
    }
    boost::archive::xml_oarchive oa (ofs);
    oa << boost::serialization::make_nvp ("Time", Time::getInstance ());
    oa << boost::serialization::make_nvp ("Physics", Physics::getInstance ());
    oa << boost::serialization::make_nvp ("FactoryID", ObjectFactory::lastObjectID);
    oa << boost::serialization::make_nvp ("Graphics", Graphics::getInstance ());
    oa << boost::serialization::make_nvp ("Scene", Scene::getInstance ());

    Debug_Log (Debug::Level_Info, "File saved successfully: %s", fileName.c_str());
    return true;
}

void WorldParser::setFile (std::string fileName) {
    this->fileName = fileName;
}