#pragma once

#include <Component.h>
#include <SceneObject.h>
#include <Vector2.h>

class Body : public Component {

    friend class Physics;

public:

    virtual ~Body ();

    double mass;

    // 0 for plastic collision, 1 for ideal elastic
    double elasticity;

    double gravitationalCoefficient;

    virtual Vector2 getGlobalVelocity (Collider* collider) = 0;
    virtual double getGlobalAngularVelocity (Collider* collider) = 0;

    virtual void applyImpulse (Collider* collider, Vector2 impulse, Vector2 point) = 0;

    virtual double getMomentOfInertia (Collider* collider) = 0;

protected:

    Body (unsigned int updatePasses);

    virtual void advance (Collider* collider, double timeOffset) = 0;
    virtual void translateCollider (Collider* collider, Vector2 offset) = 0;

private:

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};


BOOST_SERIALIZATION_ASSUME_ABSTRACT (RigidBody)

template <class Archive>
void Body::serialize (Archive &ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP (Component);
    ar & boost::serialization::make_nvp ("Mass", mass);
    ar & boost::serialization::make_nvp ("Elasticity", elasticity);
    ar & boost::serialization::make_nvp ("GravitationalCoefficient", gravitationalCoefficient);
}