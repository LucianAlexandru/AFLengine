#include <PointCollider.h>
#include <Transform.h>


BOOST_CLASS_EXPORT_IMPLEMENT (PointCollider)


PointCollider::PointCollider ()
: Collider (Collider::Shape_Point) {}

AABoundingBoxRef PointCollider::getAABoundingBox () {
    Vector2 center = getGlobalCenterPosition();
    return AABoundingBoxRef (new AABoundingBox (this, center, center));
}

ProjectionAxisList PointCollider::getProjectionAxes (Collider* other) {
    return ProjectionAxisList ();
}

ShapeProjection PointCollider::projectOntoAxis (Vector2 axis) {

    Vector2 center = getGlobalCenterPosition ();
    double dotProd = axis.dotProduct (center);

    return ShapeProjection (dotProd, center);

}

void PointCollider::update (UpdatePass::Pass pass) {}

double PointCollider::momentOfInertiaCoefficient () {
    return 0.0;
}

Collider::VertexList PointCollider::getVertexList () {
    return VertexList ();
}
