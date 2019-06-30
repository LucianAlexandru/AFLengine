#pragma once
#include <memory>

class Component;
typedef std::shared_ptr<Component> ComponentRef;

class Transform;
typedef std::shared_ptr<Transform> TransformRef;

class Collider;
typedef std::shared_ptr<Collider> ColliderRef;
typedef std::weak_ptr<Collider> ColliderWeakRef;

class Body;
typedef std::shared_ptr<Body> BodyRef;

class Renderer;
typedef std::shared_ptr<Renderer> RendererRef;

class Constraint;
typedef std::shared_ptr<Constraint> ConstraintRef;

namespace UpdatePass {

    enum Pass {
        FrameStart_Early    = 1,
        FrameStart_Normal   = 2,
        FrameStart_Late     = 4,

        FrameEnd_Early      = 8,
        FrameEnd_Normal     = 16,
        FrameEnd_Late       = 32,

        Render              = 64
    };

    const Pass frameStart[3] = { FrameStart_Early, FrameStart_Normal, FrameStart_Late };
    const Pass frameEnd[3] = { FrameEnd_Early, FrameEnd_Normal, FrameEnd_Late };

}

namespace GizmoPass {

    enum Pass {
        One,
        Two,
        Three,
        Four
    };

    const Pass passes[4] = { One, Two, Three, Four };

}