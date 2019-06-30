#include <Input.h>
#include <Graphics.h>
#include <Time_.h>
#include <Physics.h>
#include <ObjectFactory.h>

#include <body.h>
#include <CameraBody.h>
#include <Transform.h>
#include <GUIBox.h>
#include <GUIText.h>

#include <gl\freeglut.h>

#include <RigidBody.h>
#include <PointCollider.h>
#include <CircleCollider.h>
#include <RectangleCollider.h>
#include <PolygonCollider.h>
#include <ShapeRenderer.h>


#define FIELD_OF_VIEW_DELTA     1.0
#define TIME_SCALE_DELTA        0.1
#define VELOCITY_UNIT           1.0
#define ROTATION_UNIT           1.5708


Input& Input::getInstance () {
    static Input instance;
    return instance;
}

Input::Input ()
: selectedObject ()
, guiBox()
, guiText() {}

Input::~Input () {}

void Input::init () {

    guiBox = ObjectFactory::createGUIBox ();
    guiBox->nonPersistent = true;
    guiBox->bottomLeft.position = Vector2 (1.0, 1.0);
    guiBox->bottomLeft.margin = Vector2 (-310, -40);
    guiBox->topRight.position = Vector2 (1.0, 1.0);
    guiBox->topRight.margin = Vector2 (-30, -30);
    guiBox->boxColor = Color (64, 64, 128, 128);
    guiBox->borderSize = 4.0;
    guiBox->borderColor = Color (192, 192, 255, 192);

    guiText = ObjectFactory::createGUIText ();
    guiText->nonPersistent = true;
    guiText->color = Color (224, 224, 255);
    guiText->position.position = Vector2 (1.0, 1.0);
    guiText->position.margin = Vector2 (-300, -40);

    mouseInputObject = ObjectFactory::createSceneObject ();
    mouseInputObject->nonPersistent = true;
    mouseInputObject->transform->showGizmos = false;
    ColliderRef mouseInputCollider = ColliderRef (new PointCollider ());
    mouseInputObject->addComponent (mouseInputCollider);
    
    mouseSpring = SpringConstraintRef (new SpringConstraint ());
    mouseSpring->forceDistribution = 1.0;
    mouseSpring->springConstant = 10.0;
    mouseSpring->colliderTarget = mouseInputCollider;

}

void Input::update () {

    SceneObjectRef object = selectedObject.lock ();

    if (object) {

        guiBox->active = true;
        guiText->active = true;

        char buffer[200];
        TransformRef transform = object->transform;
        sprintf_s (buffer, sizeof (buffer), "SceneObject ID: %4d\nPosition:  %+06.2lf   %+06.2lf\nRotation:  %+06.2lf\nScale:       %+06.2lf   %+06.2lf\n\0",
                   object->objectID,
                   transform->position.x, transform->position.y,
                   transform->rotation,
                   transform->scale.x, transform->scale.y);

        guiText->text = buffer;
        guiBox->bottomLeft.margin.y = guiBox->topRight.margin.y - guiText->getTextSize ();

    }
    else {

        guiBox->active = false;
        guiText->active = false;

    }

}

void Input::keyboardPressCallback (unsigned char key, int, int) {
    
    Input& input = Input::getInstance ();

    switch (key) {

        case 'w':
        case 'a':
        case 's':
        case 'd':
            input.keyAddVelocity (key, 1);
            break;

        case 'r':
            input.keyAddRotation (1);
            break;

        case 'o':
        case 'p':
            input.keyFieldOfView (key);
            break;

        case 'g':
            input.keySwitchGizmosState ();
            break;

        case ' ':
            input.keyPause ();
            break;

        case 'n':
            input.keyNextFrame ();
            break;

        case 'k':
        case 'l':
            input.keyTimeScale (key);
            break;

        case 't':
            input.keyTimeInfo ();
            break;

        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '0':
            input.keyCreatePrefabObject (key - '0');
            break;

        case 27: // Escape key
            glutLeaveMainLoop ();
            break;

    }

}

void Input::keyboardReleaseCallback (unsigned char key, int, int) {

    Input& input = Input::getInstance ();

    switch (key) {

        case 'w':
        case 'a':
        case 's':
        case 'd':
            input.keyAddVelocity (key, -1);
            break;

        case 'r':
            input.keyAddRotation (-1);
            break;

    }

}

void Input::mouseInputCallback (int button, int state, int x, int y) {

    Input& input = Input::getInstance ();

    if (button == GLUT_LEFT_BUTTON &&
        state == GLUT_DOWN) {

        Graphics& renderer = Graphics::getInstance ();

        Vector2 hitPosition = renderer.convertScreenToWorldPosition (Vector2 (x, y));
        input.mouseInputObject->transform->position = hitPosition;
        ColliderRef selectedColider = Physics::getInstance ().hitTest (input.mouseInputObject);

        if (!selectedColider) {
            input.selectedObject.reset ();
        }
        else {

            SceneObjectRef selectedObject = selectedColider->getObject ();
            input.selectedObject = selectedObject;

            selectedObject->addComponent (input.mouseSpring);
            input.mouseSpring->localPivotSelf = (selectedObject->transform->getTransformationMatrix ()
                                                 * TransformationMatrix::Translation (selectedColider->positionOffset)).getInverse () * hitPosition;
            input.mouseSpring->colliderSelf = selectedColider;

        }

    }

    if (button == GLUT_LEFT_BUTTON &&
        state == GLUT_UP) {

        SceneObjectRef selectedObject = input.selectedObject.lock ();

        if (selectedObject) {
            selectedObject->removeComponent (input.mouseSpring);
        }

    }

}

