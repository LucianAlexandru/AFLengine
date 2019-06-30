#include <SpringConstraint.h>

#include <Time_.h>

#include <Transform.h>
#include <Body.h>
#include <Collider.h>

#include <glutAdapter.h>

#define GIZMO_LINE_COUNT    5


BOOST_CLASS_EXPORT_IMPLEMENT (SpringConstraint)

SpringConstraint::SpringConstraint ()
: Constraint (UpdatePass::FrameEnd_Normal, GizmoPass::Four)
, colliderSelf ()
, colliderTarget ()
, localPivotSelf ()
, localPivotTarget () {

    restLength = 0.0;
    springConstant = 1.0;
    forceDistribution = 0.5;

}

#include <Debug.h>

void SpringConstraint::update (UpdatePass::Pass pass) {

    ColliderRef colliderTarget = this->colliderTarget.lock ();
    ColliderRef colliderSelf = this->colliderSelf.lock ();
    
    if (!colliderTarget || !colliderSelf)
        return;

    SceneObjectRef objectTarget = colliderTarget->getObject ();
    SceneObjectRef objectSelf = getObject ();

    if (!objectTarget || (objectSelf != colliderSelf->getObject()))
        return;

    Vector2 globalPivotSelf = objectSelf->transform->getTransformationMatrix ()
        * TransformationMatrix::Translation(colliderSelf->positionOffset) * localPivotSelf;
    Vector2 globalPivotTarget = objectTarget->transform->getTransformationMatrix ()
        * TransformationMatrix::Translation (colliderTarget->positionOffset) * localPivotTarget;

    Vector2 distance = globalPivotTarget - globalPivotSelf;
    if (distance.magnitudeSqr () < DBL_EPSILON)
        return;
    Vector2 displacement = distance.normalised() * (distance.magnitude() - restLength);

    double deltaTime = Time::getInstance ().getDeltaTime ();

    BodyRef body;

    body = std::static_pointer_cast<Body> (objectSelf->getComponent (Component::Type_Body));
    if (body) {
        body->applyImpulse (colliderSelf.get(), displacement * springConstant * forceDistribution * deltaTime, globalPivotSelf);
    }

    body = std::static_pointer_cast<Body> (objectTarget->getComponent (Component::Type_Body));
    if (body) {
        body->applyImpulse (colliderTarget.get(), displacement * (-1) * springConstant * (1.0 - forceDistribution) * deltaTime, globalPivotTarget);
    }

}

void SpringConstraint::drawGizmos () {

    ColliderRef colliderTarget = this->colliderTarget.lock ();
    ColliderRef colliderSelf = this->colliderSelf.lock ();

    if (!colliderTarget || !colliderSelf)
        return;

    SceneObjectRef objectTarget = colliderTarget->getObject ();
    SceneObjectRef objectSelf = getObject ();

    if (!objectTarget || (objectSelf != colliderSelf->getObject ()))
        return;

    Vector2 globalPivotSelf = objectSelf->transform->getTransformationMatrix ()
        * TransformationMatrix::Translation (colliderSelf->positionOffset) * localPivotSelf;
    Vector2 globalPivotTarget = objectTarget->transform->getTransformationMatrix ()
        * TransformationMatrix::Translation (colliderTarget->positionOffset) * localPivotTarget;

    Vector2 step = (globalPivotTarget - globalPivotSelf) / (2 * GIZMO_LINE_COUNT - 1);

    glLineWidth (4);

    glColor3 (Color::GRAY);

    glBegin (GL_LINES);
    for (int i = 0; i < 2 * GIZMO_LINE_COUNT; i++) {
        glVertex2d (globalPivotSelf + i * step);
    }
    glEnd ();

}
