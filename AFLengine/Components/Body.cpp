#include <Body.h>

Body::Body (unsigned int updatePasses)
: Component (Type_Body, updatePasses, GizmoPass::Four)
, mass (1.0)
, elasticity (0.9)
, gravitationalCoefficient (1.0) {}

Body::~Body () {}