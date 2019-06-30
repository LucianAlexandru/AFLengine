#include <ObjectFactory.h>
#include <ComponentDefines.h>
#include <SceneObject.h>
#include <GUIBox.h>
#include <GUIText.h>
#include <Scene.h>
#include <Graphics.h>
#include <Transform.h>

unsigned int ObjectFactory::lastObjectID = 0;

SceneObjectRef ObjectFactory::createSceneObject () {

    Identifier id = ++lastObjectID;

    TransformRef transform (new Transform ());
    SceneObjectRef sceneObject (new SceneObject (id, transform));
    sceneObject->addComponent (transform);

    Scene& scene = Scene::getInstance ();
    scene.addSceneObject (sceneObject);

    return sceneObject;
}

GUIBoxRef ObjectFactory::createGUIBox () {

    Identifier id = ++lastObjectID;

    GUIBoxRef guiBox = GUIBoxRef (new GUIBox (id));

    Graphics& renderer = Graphics::getInstance ();
    renderer.addGUIObject (std::static_pointer_cast <GUIObject> (guiBox));

    return guiBox;
}

GUITextRef ObjectFactory::createGUIText () {

    Identifier id = ++lastObjectID;

    GUITextRef guiText = GUITextRef (new GUIText (id));

    Graphics& renderer = Graphics::getInstance ();
    renderer.addGUIObject (std::static_pointer_cast <GUIObject> (guiText));

    return guiText;
}
