#pragma once
#include <ObjectDefines.h>

#include <Object.h>
#include <WorldParser.h>

class ObjectFactory {

    friend class WorldParser;

public:

    static SceneObjectRef createSceneObject ();

    static GUIBoxRef createGUIBox ();
    static GUITextRef createGUIText ();

private:

    static Identifier lastObjectID;

};
