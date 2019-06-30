#include <PolygonCollider.h>
#include <Transform.h>

#include <math.h>


BOOST_CLASS_EXPORT_IMPLEMENT (PolygonCollider)


PolygonCollider::PolygonCollider ()
: Collider (Collider::Shape_Polygon) {

    this->vertices.reserve (POLYGON_MAX_VERTICES);
    startFrameVertices.reserve (POLYGON_MAX_VERTICES);

}

void PolygonCollider::setVertices (std::vector<Vector2>& vertices) {

    this->vertices = vertices;
    
}

AABoundingBoxRef PolygonCollider::getAABoundingBox () {

    Vector2 bottomLeft = startFrameVertices[0];
    Vector2 topRight = startFrameVertices[0];

    for (auto vertex : startFrameVertices) {

        if (bottomLeft.x > vertex.x)
            bottomLeft.x = vertex.x;
        if (bottomLeft.y > vertex.y)
            bottomLeft.y = vertex.y;

        if (topRight.x < vertex.x)
            topRight.x = vertex.x;
        if (topRight.y < vertex.y)
            topRight.y = vertex.y;

    }

    return AABoundingBoxRef (new AABoundingBox (this, bottomLeft, topRight));

}

ProjectionAxisList PolygonCollider::getProjectionAxes (Collider* other) {

    ProjectionAxisList results;

    // check if the vertices are rotating clockwise or counterclockwise
    double vertexOrientation = (startFrameVertices[1] - startFrameVertices[0])
        .crossProduct ((startFrameVertices[2] - startFrameVertices[1]))
        > 0.0 ? 1.0 : -1.0;

    switch (other->shape) {

    case Collider::Shape_Point:
    case Collider::Shape_Circle:
    {
        //find the Voronoi zone of the circle centre

        Vector2 center = other->getGlobalCenterPosition ();

        size_t closestVertex = 0;
        size_t vertexCount = startFrameVertices.size ();
        double minDistSqr = (startFrameVertices[0] - center).magnitudeSqr ();

        //get the vertex closest to the circle center
        for (int i = 0; i < vertexCount; ++i) {
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
        edge = startFrameVertices[(closestVertex + 1) % vertexCount] - startFrameVertices[closestVertex];
        edgeNormal = edge.getNormal (vertexOrientation);
        deltaOrientation1 = deltaPos.crossProduct (edgeNormal) * vertexOrientation;
        results.push_back (ProjectionAxis (edgeNormal, startFrameVertices[closestVertex]));

        //previous edge
        edge = startFrameVertices[closestVertex] - startFrameVertices[(closestVertex + vertexCount - 1) % vertexCount];
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
        for (int i = 1; i < startFrameVertices.size(); ++i) {
            results.push_back (ProjectionAxis ((startFrameVertices[i] - startFrameVertices[i - 1]).getNormal (vertexOrientation), startFrameVertices[i]));
        }
        results.push_back (ProjectionAxis ((startFrameVertices[0] - *(startFrameVertices.end() - 1)).getNormal (vertexOrientation), startFrameVertices[0]));

        break;
    }

    default:
        break;

    }

    return results;
}

ShapeProjection PolygonCollider::projectOntoAxis (Vector2 axis) {

    double minProjection = axis.dotProduct (startFrameVertices[0]);
    Vector2 point1 = startFrameVertices[0];
    Vector2 point2 = startFrameVertices[0];

    for (auto vertex : startFrameVertices) {
        double dotProd = axis.dotProduct (vertex);
        if (minProjection > dotProd) {
            minProjection = dotProd;
            point1 = vertex;
            point2 = vertex;
        }
        else if (abs (minProjection - dotProd) < FLT_EPSILON) {
            point2 = vertex;
        }
    }

    return ShapeProjection (minProjection, point1, point2);

}

void PolygonCollider::update (UpdatePass::Pass pass) {

    TransformationMatrix tMatrix = getObject ()->transform->getTransformationMatrix () * TransformationMatrix::Translation (positionOffset);

    startFrameOrigin = tMatrix.getTranslation ();
    startFrameVertices.clear ();

    for (auto vertex : vertices) {
        startFrameVertices.push_back (tMatrix * vertex);
    }
    
}

double PolygonCollider::momentOfInertiaCoefficient () {
    
    double denominator = 0.0;
    double numerator = 0.0;

    for (int i = 0; i < startFrameVertices.size (); ++i) {

        int j = (i + 1) % startFrameVertices.size ();
        Vector2 vertex1 = startFrameVertices[i] - startFrameOrigin;
        Vector2 vertex2 = startFrameVertices[j] - startFrameOrigin;

        double crossProduct = abs(vertex1.crossProduct (vertex2));
        denominator += crossProduct * (vertex1.dotProduct (vertex1) + vertex1.dotProduct (vertex2) + vertex2.dotProduct (vertex2));
        numerator += crossProduct;
    }

    return denominator / (numerator * 6.0);

}

Collider::VertexList PolygonCollider::getVertexList () {

    TransformationMatrix tMatrix = getObject ()->transform->getTransformationMatrix () * TransformationMatrix::Translation (positionOffset);

    VertexList vertexList;

    for (auto vertex : vertices) {
        vertexList.push_back (tMatrix * vertex);
    }

    return vertexList;

}
