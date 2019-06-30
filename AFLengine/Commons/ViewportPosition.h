#pragma once
#include <Vector2.h>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

struct ViewportPosition {

    Vector2 position;
    Vector2 margin;

private:

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};

template <class Archive>
void ViewportPosition::serialize (Archive &ar, const unsigned int version) {
    ar & boost::serialization::make_nvp ("Position", position);
    ar & boost::serialization::make_nvp ("Margin", margin);
}
