#include <Graphics.h>
#include <Input.h>
#include <Time_.h>
#include <Scene.h>

#include <SceneObject.h>
#include <Transform.h>
#include <CameraBody.h>

#include <glutAdapter.h>

Graphics& Graphics::getInstance () {
    static Graphics instance;
    return instance;
}

Graphics::Graphics ()
: camera()
, gizmosState (Gizmos_Selected) {

    guiObjects.clear ();

    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

Graphics::~Graphics () {}

void Graphics::init () {

    glClearColor (backgroundColor);

}

void Graphics::update () {

    glClear (GL_COLOR_BUFFER_BIT);

    Scene& scene = Scene::getInstance ();

    // Draw scene objects
    for (auto object : scene.sceneObjects) {
        if (object->active)
            object->update (UpdatePass::Render);
    }

    // Draw gizmos
    switch (gizmosState) {

        case Gizmos_None:
            break;

        case Gizmos_Selected:
        {
            SceneObjectRef selectedObject = Input::getInstance ().getSelectedObject ();
            if (selectedObject && selectedObject->active)
                selectedObject->drawGizmos ();
        } break;

        case Gizmos_All:
        {
            for (auto object : scene.sceneObjects) {
                if (object->active)
                    object->drawGizmos ();
            }
        } break;

    }

    // Draw GUI objects
    for (auto guiObject : guiObjects) {
        if (guiObject->active)
            guiObject->render ();
    }

    glutSwapBuffers ();
}

void Graphics::reshape () {

    Graphics& renderer = getInstance ();
    CameraBodyRef cameraBody = std::static_pointer_cast<CameraBody> (renderer.camera->getComponent (Component::Type_Body));
    double fieldOfView = cameraBody->getFieldOfView ();
    Vector2 ortoField (fieldOfView, fieldOfView * renderer.windowSize.y / renderer.windowSize.x);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho (ortoField / (-2) , ortoField / 2);
    glRotate (renderer.camera->transform->rotation);
    glTranslate (renderer.camera->transform->position * (-1));
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

}

void Graphics::reshapeCallback (int w, int h) {

    Graphics& renderer = getInstance ();
    renderer.windowSize = Vector2 (w, h);

    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    reshape ();

}

Vector2 Graphics::convertScreenToWorldPosition (Vector2 screenPosition) {

    double fieldOfView = std::static_pointer_cast<CameraBody> (camera->getComponent (Component::Type_Body))->getFieldOfView ();
    double cameraRotation = camera->transform->rotation;

    Vector2 convertedViewport (screenPosition.x - windowSize.x / 2, windowSize.y / 2 - screenPosition.y);
    Vector2 worldOffset = convertedViewport * fieldOfView / windowSize.x;

    return camera->transform->position + worldOffset.rotate (-cameraRotation);
}

Vector2 Graphics::convertViewportToWorldPosition (ViewportPosition viewportPosition) {

    Vector2 screenPosition = windowSize;

    screenPosition.x *= viewportPosition.position.x;
    screenPosition.y *= 1.0 - viewportPosition.position.y;
    screenPosition.x += viewportPosition.margin.x;
    screenPosition.y -= viewportPosition.margin.y;

    return convertScreenToWorldPosition (screenPosition);
}

void Graphics::setCamera (SceneObjectRef camera) {
    if (!camera)
        return;
    CameraBodyRef cameraBody = std::static_pointer_cast<CameraBody> (camera->getComponent (Component::Type_Body));
    if (!cameraBody)
        return;
    this->camera = camera;
}

SceneObjectRef Graphics::getCamera () {
    return camera;
}

void Graphics::setBackgroundColor (Color color) {
    backgroundColor = color;
    glClearColor (backgroundColor);
}

bool Graphics::addGUIObject (GUIObjectRef guiObject) {

    if (!guiObject)
        return false;
    if (guiObject->objectID == Object::Identifier_Null)
        return false;

    GUIObjectList::iterator it = guiObjects.begin ();
    for (; it != guiObjects.end (); ++it) {
        if ((*it)->depth >= guiObject->depth)
            break;
    }

    guiObjects.insert (it, guiObject);
    return true;
}

bool Graphics::removeGUIObject (GUIObjectRef guiObject) {
    if (!guiObject)
        return false;
    return removeGUIObject (guiObject->objectID);
}

bool Graphics::removeGUIObject (Identifier objectID) {

    for (GUIObjectList::iterator it = guiObjects.begin (); it != guiObjects.end (); it++) {
        if ((*it)->objectID == objectID) {
            guiObjects.erase (it);
            return true;
        }
    }

    return false;
}

GUIObjectRef Graphics::getGUIObject (Identifier objectID) {

    for (GUIObjectList::iterator it = guiObjects.begin (); it != guiObjects.end (); it++) {
        if ((*it)->objectID == objectID) {
            return *it;
        }
        if ((*it)->objectID > objectID)
            break;
    }

    return GUIObjectRef ();
}

void Graphics::switchGizmosState () {
    gizmosState = (GizmosState) ((gizmosState + 1) % 3);
}