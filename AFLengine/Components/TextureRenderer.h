#pragma once

#include <Renderer.h>
#include <Transform.h>

#include <string>

#include <glutAdapter.h>

class TextureRenderer : public Renderer {

public:

    TextureRenderer ();

    void setTexture (std::string fileName);

    void update (UpdatePass::Pass pass) override;

private:

    GLuint tex;
    std::string textureFileName;

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void save (Archive & ar, const unsigned int version) const;
    template <class Archive>
    void load (Archive & ar, const unsigned int version);
    BOOST_SERIALIZATION_SPLIT_MEMBER ()

};


BOOST_CLASS_EXPORT_KEY2 (TextureRenderer, "Texture Renderer")

template <class Archive>
void TextureRenderer::save (Archive &ar, const unsigned int version) const {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP (Renderer);
    ar & boost::serialization::make_nvp ("TextureFile", textureFileName);
}

template <class Archive>
void TextureRenderer::load (Archive &ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP (Renderer);
    ar & boost::serialization::make_nvp ("TextureFile", textureFileName);
    setTexture (textureFileName);
}
