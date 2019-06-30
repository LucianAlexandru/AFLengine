#include <GUIText.h>

#include <glutAdapter.h>

#include <vector>


BOOST_CLASS_EXPORT_IMPLEMENT (GUIText)


GUIText::GUIText ()
: GUIObject (Object::Identifier_Null) {}


GUIText::GUIText (Identifier objectID)
: GUIObject (objectID) {
    setFont (GLUT_BITMAP_9_BY_15);
    text.clear ();
}

void* GUIText::getFont () {
    return font;
}

void GUIText::setFont (void* font) {

    if (font == GLUT_BITMAP_TIMES_ROMAN_10)
        fontSize = 10;
    else if (font == GLUT_BITMAP_TIMES_ROMAN_24)
        fontSize = 24;
    else if (font == GLUT_BITMAP_9_BY_15)
        fontSize = 25;
    else
        return;

    this->font = font;
}

unsigned int GUIText::getTextSize () {

    size_t find = 0, lines = 0;
    while ((find = text.find ('\n', find)) != std::string::npos) {
        ++lines;
        ++find;
    }
    return (unsigned int) fontSize * lines;

}

void GUIText::render () {

    Graphics& graphics = Graphics::getInstance ();

    glColor4 (color);

    std::vector<std::string> textLines;
    size_t pos = 0, find;
    while ((find = text.find ('\n', pos)) != std::string::npos) {
        textLines.push_back (text.substr (pos, find - pos));
        pos = find + 1;
    }
    textLines.push_back (text.substr (pos));

    ViewportPosition textPosition = position;
    textPosition.margin.y -= fontSize / 4;
    for (auto line : textLines) {

        glRasterPos2d (graphics.convertViewportToWorldPosition (textPosition));

        for (std::string::iterator c = line.begin (); c != line.end (); c++)
            glutBitmapCharacter (font, *c);

        textPosition.margin.y -= fontSize;
    }

}