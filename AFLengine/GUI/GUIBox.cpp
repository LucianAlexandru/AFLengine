#include <GUIBox.h>

#include <glutAdapter.h>


BOOST_CLASS_EXPORT_IMPLEMENT (GUIBox)


GUIBox::GUIBox ()
: GUIObject (Object::Identifier_Null) {}

GUIBox::GUIBox (Identifier objectID)
: GUIObject (objectID) {}

void GUIBox::render () {

    Graphics& graphics = Graphics::getInstance ();

    Vector2 worldBottomLeft = graphics.convertViewportToWorldPosition (bottomLeft);
    Vector2 worldTopRight = graphics.convertViewportToWorldPosition (topRight);

    ViewportPosition bottomRight = bottomLeft;
    bottomRight.position.x = topRight.position.x;
    bottomRight.margin.x = topRight.margin.x;
    Vector2 worldBottomRight = graphics.convertViewportToWorldPosition (bottomRight);

    ViewportPosition topLeft = bottomLeft;
    topLeft.position.y = topRight.position.y;
    topLeft.margin.y = topRight.margin.y;
    Vector2 worldTopLeft = graphics.convertViewportToWorldPosition (topLeft);

    glColor4 (boxColor);

    glBegin (GL_TRIANGLE_FAN);

    glVertex2d (worldTopRight);
    glVertex2d (worldTopLeft);
    glVertex2d (worldBottomLeft);
    glVertex2d (worldBottomRight);

    glEnd ();

    glColor4 (borderColor);
    glLineWidth (borderSize);

    glBegin (GL_LINE_LOOP);

    glVertex2d (worldTopRight);
    glVertex2d (worldTopLeft);
    glVertex2d (worldBottomLeft);
    glVertex2d (worldBottomRight);

    glEnd ();

}