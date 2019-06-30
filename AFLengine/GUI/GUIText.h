#pragma once

#include <GUIObject.h>
#include <ObjectFactory.h>
#include <Color.h>
#include <ViewportPosition.h>

#include <string>

#include <boost/serialization/string.hpp>

class GUIText : public GUIObject {

    friend class ObjectFactory;

public:

    ViewportPosition position;
    std::string text;
    Color color;

    void render () override;

    void* getFont ();
    void setFont (void* font);

    unsigned int getTextSize ();

private:

    GUIText (Identifier objectID);

    void* font;
    unsigned int fontSize;

    // serialization
    GUIText ();

    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};


BOOST_CLASS_EXPORT_KEY2 (GUIText, "GUI Text")

template <class Archive>
void GUIText::serialize (Archive &ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP (GUIObject);
    ar & boost::serialization::make_nvp ("Position", position);
    ar & boost::serialization::make_nvp ("Text", text);
    ar & boost::serialization::make_nvp ("Color", color);
    ar & boost::serialization::make_nvp ("Font", reinterpret_cast <size_t&> (font));
    setFont (font);
}
