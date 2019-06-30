#pragma once

#include <ObjectDefines.h>
#include <ComponentDefines.h>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/export.hpp>

#define COMPONENT_TYPE_COUNT    5

class Component {

    friend class SceneObject;

public:

    virtual ~Component ();

    enum Type {
        Type_Transform,
        Type_Body,
        Type_Collider,
        Type_Renderer,
        Type_Constraint
    };

    const Type type;
    bool active;
    bool showGizmos;

    virtual void update (UpdatePass::Pass pass) = 0;
    virtual void drawGizmos () = 0;

    SceneObjectRef getObject ();

    // defines whether an object can have multiple components of this type
    bool isStackable ();

protected:

    Component (Type type, unsigned int updatePasses, GizmoPass::Pass gizmosPass);

    SceneObjectWeakRef object;

private:

    const unsigned int updatePasses;
    const GizmoPass::Pass gizmosPass;

    // serialization
    Component ();

    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};


BOOST_SERIALIZATION_ASSUME_ABSTRACT (Component)

template <class Archive>
void Component::serialize (Archive &ar, const unsigned int version) {
    //ar & boost::serialization::make_nvp ("Type", const_cast <Type&> (type));
    ar & boost::serialization::make_nvp ("Active", active);
    ar & boost::serialization::make_nvp ("ShowGizmos", showGizmos);
    ar & boost::serialization::make_nvp ("SceneObject", object);
}
