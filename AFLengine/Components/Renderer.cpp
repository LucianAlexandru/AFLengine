#include <Renderer.h>


BOOST_CLASS_EXPORT_IMPLEMENT (Renderer)


Renderer::Renderer ()
: Component (Component::Type_Renderer, UpdatePass::Render, GizmoPass::One) {}

Renderer::~Renderer () {}

void Renderer::drawGizmos () {}