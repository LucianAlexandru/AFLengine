#include <RigidBody.h>
#include <Time_.h>
#include <Physics.h>

#include <SceneObject.h>
#include <Transform.h>
#include <Collider.h>

#include <glutAdapter.h>

BOOST_CLASS_EXPORT_IMPLEMENT (RigidBody)


RigidBody::RigidBody ()
: Body (UpdatePass::FrameStart_Early | UpdatePass::FrameStart_Late) {}

void RigidBody::update (UpdatePass::Pass pass) {

    TransformRef transform = getObject ()->transform;
    double deltaTime = Time::getInstance ().getDeltaTime ();

    if (pass == UpdatePass::FrameStart_Early) {

        // cache the position from the previous frame 
        previousFrameTRS = transform->getTransformationMatrix ();
        if (SceneObjectRef parent = getObject ()->getParent ()) {
            previousFrameParentTRS = parent->transform->getTransformationMatrix ();
        }

        advance (nullptr, deltaTime);
        
    }
    else if (pass == UpdatePass::FrameStart_Late) {

        TransformationMatrix changeMatrix = transform->getTransformationMatrix () * previousFrameTRS.getInverse ();
        Vector2 deltaPosition = transform->getGlobalPosition () - previousFrameTRS.getTranslation ();
        globalVelocity = deltaPosition / deltaTime;
        globalAngularVelocity = changeMatrix.getClampedRotation () / deltaTime;
        if (SceneObjectRef parent = getObject ()->getParent ()) {
            changeMatrix = previousFrameParentTRS.getInverse () * parent->transform->getTransformationMatrix ();
            deltaPosition = parent->transform->getGlobalPosition () - previousFrameParentTRS.getTranslation ();
            parentGlobalVelocity = deltaPosition / deltaTime;
        }

    }

}

void RigidBody::drawGizmos () {

    glLineWidth (4);

    glColor3 (Color::RED);

    Vector2 center = getObject ()->transform->getGlobalPosition ();
    Vector2 velocityEndPoint = getObject ()->transform->position + velocity;
    if (SceneObjectRef parent = getObject ()->getParent ()) {
        velocityEndPoint = parent->transform->getTransformationMatrix () * velocityEndPoint;
    }

    glBegin (GL_LINES);
    glVertex2d (center);
    glVertex2d (velocityEndPoint);
    glEnd ();

}

Vector2 RigidBody::getGlobalVelocity (Collider* collider) {
    return globalVelocity;
}

double RigidBody::getGlobalAngularVelocity (Collider* collider) {
    return globalAngularVelocity;
}

void RigidBody::advance (Collider* collider, double timeOffset) {

    TransformRef transform = getObject ()->transform;
    Vector2 gravitationalAcceleration = Physics::getInstance ().gravitationalAcceleration * gravitationalCoefficient;

    if (SceneObjectRef parent = getObject ()->getParent ()) {
        gravitationalAcceleration = previousFrameParentTRS.delocalizeVector (gravitationalAcceleration);
    }

    transform->position += timeOffset * velocity + timeOffset * timeOffset * gravitationalAcceleration / 2;
    transform->rotation += timeOffset * angularVelocity;

    velocity += timeOffset * gravitationalAcceleration;

}

void RigidBody::translateCollider (Collider* collider, Vector2 offset) {

    if (SceneObjectRef parent = getObject ()->getParent ()) {
        offset = parent->transform->getTransformationMatrix ().delocalizeVector (offset);
    }
    getObject ()->transform->position += offset;

}

void RigidBody::applyImpulse (Collider* collider, Vector2 impulse, Vector2 point) {

    double deltaTime = Time::getInstance ().getDeltaTime ();

    // update velocity
    Vector2 deltaVelocity = impulse / mass;
    globalVelocity += deltaVelocity;

    // if the object has a parent, substract its velocity and convert to local worldspace
    SceneObjectRef parent = getObject ()->getParent ();
    if (parent) {
        globalVelocity -= parentGlobalVelocity;
        globalVelocity = parent->transform->getTransformationMatrix ().delocalizeVector(globalVelocity);
    }

    velocity = globalVelocity;

    //update angular velocity
    Vector2 center = collider->getGlobalCenterPosition ();
    Vector2 pointCenterDelta = point - center;
    double deltaAngularVelocity = pointCenterDelta.crossProduct (impulse) / getMomentOfInertia (collider);
    globalAngularVelocity += deltaAngularVelocity;

    angularVelocity = globalAngularVelocity;
}

double RigidBody::getMomentOfInertia (Collider*) {

    double momentOfInertiaCoefficientSum = 0.0;

    SceneObjectRef object = getObject ();
    int colliderCount = object->getComponentCount (Component::Type_Collider);
    for (int i = 0; i < colliderCount; ++i) {
        ComponentRef component = object->getComponent (Component::Type_Collider, i);
        ColliderRef collider = std::static_pointer_cast<Collider> (component);
        momentOfInertiaCoefficientSum += collider->momentOfInertiaCoefficient();
    }

    return momentOfInertiaCoefficientSum * mass;
}