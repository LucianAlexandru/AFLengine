#pragma once

#include <GUIObject.h>
#include <ObjectFactory.h>
#include <ViewportPosition.h>
#include <Color.h>

class GUIBox : public GUIObject {

    friend class ObjectFactory;

public:

    ViewportPosition bottomLeft, topRight;
    Color boxColor, borderColor;
    float borderSize;

    void render () override;

private:

    GUIBox (Identifier objectID);

    // serialization
    GUIBox ();

    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};


BOOST_CLASS_EXPORT_KEY2 (GUIBox, "GUI Box")

template <class Archive>
void GUIBox::serialize (Archive &ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP (GUIObject);
    ar & boost::serialization::make_nvp ("BottomLeft", bottomLeft);
    ar & boost::serialization::make_nvp ("TopRight", topRight);
    ar & boost::serialization::make_nvp ("BoxColor", boxColor);
    ar & boost::serialization::make_nvp ("BorderColor", borderColor);
    ar & boost::serialization::make_nvp ("BorderSize", borderSize);
}
