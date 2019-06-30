#include <Physics.h>
#include <Scene.h>
#include <Time_.h>
#include <ManagerDefines.h>
#include <SceneObject.h>
#include <Collider.h>
#include <Body.h>

#include <PointCollider.h>
#include <Transform.h>
#include <ObjectFactory.h>

#include <Debug.h>

#include <algorithm>
#include <list>

#define PENETRATION_THRESHOLD   1.0e-032

#define COLLISION_MEMORY_BUFFER_SIZE         50

Physics& Physics::getInstance () {
    static Physics instance;
    return instance;
}

Physics::Physics ()
: gravitationalAcceleration (0.0, -9.80665) {}

Physics::~Physics () {}

void Physics::init () {

    collisionCountMemory = new unsigned int[COLLISION_MEMORY_BUFFER_SIZE];
    currentCollisionIndex = 0;

}

void Physics::update () {

    std::vector<AABoundingBoxRef> arrayAABB;
    std::vector<AABBmarkerRef> arrayMarkers;
    std::list<AABBmarkerRef> activeMarkers;

    int collisionCount = 0;

    //gather all AABB from the scene
    Scene& scene = Scene::getInstance ();
    for (auto object : scene.sceneObjects) {

        if (!object->active)
            continue;

        int colliderCount = object->getComponentCount (Component::Type_Collider);
        for (int i = 0; i < colliderCount; ++i) {

            ComponentRef component = object->getComponent (Component::Type_Collider, i);
            if (!component->active)
                continue;

            ColliderRef collider = std::static_pointer_cast<Collider> (component);
            AABoundingBoxRef aabb = collider->getAABoundingBox ();
            arrayAABB.push_back (aabb);
            arrayMarkers.push_back (aabb->getStart ());
            arrayMarkers.push_back (aabb->getEnd ());

        }
    }

    // sort
    std::sort (arrayMarkers.begin (), arrayMarkers.end (), AABBmarker::compareFunc);

    // sweep
    for (auto marker : arrayMarkers) {

        if (marker->position == AABBmarker::Start) {
            activeMarkers.push_back (marker);
        }

        else {

            //remove paired marker
            std::list<AABBmarkerRef>::iterator pairedMarker;
            for (pairedMarker = activeMarkers.begin (); pairedMarker != activeMarkers.end (); pairedMarker++) {
                if ((*pairedMarker)->boundingBox == marker->boundingBox)
                    break;
            }
            if (pairedMarker == activeMarkers.end ()) {
                Debug_Log (Debug::Level_Error, "[Obj %d] Missing AABB paired marker", marker->boundingBox->collider->getObject ()->objectID);
            }
            activeMarkers.erase (pairedMarker);

            for (auto other : activeMarkers) {

                // test AABB collision
                if (marker->boundingBox->testIntersection (other->boundingBox)) {

                    Collider* collider1 = marker->boundingBox->collider;
                    Collider* collider2 = other->boundingBox->collider;

                    // test collision
                    CollisionDataRef collisionData = testCollision (collider1, collider2);
                    if (!collisionData)
                        continue;

                    collisionCount++;
                    Debug_Log (Debug::Level_Info, "Collison: [Obj %d] [Obj %d]", collider1->getObject ()->objectID, collider2->getObject ()->objectID);

                    // check if objects should interact
                    if (!(collider1->getObject ()->getComponent (Component::Type_Body)) ||
                        !(collider2->getObject ()->getComponent (Component::Type_Body)))
                        continue;

                    if (backtrackObjects (collisionData) == false)
                        separateColliders (collisionData);

                    resolveCollision (collisionData);

                }

            }

        }

    }

    collisionCountMemory[currentCollisionIndex] = collisionCount;
    currentCollisionIndex = (currentCollisionIndex + 1) % COLLISION_MEMORY_BUFFER_SIZE;
    unsigned int averageCollisionCount = 0;
    for (int i = 0; i < COLLISION_MEMORY_BUFFER_SIZE; i++) {
        averageCollisionCount += collisionCountMemory[i];
    }
    Debug_Log (Debug::Level_Info, "Collisions detected: %d (Average %0.2f)", collisionCount, ((float) averageCollisionCount) / COLLISION_MEMORY_BUFFER_SIZE);

    //------------------
    // Debug
    // O(n^2) each collider compared to all others
    //------------------
    //
    //Scene& scene = Scene::getInstance ();
    //for (auto object1 : scene.sceneObjects) {
    //    int colliderCount1 = object1->getComponentCount (Component::Type_Collider);
    //    for (int i = 0; i < colliderCount1; ++i) {
    //
    //        Collider* collider1 = (Collider*) object1->getComponent (Component::Type_Collider, i).get();
    //
    //        for (auto object2 : scene.sceneObjects) {
    //            int colliderCount2 = object2->getComponentCount (Component::Type_Collider);
    //            for (int j = 0; j < colliderCount2; ++j) {
    //
    //                Collider* collider2 = (Collider*) object2->getComponent (Component::Type_Collider, j).get ();
    //
    //                if (collider1 > collider2)
    //                    continue;
    //
    //                if (!testCollision (collider1, collider2))
    //                    continue;
    //
    //                collisionCount++;
    //                Debug_Log (Debug::Level_Info, "Collison: %d %d", collider1->getObject ()->objectID, collider2->getObject ()->objectID);
    //
    //            }
    //        }
    //
    //    }
    //}
    //------------------

}

