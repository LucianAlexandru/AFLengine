#pragma once

#include <ManagerDefines.h>
#include <ObjectDefines.h>
#include <ComponentDefines.h>

#include <ColliderExtras.h>
#include <Vector2.h>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

class Physics {

    friend class Engine;

public:

    static Physics& getInstance ();
    void operator= (Physics const&) = delete;
    ~Physics ();

private:

    Physics ();
    Physics (Physics const&);

    void init ();

    void update ();


public:

    Vector2 gravitationalAcceleration;

    ColliderRef hitTest (SceneObjectRef hitObject);

private:

    unsigned int* collisionCountMemory;
    int currentCollisionIndex;

    CollisionDataRef testCollision (Collider* collider1, Collider* collider2);
    bool backtrackObjects (CollisionDataRef collisionData);
    bool separateColliders (CollisionDataRef collisionData);
    bool resolveCollision (CollisionDataRef collisionData);

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};

template <class Archive>
void Physics::serialize (Archive &ar, const unsigned int version) {
    ar & boost::serialization::make_nvp ("GravitationalAcceleration", gravitationalAcceleration);
}
