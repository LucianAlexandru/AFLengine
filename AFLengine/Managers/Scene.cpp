#include <Scene.h>
#include <Component.h>

Scene& Scene::getInstance() {
    static Scene instance;
    return instance;
}

Scene::Scene () {
    sceneObjects.clear ();
}

Scene::~Scene () {
    sceneObjects.clear ();
}

void Scene::init () {}

void Scene::updateFrameStart () {
    for (auto pass : UpdatePass::frameStart) {
        for (auto object : sceneObjects) {
            if (object->active)
                object->update (pass);
        }
    }
}

void Scene::updateFrameEnd () {
    for (auto pass : UpdatePass::frameEnd) {
        for (auto object : sceneObjects) {
            if (object->active)
                object->update (pass);
        }
    }
}

bool Scene::addSceneObject (SceneObjectRef object) {

    if (!object)
        return false;
    if (object->objectID == Object::Identifier_Null)
        return false;

    SceneObjectList::reverse_iterator it = sceneObjects.rbegin ();
    for (; it != sceneObjects.rend (); it++) {
        if ((*it)->objectID == object->objectID)
            return false;
        if ((*it)->objectID > object->objectID)
            break;
    }

    sceneObjects.insert (it.base(), object);
    return true;
}

bool Scene::removeSceneObject (SceneObjectRef object) {
    if (!object)
        return false;
    return removeSceneObject (object->objectID);
}

bool Scene::removeSceneObject (Identifier objectID) {

    for (SceneObjectList::iterator it = sceneObjects.begin (); it != sceneObjects.end (); it++) {
        if ((*it)->objectID == objectID) {
            sceneObjects.erase (it);
            return true;
        }
        if ((*it)->objectID > objectID)
            break;
    }

    return false;
}

SceneObjectRef Scene::getSceneObject (Identifier objectID) {

    for (SceneObjectList::iterator it = sceneObjects.begin (); it != sceneObjects.end (); it++) {
        if ((*it)->objectID == objectID) {
            return *it;
        }
        if ((*it)->objectID > objectID)
            break;
    }

    return SceneObjectRef ();
}
