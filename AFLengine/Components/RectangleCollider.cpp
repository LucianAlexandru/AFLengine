#include <RectangleCollider.h>
#include <Transform.h>


BOOST_CLASS_EXPORT_IMPLEMENT (RectangleCollider)


RectangleCollider::RectangleCollider ()
: Collider (Collider::Shape_Rectangle) {}

AABoundingBoxRef RectangleCollider::getAABoundingBox () {

    Vector2 bottomLeft = startFrameVertices[0];
    Vector2 topRight = startFrameVertices[0];

    for (int i = 1; i < 4; ++i) {

        if (bottomLeft.x > startFrameVertices[i].x)
            bottomLeft.x = startFrameVertices[i].x;
        if (bottomLeft.y > startFrameVertices[i].y)
            bottomLeft.y = startFrameVertices[i].y;

        if (topRight.x < startFrameVertices[i].x)
            topRight.x = startFrameVertices[i].x;
        if (topRight.y < startFrameVertices[i].y)
            topRight.y = startFrameVertices[i].y;

    }

    return AABoundingBoxRef (new AABoundingBox (this, bottomLeft, topRight));

}

ProjectionAxisList RectangleCollider::getProjectionAxes (Collider* other) {

    ProjectionAxisList results;

    // check if the vertices are rotating clockwise or counterclockwise
    double vertexOrientation = (startFrameVertices[1] - startFrameVertices[0])
        .crossProduct ((startFrameVertices[2] - startFrameVertices[1]))
        > 0.0 ? 1.0 : -1.0;

    switch (other->shape) {

    case Collider::Shape_Point:
    case Collider::Shape_Circle:
    {
        Vector2 center = other->getGlobalCenterPosition();

        int closestVertex = 0;
        double minDistSqr = (startFrameVertices[0] - center).magnitudeSqr ();

        //get the vertex closest to the circle center
        for (int i = 0; i < 4; ++i) {
            double distSqr = (startFrameVertices[i] - center).magnitudeSqr ();
            if (minDistSqr > distSqr) {
                minDistSqr = distSqr;
                closestVertex = i;
            }
        }

        Vector2 deltaPos = center - startFrameVertices[closestVertex];
        Vector2 edge, edgeNormal;
        double deltaOrientation1, deltaOrientation2;
        
        //next edge
        edge = startFrameVertices[(closestVertex + 1) % 4] - startFrameVertices[closestVertex];
        edgeNormal = edge.getNormal (vertexOrientation);
        deltaOrientation1 = deltaPos.crossProduct (edgeNormal) * vertexOrientation;
        results.push_back (ProjectionAxis (edgeNormal, startFrameVertices[closestVertex]));
        
        //previous edge
        edge = startFrameVertices[closestVertex] - startFrameVertices[(closestVertex + 3) % 4];
        edgeNormal = edge.getNormal (vertexOrientation);
        deltaOrientation2 = deltaPos.crossProduct (edgeNormal) * vertexOrientation;
        results.push_back (ProjectionAxis (edgeNormal, startFrameVertices[closestVertex]));

        //between
        if (deltaOrientation1 * deltaOrientation2 < 0.0) {
            double deltaOrientation = deltaOrientation1 > 0.0 ? 1.0 : -1.0;
            results.push_back (ProjectionAxis (deltaOrientation * deltaPos, startFrameVertices[closestVertex]));
        }
        break;
    }

    case Collider::Shape_Rectangle:
    case Collider::Shape_Polygon:
    {
        SceneObjectRef obj = this->getObject ();

        for (int i = 1; i < 4; ++i) {
            results.push_back (ProjectionAxis ((startFrameVertices[i] - startFrameVertices[i - 1]).getNormal (vertexOrientation), startFrameVertices[i]));
        }
        results.push_back (ProjectionAxis ((startFrameVertices[0] - startFrameVertices[3]).getNormal (vertexOrientation), startFrameVertices[0]));

        break;
    }

    default:
        break;

    }

    return results;
}

ShapeProjection RectangleCollider::projectOntoAxis (Vector2 axis) {

    double minProjection = axis.dotProduct (startFrameVertices[0]);
    Vector2 point1 = startFrameVertices[0];
    Vector2 point2 = startFrameVertices[0];

    for (int i = 1; i < 4; ++i) {
        double dotProd = axis.dotProduct (startFrameVertices[i]);
        if (minProjection > dotProd) {
            minProjection = dotProd;
            point1 = startFrameVertices[i];
            point2 = startFrameVertices[i];
        }
        else if (abs (minProjection - dotProd) < FLT_EPSILON) {
            point2 = startFrameVertices[i];
        }
    }

    return ShapeProjection (minProjection, point1, point2);

}

void RectangleCollider::update (UpdatePass::Pass pass) {

    TransformationMatrix tMatrix = getObject ()->transform->getTransformationMatrix () * TransformationMatrix::Translation (positionOffset);

    startFrameVertices[0] = tMatrix * Vector2 ( 0.5,  0.5);
    startFrameVertices[1] = tMatrix * Vector2 (-0.5,  0.5);
    startFrameVertices[2] = tMatrix * Vector2 (-0.5, -0.5);
    startFrameVertices[3] = tMatrix * Vector2 ( 0.5, -0.5);

}

double RectangleCollider::momentOfInertiaCoefficient () {

    TransformationMatrix tMatrix = getObject()->transform->getTransformationMatrix () * TransformationMatrix::Translation (positionOffset);;

    Vector2 scale = tMatrix.getScaleSqr ();

    return (scale.x + scale.y) / 12.0;
}

Collider::VertexList RectangleCollider::getVertexList () {

    TransformationMatrix tMatrix = getObject ()->transform->getTransformationMatrix () * TransformationMatrix::Translation (positionOffset);

    VertexList vertexList;

    vertexList.push_back (tMatrix * Vector2 ( 0.5,  0.5));
    vertexList.push_back (tMatrix * Vector2 (-0.5,  0.5));
    vertexList.push_back (tMatrix * Vector2 (-0.5, -0.5));
    vertexList.push_back (tMatrix * Vector2 ( 0.5, -0.5));

    return vertexList;

}
