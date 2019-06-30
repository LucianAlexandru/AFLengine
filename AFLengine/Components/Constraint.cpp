#include <Constraint.h>

Constraint::Constraint ()
: Component (Type_Constraint, UpdatePass::FrameEnd_Normal, GizmoPass::Three) {}

Constraint::Constraint (UpdatePass::Pass updatePass, GizmoPass::Pass gizmoPass)
: Component (Type_Constraint, updatePass, gizmoPass) {}

Constraint::~Constraint () {}