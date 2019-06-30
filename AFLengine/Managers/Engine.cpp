#include <Engine.h>
#include <Input.h>
#include <Time_.h>
#include <Scene.h>
#include <Physics.h>
#include <Graphics.h>

#include <gl\glut.h>

void Engine::init () {

    Input::getInstance ().init ();
    Time::getInstance ().init ();
    Scene::getInstance ().init ();
    Physics::getInstance ().init ();
    Graphics::getInstance ().init ();

    glutSetKeyRepeat (GLUT_KEY_REPEAT_OFF);

    glutKeyboardFunc (Input::keyboardPressCallback);
    glutKeyboardUpFunc (Input::keyboardReleaseCallback);
    glutMouseFunc (Input::mouseInputCallback);
    glutMotionFunc (Input::mousePositionCallback);
    glutReshapeFunc (Graphics::reshapeCallback);
    glutDisplayFunc (Engine::frameCallback);
    glutTimerFunc (1, Engine::timerCallback, 0);

}

void Engine::frameCallback (void) {

    Input::getInstance ().update ();
    Time::getInstance ().update ();

    if (Time::getInstance ().getDeltaTime () > 0.0) {
        Scene::getInstance ().updateFrameStart ();
        Physics::getInstance ().update ();
        Scene::getInstance ().updateFrameEnd ();
    }

    Graphics::getInstance ().update ();

}

void Engine::timerCallback (int) {

    frameCallback ();
    glutTimerFunc (1, Engine::timerCallback, 0);

}
