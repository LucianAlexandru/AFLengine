#pragma once

#include <CommonDefines.h>
#include <ComponentDefines.h>
#include <Component.h>

#include <Color.h>

class Renderer : public Component {

public:

    virtual ~Renderer ();

    Color color;

    void drawGizmos () override;

protected:

    Renderer ();

private:

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};


BOOST_CLASS_EXPORT_KEY2 (Renderer, "Renderer")

template <class Archive>
void Renderer::serialize (Archive &ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP (Component);
    ar & boost::serialization::make_nvp ("Color", color);
}
