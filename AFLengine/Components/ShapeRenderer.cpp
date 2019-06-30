#include <ShapeRenderer.h>
#include <SceneObject.h>
#include <Collider.h>

#include <glutAdapter.h>

#define COLLISION_REGISTERED_LIGHTEN_FACTOR 1


BOOST_CLASS_EXPORT_IMPLEMENT (ShapeRenderer)


ShapeRenderer::ShapeRenderer ()
: Renderer () {}

void ShapeRenderer::update (UpdatePass::Pass pass) {

    SceneObjectRef object = getObject ();

    int colliderCount = object->getComponentCount (Component::Type_Collider);
    for (int i = 0; i < colliderCount; ++i) {

        ColliderRef collider = std::static_pointer_cast<Collider> (object->getComponent (Component::Type_Collider, i));

        Collider::VertexList vertexList = collider->getVertexList ();

        glColor4 (color);
        glBegin (GL_TRIANGLE_FAN);
        for (Collider::VertexList::iterator vertex = vertexList.begin (); vertex != vertexList.end (); ++vertex)
            glVertex2d (*vertex);
        glEnd ();
    }
}
