#pragma once

#include <ComponentDefines.h>

#include <Vector2.h>
#include <list>
#include <memory>

class ProjectionAxis {

public:

    ProjectionAxis (Vector2 axis, Vector2 point);
    ProjectionAxis (Vector2 axis, Vector2 point, double rawDelta);

    double shapeProjection;
    Vector2 axis;

};

typedef std::list<ProjectionAxis> ProjectionAxisList;


class ShapeProjection {

public:

    ShapeProjection (double projection, Vector2 point);
    ShapeProjection (double projection, Vector2 point1, Vector2 point2);

    double projection;
    Vector2 surfaceStart;
    Vector2 surfaceEnd;

};


class CollisionData {

public:

    Collider* faceCollider;
    Collider* vertexCollider;
    
    Vector2 normal;
    double penetration;
    Vector2 collisionPoint;

    double backtrackedTime;

};

typedef std::shared_ptr<CollisionData> CollisionDataRef;