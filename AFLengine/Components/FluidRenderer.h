#pragma once

#include <Renderer.h>
#include <Transform.h>

class FluidRenderer : public Renderer {

public:

    FluidRenderer ();

    void update (UpdatePass::Pass pass) override;

private:

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};


BOOST_CLASS_EXPORT_KEY2 (FluidRenderer, "Fluid Renderer")

template <class Archive>
void FluidRenderer::serialize (Archive &ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP (Renderer);
}
