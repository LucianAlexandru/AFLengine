#include <IntVector2.h>

IntVector2::IntVector2 ()
: IntVector2 (0, 0) {}

IntVector2::IntVector2 (int x, int y) {
    this->x = x;
    this->y = y;
}

IntVector2::IntVector2 (Vector2 v) {
    x = v.x;
    y = v.y;
    if (v.x < 0)
        x--;
    if (v.y < 0)
        y--;
}

bool IntVector2::operator== (const IntVector2& v) const {
    return (x == v.x) && (y == v.y);
}

std::list<IntVector2> IntVector2::getNeighbours () {

    int deltaX[] = { 0, 0, 1, 1, 1, 0, -1, -1, -1 };
    int deltaY[] = { 0, 1, 1, 0, -1, -1, -1, 0, 1 };

    std::list<IntVector2> result;
    for (int i = 0; i < 9; i++) {
        IntVector2 v = *this;
        v.x += deltaX[i];
        v.y += deltaY[i];
        result.push_back (v);
    }

    return result;
}