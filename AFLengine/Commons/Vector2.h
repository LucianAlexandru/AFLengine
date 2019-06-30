#pragma once

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

class Vector2
{

public:

    double x, y;

    Vector2 ();
    Vector2 (double x, double y);

    Vector2& operator+= (const Vector2& v);
    Vector2& operator-= (const Vector2& v);
    Vector2& operator*= (double value);
    Vector2& operator/= (double value);

    Vector2 operator+ (const Vector2& v);
    Vector2 operator- (const Vector2& v);
    Vector2 operator* (double value);
    Vector2 operator/ (double value);

    double dotProduct (const Vector2& v);
    double crossProduct (const Vector2& v);

    // (0, 0, value) cross (x, y, 0)
    Vector2 crossProduct (const double value);

    double magnitudeSqr ();
    double magnitude ();
    Vector2 normalised ();

    Vector2 getNormal (double orientation = 1.0);

    Vector2 rotate (double angle);

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};

Vector2 operator* (double value, Vector2& v);


template <class Archive>
void Vector2::serialize (Archive &ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_NVP (x);
    ar & BOOST_SERIALIZATION_NVP (y);
}
