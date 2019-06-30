#pragma once

#include <ComponentDefines.h>

#include <Vector2.h>

#include <list>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/list.hpp>

class Particle;
typedef std::shared_ptr<Particle> ParticleRef;
typedef std::weak_ptr<Particle> ParticleWeakRef;

class Particle {

public:

    Particle (Collider* collider);

    Collider* collider;
    Vector2 velocity;
    std::list<ParticleWeakRef> neighbours;

private:

    Particle ();

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);
};


template <class Archive>
void Particle::serialize (Archive &ar, const unsigned int version) {
    ar & boost::serialization::make_nvp ("Collider", collider);
    ar & boost::serialization::make_nvp ("Velocity", velocity);
    ar & boost::serialization::make_nvp ("Neighbours", neighbours);
}
