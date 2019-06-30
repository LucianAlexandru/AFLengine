#pragma once

#include <gl\glut.h>
#include <Vector2.h>
#include <Color.h>

inline void glVertex2d (Vector2& point) {
    glVertex2d (point.x, point.y);
}

inline void glRasterPos2d (Vector2& position) {
    glRasterPos2d (position.x, position.y);
}

inline void glOrtho (Vector2& bottomLeft, Vector2& topRight) {
    gluOrtho2D (bottomLeft.x, topRight.x, bottomLeft.y, topRight.y);
}

inline void glClearColor (Color& color) {
    Color::Clamped clampedColor = color.clamp ();
    glClearColor (clampedColor.r, clampedColor.g, clampedColor.b, clampedColor.a);
}

inline void glColor3 (Color& color) {
    glColor3ub (color.r, color.g, color.b);
}

inline void glColor4 (Color& color) {
    glColor4ub (color.r, color.g, color.b, color.a);
}

inline void glTranslate (Vector2& translation) {
    glTranslated (translation.x, translation.y, 0.0);
}

inline void glRotate (double angle) {
    glRotated (angle * 57.295779, 0.0, 0.0, 1.0);
}
