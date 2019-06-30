#include <FluidRenderer.h>
#include <FluidBody.h>
#include <SceneObject.h>
#include <Collider.h>

#include <glutAdapter.h>
#include <Debug.h>


BOOST_CLASS_EXPORT_IMPLEMENT (FluidRenderer)


FluidRenderer::FluidRenderer ()
: Renderer () {}

void extractOuterVertices (Vector2& topVertex, Vector2& bottomVertex,
                           Collider::VertexList& vertexList, Vector2 centerPosition, Vector2 axis) {

    double topOrientation = -999999;
    double bottomOrientation = 999999;

    for (auto vertex : vertexList) {

        Vector2 deltaPos = vertex - centerPosition;
        double orientation = axis.crossProduct (deltaPos);

        if (orientation > topOrientation) {
            topOrientation = orientation;
            topVertex = vertex;
        }
        if (orientation < bottomOrientation) {
            bottomOrientation = orientation;
            bottomVertex = vertex;
        }
    }

}

void FluidRenderer::update (UpdatePass::Pass pass) {

    SceneObjectRef object = getObject ();

    ComponentRef component = object->getComponent (Component::Type_Body);
    FluidBody* fluidBody = std::dynamic_pointer_cast<FluidBody> (component).get ();
    if (!fluidBody) {
        Debug_Log (Debug::Level_Warning, "[Obj %d] FluidRenderer is missing FluidBody component", getObject ()->objectID);
        return;
    }

    glColor4 (color);

    for (auto p : fluidBody->particles) {

        ParticleRef particle = p.second;

        Collider::VertexList vertexListSelf = particle->collider->getVertexList ();
        Vector2 centerPositionSelf = particle->collider->getGlobalCenterPosition ();

        glBegin (GL_TRIANGLE_FAN);
        for (auto vertex : vertexListSelf)
            glVertex2d (vertex);
        glEnd ();

        for (auto n : particle->neighbours) {

            ParticleRef neighbour = n.lock ();

            if (particle->collider > neighbour->collider)
                continue;

            Collider::VertexList vertexListNeighbour = neighbour->collider->getVertexList ();
            Vector2 centerPositionNeighbour = neighbour->collider->getGlobalCenterPosition ();

            Vector2 deltaAxis = centerPositionNeighbour - centerPositionSelf;
            Vector2 topVertex, bottomVertex;

            glBegin (GL_TRIANGLE_STRIP);
            extractOuterVertices (topVertex, bottomVertex, vertexListSelf, centerPositionSelf, deltaAxis);
            glVertex2d (topVertex);
            glVertex2d (bottomVertex);
            extractOuterVertices (topVertex, bottomVertex, vertexListNeighbour, centerPositionNeighbour, deltaAxis);
            glVertex2d (topVertex);
            glVertex2d (bottomVertex);
            glEnd ();
            
        }
    }
}
