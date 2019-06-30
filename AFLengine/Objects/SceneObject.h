#pragma once

#include <ObjectFactory.h>
#include <ComponentDefines.h>

#include <Object.h>
#include <Component.h>

#include <memory>
#include <vector>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/weak_ptr.hpp>


class SceneObject : public std::enable_shared_from_this<SceneObject>, public Object {

    friend class ObjectFactory;

public:

    ~SceneObject ();

    const TransformRef transform;

    int getComponentCount (Component::Type type);
    ComponentRef getComponent (Component::Type type, int index = 0);

    bool addComponent (ComponentRef component);

    bool removeComponent (ComponentRef component);
    bool removeComponents (Component::Type type);

    void setParent (SceneObjectRef parent);
    SceneObjectRef getParent ();

    void update (UpdatePass::Pass pass);
    void drawGizmos ();

private:

    SceneObject (Identifier objectID, TransformRef transform);

    typedef std::vector<ComponentRef> ComponentList;
    std::vector<ComponentList> components;

    SceneObjectWeakRef parent;
    std::vector<SceneObjectWeakRef> children;

	// serialization
    SceneObject ();

	friend class boost::serialization::access;
	template <class Archive>
	void serialize (Archive &ar, const unsigned int version);

};


BOOST_CLASS_EXPORT_KEY2 (SceneObject, "Scene Object")

template <class Archive>
void SceneObject::serialize (Archive &ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP (Object);
    ar & boost::serialization::make_nvp ("Parent", parent);
    ar & boost::serialization::make_nvp ("Transform", const_cast <TransformRef&> (transform));
    ar & boost::serialization::make_nvp ("Components", components);
    ar & boost::serialization::make_nvp ("Children", children);
}
