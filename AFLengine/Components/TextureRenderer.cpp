#include <TextureRenderer.h>
#include <SceneObject.h>
#include <Collider.h>
#include <Debug.h>

#include <SOIL\SOIL.h>


BOOST_CLASS_EXPORT_IMPLEMENT (TextureRenderer)


TextureRenderer::TextureRenderer ()
    : Renderer () {

    glGenTextures (1, &tex);

}

void TextureRenderer::setTexture (std::string fileName) {

    textureFileName = fileName;

    int width, height;
    unsigned char* image = SOIL_load_image (fileName.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);

    if (!image) {
        Debug_Log (Debug::Level_Error, "Could not load texture: %s", fileName.c_str());
    }

    glBindTexture (GL_TEXTURE_2D, tex);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glBindTexture (GL_TEXTURE_2D, 0);

    SOIL_free_image_data (image);

}

void TextureRenderer::update (UpdatePass::Pass pass) {

    SceneObjectRef object = getObject ();
    TransformationMatrix tMatrix = object->transform->getTransformationMatrix ();

    glColor4ub (255, 255, 255, 255);

    glBindTexture (GL_TEXTURE_2D, tex);
    glEnable (GL_TEXTURE_2D);
    glBegin (GL_QUADS);

    glTexCoord2i (0, 0); glVertex2d (tMatrix * Vector2 (-0.5, 0.5));
    glTexCoord2i (0, 1); glVertex2d (tMatrix * Vector2 (-0.5, -0.5));
    glTexCoord2i (1, 1); glVertex2d (tMatrix * Vector2 (0.5, -0.5));
    glTexCoord2i (1, 0); glVertex2d (tMatrix * Vector2 (0.5, 0.5));

    glEnd ();
    glDisable (GL_TEXTURE_2D);
    glBindTexture (GL_TEXTURE_2D, 0);

}