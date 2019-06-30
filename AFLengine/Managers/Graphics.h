#pragma once

#include <ManagerDefines.h>
#include <ObjectDefines.h>

#include <GUIObject.h>
#include <Color.h>
#include <Vector2.h>
#include <ViewportPosition.h>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>

class Graphics {

    friend class Engine;

public:

    static Graphics& getInstance ();
    void operator= (Graphics const&) = delete;
    ~Graphics ();

private:

    Graphics ();
    Graphics (Graphics const&);

    void init ();

    void update ();

    static void reshapeCallback (int w, int h);


public:

    static void reshape ();

    Vector2 convertScreenToWorldPosition (Vector2 screenPosition);
    Vector2 convertViewportToWorldPosition (ViewportPosition viewportPosition);

    void setCamera (SceneObjectRef camera);
    SceneObjectRef getCamera ();

    void setBackgroundColor (Color color);

    bool addGUIObject (GUIObjectRef guiObject);

    bool removeGUIObject (GUIObjectRef guiObject);
    bool removeGUIObject (Identifier objectID);

    GUIObjectRef getGUIObject (Identifier objectID);

    void switchGizmosState ();

private:

    SceneObjectRef camera;
    Vector2 windowSize;

    Color backgroundColor;

    enum GizmosState {
        Gizmos_None,
        Gizmos_Selected,
        Gizmos_All
    };
    GizmosState gizmosState;

    // GUIObject list sorted by z
    typedef std::list<GUIObjectRef> GUIObjectList;
    GUIObjectList guiObjects;

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void save (Archive & ar, const unsigned int version) const;
    template <class Archive>
    void load (Archive & ar, const unsigned int version);
    BOOST_SERIALIZATION_SPLIT_MEMBER ()

};

template <class Archive>
void Graphics::save (Archive &ar, const unsigned int version) const {
    ar & boost::serialization::make_nvp ("Camera", camera);
    ar & boost::serialization::make_nvp ("BackgroundColor", backgroundColor);
    ar & boost::serialization::make_nvp ("GUI", Object::filterPersistentObjects (guiObjects));
}

template <class Archive>
void Graphics::load (Archive &ar, const unsigned int version) {
    ar & boost::serialization::make_nvp ("Camera", camera);
    ar & boost::serialization::make_nvp ("BackgroundColor", backgroundColor);
    ar & boost::serialization::make_nvp ("GUI", guiObjects);
}
