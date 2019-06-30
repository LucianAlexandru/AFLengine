#pragma once
#include <memory>

class SceneObject;
typedef std::shared_ptr<SceneObject> SceneObjectRef;
typedef std::weak_ptr<SceneObject> SceneObjectWeakRef;

class GUIObject;
typedef std::shared_ptr<GUIObject> GUIObjectRef;

class GUIText;
typedef std::shared_ptr<GUIText> GUITextRef;

class GUIBox;
typedef std::shared_ptr<GUIBox> GUIBoxRef;
