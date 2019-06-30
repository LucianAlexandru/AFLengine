#include <ImmovableConstraint.h>
#include <Transform.h>
#include <RigidBody.h>


BOOST_CLASS_EXPORT_IMPLEMENT (ImmovableConstraint)


ImmovableConstraint::ImmovableConstraint ()
: Constraint () {}

void ImmovableConstraint::update (UpdatePass::Pass pass) {

    SceneObjectRef object = getObject ();

    object->transform->position = position;
    object->transform->rotation = rotation;
    object->transform->scale = scale;

    RigidBodyRef rigidBody = std::dynamic_pointer_cast<RigidBody> (object->getComponent (Component::Type_Body));
    if (rigidBody) {
        rigidBody->velocity = Vector2 (0, 0);
        rigidBody->angularVelocity = 0;
    }

}

void ImmovableConstraint::drawGizmos () {}

void ImmovableConstraint::setFixedTransform (TransformRef transform) {
    position = transform->position;
    rotation = transform->rotation;
    scale = transform->scale;
}