void Input::mousePositionCallback (int x, int y) {

    Input& input = Input::getInstance ();
    Graphics& renderer = Graphics::getInstance ();

    input.mouseInputObject->transform->position = renderer.convertScreenToWorldPosition (Vector2 (x, y));;

}

SceneObjectRef Input::getSelectedObject () {
    return selectedObject.lock ();
}

void Input::keyAddVelocity (unsigned char key, int direction) {

    Vector2 velocity;
    switch (key) {
        case 'w':
            velocity.y = VELOCITY_UNIT;
            break;
        case 'a':
            velocity.x = -VELOCITY_UNIT;
            break;
        case 's':
            velocity.y = -VELOCITY_UNIT;
            break;
        case 'd':
            velocity.x = VELOCITY_UNIT;
            break;
    }
    velocity *= direction;

    SceneObjectRef camera = Graphics::getInstance ().getCamera ();
    if (!camera)
        return;

    CameraBodyRef cameraBody = std::static_pointer_cast<CameraBody> (camera->getComponent (Component::Type_Body));
    if (!cameraBody)
        return;

    cameraBody->velocity += velocity;

}

void Input::keyAddRotation (int direction) {

    double rotation = ROTATION_UNIT * direction;

    SceneObjectRef camera = Graphics::getInstance ().getCamera ();
    if (!camera)
        return;

    CameraBodyRef cameraBody = std::static_pointer_cast<CameraBody> (camera->getComponent (Component::Type_Body));
    if (!cameraBody)
        return;

    cameraBody->angularVelocity += rotation;

}

void Input::keyFieldOfView (unsigned int key) {

    double delta;
    switch (key) {
        case 'o':
            delta = -FIELD_OF_VIEW_DELTA;
            break;
        case 'p':
            delta = FIELD_OF_VIEW_DELTA;
            break;
    }

    SceneObjectRef camera = Graphics::getInstance ().getCamera ();
    CameraBodyRef cameraBody = std::static_pointer_cast<CameraBody> (camera->getComponent (Component::Type_Body));
    cameraBody->addFieldOfView (delta);

}

void Input::keySwitchGizmosState () {
    Graphics::getInstance ().switchGizmosState ();
}

void Input::keyPause () {
    Time::getInstance ().togglePaused ();
}

void Input::keyNextFrame () {
    Time::getInstance ().goToNextFrame ();
}

void Input::keyTimeScale (unsigned int key) {

    Time& time = Time::getInstance ();
    
    double delta;
    switch (key) {
        case 'k':
            delta = -TIME_SCALE_DELTA;
            break;
        case 'l':
            delta = TIME_SCALE_DELTA;
            break;
    }

    time.addTimeScale (delta);
}

void Input::keyTimeInfo () {
    Time::getInstance ().toggleTimeInfo ();
}

void Input::keyCreatePrefabObject (unsigned int index) {

    SceneObjectRef obj = ObjectFactory::createSceneObject ();
    Collider* collider = nullptr;
    Body* body = new RigidBody();
    ShapeRenderer* shapeRenderer = new ShapeRenderer ();

    switch (index) {

    case 1:
        collider = new CircleCollider ();
        break;

    case 2:
        collider = new RectangleCollider ();
        break;

    case 3:
    {
        std::vector<Vector2> vertices;
        vertices.push_back (Vector2 (0.0, 0.58));
        vertices.push_back (Vector2 (-0.5, -0.29));
        vertices.push_back (Vector2 (0.5, -0.29));

        PolygonCollider* polygonCollider = new PolygonCollider ();
        polygonCollider->setVertices (vertices);
        collider = polygonCollider;
        break;
    }

    case 4:
    {
        std::vector<Vector2> vertices;
        vertices.push_back (Vector2 (0.0, 1.0));
        vertices.push_back (Vector2 (-0.95, 0.31));
        vertices.push_back (Vector2 (-0.5, -0.48));
        vertices.push_back (Vector2 (0.5, -0.48));
        vertices.push_back (Vector2 (0.95, 0.31));

        PolygonCollider* polygonCollider = new PolygonCollider ();
        polygonCollider->setVertices (vertices);
        collider = polygonCollider;
        break;
    }

    case 5:
        break;

    case 6:
        break;

    case 7:
        break;

    case 8:
        break;

    case 9:
        break;

    case 0:
        break;

    default:
        return;

    }

    shapeRenderer->color = Color::create (obj->objectID);

    obj->addComponent (ComponentRef (collider));
    obj->addComponent (ComponentRef (body));
    obj->addComponent (ComponentRef (shapeRenderer));

    body->mass = 10;
    obj->transform->scale = Vector2 (2.5, 2.5);

}
