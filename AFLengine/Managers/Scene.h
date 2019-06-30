#pragma once

#include <ManagerDefines.h>
#include <ObjectDefines.h>
#include <ComponentDefines.h>

#include <SceneObject.h>
#include <GUIObject.h>

#include <list>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/shared_ptr.hpp>

class Scene {

    friend class Engine;
    friend class Physics;
    friend class Graphics;

public:

    static Scene& getInstance ();
    void operator= (Scene const&) = delete;
    ~Scene ();

private:

    Scene ();
    Scene (Scene const&);

    void init ();

    void updateFrameStart ();
    void updateFrameEnd ();


public:

    bool addSceneObject (SceneObjectRef object);

    bool removeSceneObject (SceneObjectRef object);
    bool removeSceneObject (Identifier objectID);

    SceneObjectRef getSceneObject (Identifier objectID);

private:

    // SceneObject list is sorted by objectID
    typedef std::list<SceneObjectRef> SceneObjectList;
    SceneObjectList sceneObjects;

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void save (Archive & ar, const unsigned int version) const;
    template <class Archive>
    void load (Archive & ar, const unsigned int version);
    BOOST_SERIALIZATION_SPLIT_MEMBER ()

};


template <class Archive>
void Scene::save (Archive &ar, const unsigned int version) const {
    ar & boost::serialization::make_nvp ("Objects", Object::filterPersistentObjects <std::list, SceneObjectRef> (sceneObjects));
}

template <class Archive>
void Scene::load (Archive &ar, const unsigned int version) {
    ar & boost::serialization::make_nvp ("Objects", sceneObjects);
}
