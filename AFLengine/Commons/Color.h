#pragma once

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

class Color {

public:

    static Color RED, GREEN, BLUE, WHITE, GRAY, BLACK;

    unsigned char r, g, b, a;

    class Clamped {

    public:
        float r, g, b, a;

        Clamped (float r, float g, float b, float a);
    };

    Color ();
    Color (unsigned char r, unsigned char g, unsigned char b);
    Color (unsigned char r, unsigned char g, unsigned char b, unsigned char a);

    static Color create (unsigned int seed);

    Clamped clamp ();

    Color operator* (double value);
    Color& operator*= (double value);

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};

template <class Archive>
void Color::serialize (Archive &ar, const unsigned int version) {
    ar & boost::serialization::make_nvp ("Red", r);
    ar & boost::serialization::make_nvp ("Green", g);
    ar & boost::serialization::make_nvp ("Blue", b);
    ar & boost::serialization::make_nvp ("Alpha", a);
}
