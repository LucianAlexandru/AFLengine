#pragma once
#include <CommonDefines.h>
#include <ComponentDefines.h>
#include <Vector2.h>

class AABoundingBox {

public:

    Collider* const collider;
    const Vector2 bottomLeft;
    const Vector2 topRight;

    AABoundingBox (Collider* collider, Vector2 bottomLeft, Vector2 topRight);

    bool testIntersection (AABoundingBox* other) const;
    bool testIntersection (Vector2 other) const;

    AABBmarkerRef getStart ();
    AABBmarkerRef getEnd ();

};

class AABBmarker {

public:

    enum MarkerPosition {
        Start,
        End
    };

    AABoundingBox* const boundingBox;
    const double x;
    const MarkerPosition position;

    AABBmarker (AABoundingBox* boundingBox, double x, MarkerPosition position);

    bool operator< (const AABBmarker& other) const;

    static bool compareFunc (AABBmarkerRef m1, AABBmarkerRef m2);

};
