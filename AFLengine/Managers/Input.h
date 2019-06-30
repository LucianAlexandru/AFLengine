#pragma once

#include <ManagerDefines.h>
#include <ObjectDefines.h>

#include <SpringConstraint.h>

class Input {

    friend class Engine;

public:

    static Input& getInstance ();
    void operator= (Input const&) = delete;
    ~Input ();

private:

    Input ();
    Input (Input const&);

    void init ();

    void update ();

    static void keyboardPressCallback (unsigned char key, int, int);
    static void keyboardReleaseCallback (unsigned char key, int, int);
    static void mouseInputCallback (int button, int state, int x, int y);
    static void mousePositionCallback (int x, int y);

public:

    SceneObjectRef getSelectedObject ();

private:

    SceneObjectWeakRef selectedObject;

    SceneObjectRef mouseInputObject; 
    SpringConstraintRef mouseSpring;

    GUIBoxRef guiBox;
    GUITextRef guiText;

    // Graphics
    void keyAddVelocity (unsigned char key, int direction);
    void keyAddRotation (int direction);
    void keyFieldOfView (unsigned int key);
    void keySwitchGizmosState ();

    // Time
    void keyPause ();
    void keyNextFrame ();
    void keyTimeScale (unsigned int key);
    void keyTimeInfo ();

    // Scene
    void keyCreatePrefabObject (unsigned int index);

};