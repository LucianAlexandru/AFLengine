#include <AABoundingBox.h>
#include <math.h>

AABoundingBox::AABoundingBox (Collider* collider, Vector2 bottomLeft, Vector2 topRight)
: collider (collider)
, bottomLeft (bottomLeft)
, topRight (topRight) {}

bool AABoundingBox::testIntersection (AABoundingBox* other) const {

    if (this->bottomLeft.x > other->topRight.x)
        return false;
    if (other->bottomLeft.x > this->topRight.x)
        return false;

    if (this->bottomLeft.y > other->topRight.y)
        return false;
    if (other->bottomLeft.y > this->topRight.y)
        return false;

    return true;

}

bool AABoundingBox::testIntersection (Vector2 other) const {

    if (bottomLeft.x > other.x)
        return false;
    if (topRight.x < other.x)
        return false;

    if (bottomLeft.y > other.y)
        return false;
    if (topRight.y < other.y)
        return false;

    return true;

}

AABBmarkerRef AABoundingBox::getStart () {
    return AABBmarkerRef (new AABBmarker (this, bottomLeft.x, AABBmarker::Start));
}

AABBmarkerRef AABoundingBox::getEnd () {
    return AABBmarkerRef (new AABBmarker (this, topRight.x, AABBmarker::End));
}

AABBmarker::AABBmarker (AABoundingBox* boundingBox, double x, MarkerPosition position)
: boundingBox(boundingBox)
, x(x)
, position(position) {}

bool AABBmarker::operator< (const AABBmarker& other) const {
    if (abs (x - other.x) < FLT_EPSILON)
        return position < other.position;
    return x < other.x;
}

bool AABBmarker::compareFunc (AABBmarkerRef m1, AABBmarkerRef m2) {
    return (*m1) < (*m2);
}
