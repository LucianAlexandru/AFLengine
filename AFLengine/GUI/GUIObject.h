#pragma once

#include <ObjectDefines.h>

#include <Object.h>
#include <Graphics.h>

class GUIObject : public Object {

public:

    float depth;

    virtual void render () = 0;

protected:

    GUIObject (Identifier objectID);

private:

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};


BOOST_SERIALIZATION_ASSUME_ABSTRACT (GUIObject)

template <class Archive>
void GUIObject::serialize (Archive &ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP (Object);
    ar & boost::serialization::make_nvp ("Depth", depth);
}
