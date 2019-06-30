#pragma once

#include <Renderer.h>
#include <Transform.h>

class ShapeRenderer : public Renderer {

public:

    ShapeRenderer ();

    void update (UpdatePass::Pass pass) override;

private:

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};


BOOST_CLASS_EXPORT_KEY2 (ShapeRenderer, "Shape Renderer")

template <class Archive>
void ShapeRenderer::serialize (Archive &ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP (Renderer);
}
