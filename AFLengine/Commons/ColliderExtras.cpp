#include <ColliderExtras.h>

ProjectionAxis::ProjectionAxis (Vector2 axis, Vector2 point) {
    this->axis = axis.normalised();
    this->shapeProjection = this->axis.dotProduct (point);
}

ProjectionAxis::ProjectionAxis (Vector2 axis, Vector2 point, double rawDelta) {
    this->axis = axis.normalised ();
    this->shapeProjection = this->axis.dotProduct (point) + rawDelta;
}

ShapeProjection::ShapeProjection (double projection, Vector2 point) {
    this->projection = projection;
    this->surfaceStart = point;
    this->surfaceEnd = point;
}

ShapeProjection::ShapeProjection (double projection, Vector2 point1, Vector2 point2) {
    this->projection = projection;
    if (point1.x <= point2.x) {
        this->surfaceStart = point1;
        this->surfaceEnd = point2;
    }
    else {
        this->surfaceStart = point2;
        this->surfaceEnd = point1;
    }
}