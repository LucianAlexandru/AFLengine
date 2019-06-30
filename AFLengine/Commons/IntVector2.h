#pragma once

#include <Vector2.h>
#include <list>

class IntVector2 {

public:

    int x, y;

    IntVector2 ();
    IntVector2 (int x, int y);
    IntVector2 (Vector2 v);

    bool operator== (const IntVector2& v) const;

    std::list<IntVector2> getNeighbours ();

};

namespace std
{
    template<>
    struct hash<IntVector2>
    {
        size_t operator()(const IntVector2& v) const
        {
            hash<int> hasher;
            size_t h1 = hasher (v.x);
            size_t h2 = hasher (v.y);
            return h1 + (h2<<6) + (h2>>2);
        }
    };
}
