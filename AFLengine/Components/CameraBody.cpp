#include <CameraBody.h>

#include <Time_.h>
#include <Graphics.h>

#include <SceneObject.h>
#include <Transform.h>

#define FIELD_OF_VIEW_BASE 20
#define FIELD_OF_VIEW_MIN 1


BOOST_CLASS_EXPORT_IMPLEMENT (CameraBody)


CameraBody::CameraBody ()
: RigidBody () {

    fieldOfView = FIELD_OF_VIEW_BASE;

}

void CameraBody::update (UpdatePass::Pass pass) {

    if (pass != UpdatePass::FrameStart_Early)
        return;

    if (velocity.magnitudeSqr () > DBL_EPSILON ||
        abs (angularVelocity) > DBL_EPSILON) {

        SceneObjectRef obj = getObject ();

        double deltaTime = Time::getInstance ().getUnscaledDeltaTime ();
        obj->transform->rotation += deltaTime * angularVelocity;
        obj->transform->position += deltaTime * velocity.rotate (-(obj->transform->rotation)) * fieldOfView / FIELD_OF_VIEW_BASE;
        Graphics::reshape ();

    }

}

void CameraBody::addFieldOfView (double deltaFieldOfView) {
    fieldOfView += deltaFieldOfView;
    fieldOfView = fieldOfView > FIELD_OF_VIEW_MIN ? fieldOfView : FIELD_OF_VIEW_MIN;
    Graphics::reshape ();
}

double CameraBody::getFieldOfView () {
    return fieldOfView;
}
