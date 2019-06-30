#include <Time_.h>
#include <ObjectFactory.h>
#include <Debug.h>

#include <GUIBox.h>
#include <GUIText.h>

#include <gl/freeglut.h>
#include <math.h>

#define TIME_SCALE_MIN      0.0
#define TIME_SCALE_MAX      5.0

#define BASE_TIME_STEP                  0.0333

#define TICK_MEMORY_BUFFER_SIZE         50

Time& Time::getInstance () {
    static Time instance;
    return instance;
}

Time::Time ()
: deltaTime (0.0)
, unscaledDeltaTime (0.0)
, timeScale (1.0)
, frame (0)
, paused (false)
, forwardFrame (false)
, showTimeInfo (false)
, guiBox ()
, guiText () {

    tickCountMemory = new unsigned int[TICK_MEMORY_BUFFER_SIZE];
    currentTickIndex = 0;

}

Time::~Time () {

    delete tickCountMemory;

}

#include <Debug.h>

void Time::init () {

    guiBox = ObjectFactory::createGUIBox ();
    guiBox->nonPersistent = true;
    guiBox->bottomLeft.position = Vector2 (0.0, 1.0);
    guiBox->bottomLeft.margin = Vector2 (30, -40);
    guiBox->topRight.position = Vector2 (0.0, 1.0);
    guiBox->topRight.margin = Vector2 (275, -30);
    guiBox->boxColor = Color (64, 128, 64, 128);
    guiBox->borderSize = 4.0;
    guiBox->borderColor = Color (192, 255, 192, 192);

    guiText = ObjectFactory::createGUIText ();
    guiText->nonPersistent = true;
    guiText->color = Color (224, 255, 224);
    guiText->position.position = Vector2 (0.0, 1.0);
    guiText->position.margin = Vector2 (40, -40);

    unsigned int tickCount = glutGet (GLUT_ELAPSED_TIME);

    for (int i = 0; i < TICK_MEMORY_BUFFER_SIZE; ++i) {
        tickCountMemory[i] = tickCount;
    }

}

void Time::update () {

    unsigned int newTickCount = glutGet (GLUT_ELAPSED_TIME);
    int nextTickIndex = (currentTickIndex + 1) % TICK_MEMORY_BUFFER_SIZE;
    double fps = 1000.0 * TICK_MEMORY_BUFFER_SIZE / (newTickCount - tickCountMemory[nextTickIndex]);

    unscaledDeltaTime = (newTickCount - tickCountMemory[currentTickIndex]) / 1000.0;

    if (paused) {
        if (forwardFrame) {
            deltaTime = BASE_TIME_STEP;
        }
        else {
            deltaTime = 0.0;
        }
    }
    else {
        deltaTime = unscaledDeltaTime * timeScale;
    }

    forwardFrame = false;
    frame++;

    Debug_Log (Debug::Level_Info, "Frame: %7d. Delta time: %0.5lf. FPS: %3.2lf", frame, deltaTime, fps);

    if (showTimeInfo) {

        guiBox->active = true;
        guiText->active = true;

        char buffer[200];
        sprintf_s (buffer, sizeof (buffer), "Elapsed time: %02d:%02d.%02d\nFrame: %7d\nFPS: %3.2lf\n\0",
                   newTickCount / 60000, (newTickCount / 1000) % 60, (newTickCount % 1000) / 10,
                   frame,
                   fps);

        guiText->text = buffer;
        guiBox->bottomLeft.margin.y = guiBox->topRight.margin.y - guiText->getTextSize ();

    }
    else {

        guiBox->active = false;
        guiText->active = false;

    }

    currentTickIndex = nextTickIndex;
    tickCountMemory[currentTickIndex] = newTickCount;

}

double Time::getDeltaTime () {
    return deltaTime;
}

double Time::getUnscaledDeltaTime () {
    return unscaledDeltaTime;
}

void Time::togglePaused () {
    paused = !paused;
}

void Time::goToNextFrame () {
    forwardFrame = true;
}

void Time::addTimeScale (double deltaTimeScale) {
    timeScale += deltaTimeScale;
    timeScale = timeScale > TIME_SCALE_MIN ? timeScale : TIME_SCALE_MIN;
    timeScale = timeScale < TIME_SCALE_MAX ? timeScale : TIME_SCALE_MAX;
}

void Time::toggleTimeInfo () {
    showTimeInfo = !showTimeInfo;
    guiBox->active = showTimeInfo;
    guiText->active = showTimeInfo;
}
