#pragma once

#include <Component.h>
#include <SceneObject.h>
#include <Vector2.h>
#include <memory>
#include <TransformationMatrix.h>

class Transform : public Component {

public:

    Transform ();
    virtual ~Transform ();

    virtual void update (UpdatePass::Pass pass) override;
    virtual void drawGizmos () override;

public:

    Vector2 position;
    double rotation; //radians
    Vector2 scale;

    TransformationMatrix getTransformationMatrix ();

    Vector2 getGlobalPosition ();

    /*void addWorldOffset (Vector2 worldOffset);
    void addWorldRotation (double worldRotation);*/

private:

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};


BOOST_CLASS_EXPORT_KEY2 (Transform, "Transform")

template <class Archive>
void Transform::serialize (Archive &ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP (Component);
    ar & boost::serialization::make_nvp ("Position", position);
    ar & boost::serialization::make_nvp ("Rotation", rotation);
    ar & boost::serialization::make_nvp ("Scale", scale);
}