ColliderRef Physics::hitTest (SceneObjectRef hitObject) {

    Vector2 position = hitObject->transform->position;
    Collider* hitCollider = (Collider*) hitObject->getComponent (Component::Type_Collider).get();

    Scene& scene = Scene::getInstance ();
    for (auto object : scene.sceneObjects) {

        int colliderCount = object->getComponentCount (Component::Type_Collider);
        for (int i = 0; i < colliderCount; ++i) {

            ComponentRef component = object->getComponent (Component::Type_Collider, i);
            ColliderRef collider = std::static_pointer_cast<Collider> (component);

            AABoundingBoxRef aabb = collider->getAABoundingBox ();

            if (aabb->testIntersection (position) == false)
                continue;

            if (testCollision (collider.get (), hitCollider)) {
                Debug_Log (Debug::Level_Debug, "Hit Test Object: %d", collider->getObject ()->objectID);
                return collider;
            }
        }

    }

    return ColliderRef ();

}

CollisionDataRef Physics::testCollision (Collider* collider1, Collider* collider2) {

    CollisionDataRef collisionData(nullptr);

    if (collider1->getObject () == collider2->getObject ())
        return collisionData;

    Vector2 surfaceStart, surfaceEnd;

    ProjectionAxisList projectionAxes = collider1->getProjectionAxes (collider2);

    for (auto projectionAxis : projectionAxes) {

        ShapeProjection projection = collider2->projectOntoAxis (projectionAxis.axis);
        double penetration = projectionAxis.shapeProjection - projection.projection;

        if (penetration < PENETRATION_THRESHOLD)
            return CollisionDataRef ();

        if (!collisionData || (penetration < collisionData->penetration)) {
            collisionData = CollisionDataRef (new CollisionData ());
            collisionData->faceCollider = collider1;
            collisionData->vertexCollider = collider2;
            collisionData->normal = projectionAxis.axis;
            collisionData->penetration = penetration;
            surfaceStart = projection.surfaceStart;
            surfaceEnd = projection.surfaceEnd;
        }
    }

    projectionAxes = collider2->getProjectionAxes (collider1);

    for (auto projectionAxis : projectionAxes) {

        ShapeProjection projection = collider1->projectOntoAxis (projectionAxis.axis);
        double penetration = projectionAxis.shapeProjection - projection.projection;

        if (penetration < PENETRATION_THRESHOLD)
            return CollisionDataRef ();

        if (!collisionData || (penetration < collisionData->penetration)) {
            collisionData = CollisionDataRef (new CollisionData ());
            collisionData->faceCollider = collider2;
            collisionData->vertexCollider = collider1;
            collisionData->normal = projectionAxis.axis;
            collisionData->penetration = penetration;
            surfaceStart = projection.surfaceStart;
            surfaceEnd = projection.surfaceEnd;
        }
        else if (collisionData && abs (penetration - collisionData->penetration) < FLT_EPSILON) {
            projection.surfaceStart += penetration * projectionAxis.axis;
            projection.surfaceEnd += penetration * projectionAxis.axis;
            if (surfaceStart.x < projection.surfaceStart.x) {
                surfaceStart = projection.surfaceStart;
            }
            if (surfaceEnd.x > projection.surfaceEnd.x) {
                surfaceEnd = projection.surfaceEnd;
            }
        }
    }

    if (collisionData) {
        collisionData->collisionPoint = (surfaceStart + surfaceEnd) / 2;
    }

    return collisionData;

}

bool Physics::backtrackObjects (CollisionDataRef collisionData) {

    BodyRef body1 = std::static_pointer_cast<Body> (collisionData->faceCollider->getObject ()->getComponent (Component::Type_Body));
    Vector2 velocity1 = body1->getGlobalVelocity (collisionData->faceCollider);
    double angularVelocity1 = body1->getGlobalAngularVelocity (collisionData->faceCollider);
    Vector2 contactCenterDelta1 = collisionData->collisionPoint + collisionData->normal * collisionData->penetration - collisionData->faceCollider->getGlobalCenterPosition ();

    BodyRef body2 = std::static_pointer_cast<Body> (collisionData->vertexCollider->getObject ()->getComponent (Component::Type_Body));
    Vector2 velocity2 = body2->getGlobalVelocity (collisionData->vertexCollider);
    double angularVelocity2 = body2->getGlobalAngularVelocity (collisionData->vertexCollider);
    Vector2 contactCenterDelta2 = collisionData->collisionPoint - collisionData->vertexCollider->getGlobalCenterPosition ();

    double relativeSpeed = ((velocity1 + contactCenterDelta1.crossProduct (angularVelocity1))
                          - (velocity2 + contactCenterDelta2.crossProduct (angularVelocity2)))
                            .dotProduct (collisionData->normal);

    if (relativeSpeed < 0.0) {
        return false;
    }

    double backtrackedTime = collisionData->penetration / relativeSpeed;
    if (backtrackedTime > Time::getInstance ().getDeltaTime ()) {
        return false;
    }

    body1->advance (collisionData->faceCollider, -backtrackedTime);
    body2->advance (collisionData->vertexCollider, -backtrackedTime);

    collisionData->backtrackedTime = backtrackedTime;
    collisionData->collisionPoint += collisionData->normal * relativeSpeed * backtrackedTime;
    collisionData->penetration = 0.0;

    Debug_Log (Debug::Level_Debug, "Post backtrack (%0.5lf s) collision point: (%0.3lf, %0.3lf)",
               collisionData->backtrackedTime, collisionData->collisionPoint.x, collisionData->collisionPoint.y);
    return true;
}

