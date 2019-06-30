#include <Transform.h>

#include <glutAdapter.h>


BOOST_CLASS_EXPORT_IMPLEMENT (Transform)


Transform::Transform ()
: Component (Type_Transform, UpdatePass::FrameStart_Normal, GizmoPass::One)
, position (0.0, 0.0)
, rotation (0.0)
, scale (1.0, 1.0) {}

Transform::~Transform () {}

void Transform::update (UpdatePass::Pass pass) {}

void Transform::drawGizmos () {

    glLineWidth (3);

    glColor3 (Color::BLUE);

    TransformationMatrix transformationMatrix = getTransformationMatrix ();

    glBegin (GL_LINES);
    glVertex2d (transformationMatrix.getTranslation ());
    glVertex2d (transformationMatrix * Vector2 (1, 0));
    glVertex2d (transformationMatrix.getTranslation ());
    glVertex2d (transformationMatrix * Vector2 (0, 1));
    glEnd ();

}

TransformationMatrix Transform::getTransformationMatrix () {

    TransformationMatrix result (position, rotation, scale);

    SceneObjectRef obj = getObject ();
    while ( obj = obj->getParent () ) {
        result = obj->transform->getTransformationMatrix () * result;
    }

    return result;
}

Vector2 Transform::getGlobalPosition () {
    return getTransformationMatrix ().getTranslation ();
}
