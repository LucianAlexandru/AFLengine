#pragma once

#include <Body.h>
#include <Component.h>
#include <TransformationMatrix.h>

class RigidBody : public Body {

public:

    RigidBody ();

    Vector2 velocity;
    double angularVelocity;

    Vector2 getGlobalVelocity (Collider* collider) override;
    double getGlobalAngularVelocity (Collider* collider) override;

    void applyImpulse (Collider* collider, Vector2 impulse, Vector2 point) override;

    double getMomentOfInertia (Collider* collider) override;

    virtual void update (UpdatePass::Pass pass) override;
    void drawGizmos () override;

protected:

    void advance (Collider* collider, double timeOffset) override;
    void translateCollider (Collider* collider, Vector2 offset) override;

private:

    TransformationMatrix previousFrameTRS, previousFrameParentTRS;
    Vector2 globalVelocity, parentGlobalVelocity;
    double globalAngularVelocity;

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};

typedef std::shared_ptr<RigidBody> RigidBodyRef;


BOOST_CLASS_EXPORT_KEY2 (RigidBody, "Rigid Body")

template <class Archive>
void RigidBody::serialize (Archive &ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP (Body);
    ar & boost::serialization::make_nvp ("Velocity", velocity);
    ar & boost::serialization::make_nvp ("AngularVelocity", angularVelocity);
}
