#include <Vector2.h>
#include <math.h>

Vector2::Vector2 ()
: Vector2 (0.0, 0.0) {}

Vector2::Vector2 (double x, double y) {
    this->x = x;
    this->y = y;
}


Vector2& Vector2::operator+= (const Vector2& v) {
    x += v.x;
    y += v.y;
    return *this;
}

Vector2& Vector2::operator-= (const Vector2& v) {
    x -= v.x;
    y -= v.y;
    return *this;
}

Vector2& Vector2::operator*= (double value) {
    x *= value;
    y *= value;
    return *this;
}

Vector2& Vector2::operator/= (double value) {
    x /= value;
    y /= value;
    return *this;
}

Vector2 Vector2::operator+ (const Vector2& v) {
    return Vector2 (x + v.x, y + v.y);
}

Vector2 Vector2::operator- (const Vector2& v) {
    return Vector2 (x - v.x, y - v.y);
}

Vector2 Vector2::operator* (double value) {
    return Vector2 (x * value, y * value);
}

Vector2 Vector2::operator/ (double value) {
    return Vector2 (x / value, y / value);
}

Vector2 operator* (double value, Vector2& v) {
    return v * value;
}

double Vector2::dotProduct (const Vector2& v) {
    return x * v.x + y * v.y;
}

double Vector2::crossProduct (const Vector2& v) {
    return x * v.y - y * v.x;
}

Vector2 Vector2::crossProduct (const double value) {
    return Vector2 (-value * y, value * x);
}

double Vector2::magnitudeSqr () {
    return x * x + y * y;
}

double Vector2::magnitude () {
    return sqrt (x * x + y * y);
}

Vector2 Vector2::normalised () {
    return Vector2 (x, y) / magnitude ();
}

Vector2 Vector2::getNormal (double orientation) {
    return Vector2 (y * orientation, - x * orientation);
}

Vector2 Vector2::rotate (double angle) {
    double cosAngle = cos (angle), sinAngle = sin (angle);
    return Vector2 (x * cosAngle - y * sinAngle, y * cosAngle + x * sinAngle);
}