#include <CircleCollider.h>
#include <Transform.h>

#define _USE_MATH_DEFINES
#include <math.h>


BOOST_CLASS_EXPORT_IMPLEMENT (CircleCollider)


CircleCollider::CircleCollider()
: Collider(Collider::Shape_Circle) {
    triangleCount = 32;
}

AABoundingBoxRef CircleCollider::getAABoundingBox () {
    
    Vector2 boxDiag (startFrameRadius, startFrameRadius);
    return AABoundingBoxRef (new AABoundingBox (this, startFrameOrigin - boxDiag, startFrameOrigin + boxDiag));

}

ProjectionAxisList CircleCollider::getProjectionAxes (Collider* other) {
    
    ProjectionAxisList results;

    switch (other->shape) {

    case Collider::Shape_Circle:
    
        if (this > other)
            break;

    case Collider::Shape_Point:
    {
        Vector2 otherCenter = other->getGlobalCenterPosition ();

        results.push_back (ProjectionAxis (otherCenter - startFrameOrigin, startFrameOrigin, startFrameRadius));
        break;
    }

    default:
        break;

    }

    return results;

}

ShapeProjection CircleCollider::projectOntoAxis (Vector2 axis) {

    double centerProjection = axis.dotProduct (startFrameOrigin);
    return ShapeProjection (centerProjection - startFrameRadius, startFrameOrigin - startFrameRadius * axis);

}

void CircleCollider::update (UpdatePass::Pass pass) {

    TransformationMatrix tMatrix = getObject ()->transform->getTransformationMatrix () * TransformationMatrix::Translation (positionOffset);

    startFrameOrigin = tMatrix.getTranslation ();
    Vector2 scale = tMatrix.getScaleSqr ();
    double diameterSqr = scale.x > scale.y ? scale.x : scale.y;
    startFrameRadius = sqrt (diameterSqr) / 2.0;

}

double CircleCollider::momentOfInertiaCoefficient () {

    return startFrameRadius * startFrameRadius;
}

Collider::VertexList CircleCollider::getVertexList () {

    TransformationMatrix tMatrix = getObject ()->transform->getTransformationMatrix () * TransformationMatrix::Translation (positionOffset);

    Vector2 center = tMatrix.getTranslation ();
    Vector2 scale = tMatrix.getScaleSqr ();
    double diameterSqr = scale.x > scale.y ? scale.x : scale.y;
    double radius = sqrt (diameterSqr) / 2.0;
    double step = 2 * M_PI / triangleCount;

    VertexList vertexList;

    vertexList.push_back (center + radius * Vector2 (1, 0));
    for (double angle = step; angle < 2 * M_PI; angle+=step)
        vertexList.push_back (center + radius * Vector2 (cos(angle), sin(angle)));
    vertexList.push_back (center + radius * Vector2 (1, 0));

    return vertexList;

}
