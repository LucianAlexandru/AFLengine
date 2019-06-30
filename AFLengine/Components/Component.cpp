#include <Component.h>

Component::Component ()
: type (Type_Transform)
, active (true)
, updatePasses (UpdatePass::FrameStart_Normal)
, gizmosPass (GizmoPass::One) {}


Component::Component (Type type, unsigned int updatePass, GizmoPass::Pass gizmoPass)
: type(type)
, active (true)
, showGizmos (true)
, updatePasses (updatePass)
, gizmosPass (gizmoPass) {}

Component::~Component () {}

bool Component::isStackable () {

    switch (type) {
        case Type_Collider:
        case Type_Constraint:
            return true;
        default:
            return false;
    }

}

SceneObjectRef Component::getObject () {
    return object.lock();
}
