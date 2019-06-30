#pragma once

#include <ComponentDefines.h>

#include <Constraint.h>

#include <Vector2.h>

class ImmovableConstraint : public Constraint {

public:

    ImmovableConstraint ();

    virtual void update (UpdatePass::Pass pass) override;
    virtual void drawGizmos () override;

    void setFixedTransform (TransformRef transform);

private:

    Vector2 position;
    double rotation;
    Vector2 scale;

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};


BOOST_CLASS_EXPORT_KEY2 (ImmovableConstraint, "Immovable Constraint")

template <class Archive>
void ImmovableConstraint::serialize (Archive &ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP (Constraint);
    ar & boost::serialization::make_nvp ("position", position);
    ar & boost::serialization::make_nvp ("rotation", rotation);
    ar & boost::serialization::make_nvp ("scale", scale);
}