bool Physics::separateColliders (CollisionDataRef collisionData) {

    BodyRef body1 = std::static_pointer_cast<Body> (collisionData->faceCollider->getObject ()->getComponent (Component::Type_Body));
    double mass1 = body1->mass;
    BodyRef body2 = std::static_pointer_cast<Body> (collisionData->vertexCollider->getObject ()->getComponent (Component::Type_Body));
    double mass2 = body2->mass;

    double pushCoef = mass1 / (mass1 + mass2);
    body1->translateCollider (collisionData->faceCollider, collisionData->normal * collisionData->penetration * (1 - pushCoef) * (-1));
    body2->translateCollider (collisionData->vertexCollider, collisionData->normal * collisionData->penetration * pushCoef);

    collisionData->backtrackedTime = 0.0;
    collisionData->collisionPoint += collisionData->normal * collisionData->penetration * pushCoef;
    collisionData->penetration = 0.0;

    Debug_Log (Debug::Level_Debug, "Post separation collision point: (%0.3lf, %0.3lf)",
               collisionData->collisionPoint.x, collisionData->collisionPoint.y);
    return true;
}

bool Physics::resolveCollision (CollisionDataRef collisionData) {

    BodyRef body1 = std::static_pointer_cast<Body> (collisionData->faceCollider->getObject ()->getComponent (Component::Type_Body));
    Vector2 velocity1 = body1->getGlobalVelocity (collisionData->faceCollider);
    double angularVelocity1 = body1->getGlobalAngularVelocity (collisionData->faceCollider);
    Vector2 contactCenterDelta1 = collisionData->collisionPoint - collisionData->faceCollider->getGlobalCenterPosition ();

    BodyRef body2 = std::static_pointer_cast<Body> (collisionData->vertexCollider->getObject ()->getComponent (Component::Type_Body));
    Vector2 velocity2 = body2->getGlobalVelocity (collisionData->vertexCollider);
    double angularVelocity2 = body2->getGlobalAngularVelocity (collisionData->vertexCollider);
    Vector2 contactCenterDelta2 = collisionData->collisionPoint - collisionData->vertexCollider->getGlobalCenterPosition ();

    Vector2 relativeVelocity =
        (velocity1 + contactCenterDelta1.crossProduct (angularVelocity1))  // initial pre-collision velocity of center of mass body 1 
      - (velocity2 + contactCenterDelta2.crossProduct (angularVelocity2)); // initial pre-collision velocity of center of mass body 2 

    // if object go away from each other, collision is already resolved
    if (relativeVelocity.dotProduct (collisionData->normal) < 0) {
        Debug_Log (Debug::Level_Warning, "Skipping resolution for [obj %d] [obj %d]",
                   collisionData->faceCollider->getObject ()->objectID,
                   collisionData->vertexCollider->getObject ()->objectID);
        return false;
    }

    double momentOfInertia1 = body1->getMomentOfInertia (collisionData->faceCollider);
    double angularMomentum1Sqr = contactCenterDelta1.crossProduct (collisionData->normal) * contactCenterDelta1.crossProduct (collisionData->normal);

    double momentOfInertia2 = body2->getMomentOfInertia (collisionData->vertexCollider);
    double angularMomentum2Sqr = contactCenterDelta2.crossProduct (collisionData->normal) * contactCenterDelta2.crossProduct (collisionData->normal);

    double impulseScale = - relativeVelocity.dotProduct (collisionData->normal)
        / ( (1.0 / body1->mass) + (1.0 / body2->mass) + angularMomentum1Sqr / momentOfInertia1 + angularMomentum2Sqr / momentOfInertia2 );
    Vector2 impulseComponent = impulseScale * collisionData->normal;

    body1->applyImpulse (collisionData->faceCollider, (1.0 + body1->elasticity) * impulseComponent, collisionData->collisionPoint);
    body2->applyImpulse (collisionData->vertexCollider, -(1.0 + body2->elasticity) * impulseComponent, collisionData->collisionPoint);

    // forward propagate the colliders with the backtracked time
    body1->advance (collisionData->faceCollider, collisionData->backtrackedTime);
    body2->advance (collisionData->vertexCollider, collisionData->backtrackedTime);

    return true;
}