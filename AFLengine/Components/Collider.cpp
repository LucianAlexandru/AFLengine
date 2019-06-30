#include <Collider.h>
#include <Transform.h>
#include <SceneObject.h>

#include <glutAdapter.h>

Collider::Collider (Shape shape)
: Component (Component::Type_Collider, UpdatePass::FrameStart_Late, GizmoPass::Two)
, shape (shape)
, positionOffset() {}

Collider::~Collider () {}

void Collider::drawGizmos () {

    glLineWidth (2);

    glColor3 (Color::GREEN);

    VertexList vertexList = getVertexList ();

    glBegin (GL_LINE_LOOP);
    for (VertexList::iterator vertex = vertexList.begin (); vertex != vertexList.end (); ++vertex)
        glVertex2d (*vertex);
    glEnd ();

}

Vector2 Collider::getGlobalCenterPosition () {
    TransformationMatrix tMatrix = getObject ()->transform->getTransformationMatrix();
    return (tMatrix * TransformationMatrix::Translation(positionOffset)).getTranslation ();
}
