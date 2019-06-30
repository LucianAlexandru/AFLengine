#pragma once

#include <Body.h>
#include <Component.h>
#include <Particle.h>
#include <IntVector2.h>

#include <unordered_map>
#include <vector>
#include <list>

#include <boost/serialization/unordered_map.hpp>

class FluidBody : public Body {

    friend class FluidRenderer;

public:

    FluidBody ();

    Vector2 getGlobalVelocity (Collider* collider) override;
    double getGlobalAngularVelocity (Collider* collider) override;

    void applyImpulse (Collider* collider, Vector2 impulse, Vector2 point) override;

    double getMomentOfInertia (Collider* collider) override;

    void update (UpdatePass::Pass pass) override;
    void drawGizmos () override;

protected:

    void advance (Collider* collider, double timeOffset) override;
    void translateCollider (Collider* collider, Vector2 offset) override;

public:

    double neighbourRadius;
    double restDensity;
    double viscosityLinearDependence;
    double viscosityQuadraticDependence;
    double stiffness;
    double nearStiffness;

private:

    std::unordered_map<Collider*, ParticleRef> particles;
    std::unordered_map<IntVector2, std::list<ParticleRef>> particleGrid;

    void applyExternalForces ();
    void applyViscosity ();
    void advanceParticles ();
    void updateNeighbors ();
    void doubleDensityRelaxation ();

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};


BOOST_CLASS_EXPORT_KEY2 (FluidBody, "Fluid Body")

template <class Archive>
void FluidBody::serialize (Archive &ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP (Body);
    ar & boost::serialization::make_nvp ("NeighbourRadius", neighbourRadius);
    ar & boost::serialization::make_nvp ("RestDensity", restDensity);
    ar & boost::serialization::make_nvp ("ViscosityLinearDependence", viscosityLinearDependence);
    ar & boost::serialization::make_nvp ("ViscosityQuadraticDependence", viscosityQuadraticDependence);
    ar & boost::serialization::make_nvp ("Stiffness", stiffness);
    ar & boost::serialization::make_nvp ("NearStiffness", nearStiffness);
    ar & boost::serialization::make_nvp ("Particles", particles);
}
