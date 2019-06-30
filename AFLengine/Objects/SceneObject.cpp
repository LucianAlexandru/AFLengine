#include <SceneObject.h>
#include <Transform.h>
#include <Collider.h>
#include <Renderer.h>
#include <memory>


BOOST_CLASS_EXPORT_IMPLEMENT (SceneObject)


SceneObject::SceneObject () : Object (Object::Identifier_Null) {}

SceneObject::SceneObject (Identifier objectID, TransformRef transform)
: Object (objectID)
, components (COMPONENT_TYPE_COUNT)
, parent ()
, transform (transform) {}

SceneObject::~SceneObject () {
    
    for (int i = 0; i < COMPONENT_TYPE_COUNT; ++i) {
        for (int j = 0; j < components[i].size (); ++j)
            components[i][j]->object.reset ();
        components[i].clear ();
    }
    
    SceneObjectRef p = getParent ();
    if (p) {
        for (auto c = p->children.begin (); c < p->children.end (); c++) {
            if (c->lock ().get () == this) {
                p->children.erase (c);
                break;
            }
        }
    }

}

int SceneObject::getComponentCount (Component::Type type) {
    return (int) components[type].size ();
}

ComponentRef SceneObject::getComponent (Component::Type type, int index) {

    if (index < components[type].size())
        return components[type][index];

    return ComponentRef();
}

bool SceneObject::addComponent (ComponentRef component) {

    if (!component)
        return false;

    if (component->isStackable () == false) {
        if (components[component->type].size() > 0)
            return false;
    }

    {
        SceneObjectRef object = component->getObject ();
        if (object) {
            if (!object->removeComponent (component))
                return false;
        }
    }

    components[component->type].push_back (component);
    component->object = shared_from_this ();
    return true;
}

bool SceneObject::removeComponent (ComponentRef component) {

    if (!component)
        return false;
    if (component->type == Component::Type_Transform)
        return false;

    int type = component->type;

    for (ComponentList::iterator comp = components[type].begin (); comp != components[type].end (); ++comp)
        if (*comp == component) {
            component->object.reset ();
            components[type].erase (comp);
            return true;
        }

    return false;
}

bool SceneObject::removeComponents (Component::Type type) {

    if (type == Component::Type_Transform)
        return false;

    if (components[type].size () == 0)
        return false;

    for (ComponentList::iterator comp = components[type].begin (); comp != components[type].end (); ++comp) {
        (*comp)->object.reset ();
    }

    components[type].clear ();
    return true;
}

void SceneObject::setParent (SceneObjectRef parent) {
    {
        SceneObjectRef p = getParent ();
        if (p) {
            for (auto c = p->children.begin (); c < p->children.end (); c++) {
                if (c->lock().get() == this) {
                    p->children.erase (c);
                    break;
                }
            }
        }
    }
    this->parent = parent;
    if (parent)
        parent->children.push_back (shared_from_this ());
}

SceneObjectRef SceneObject::getParent () {
    return parent.lock();
}

void SceneObject::update (UpdatePass::Pass pass) {
    for (int type = 0; type < COMPONENT_TYPE_COUNT; ++type) {
        for (ComponentList::iterator comp = components[type].begin (); comp != components[type].end (); ++comp) {
            if ((*comp)->active && ((*comp)->updatePasses & pass))
                (*comp)->update (pass);
        }
    }
}

void SceneObject::drawGizmos () {
    for (auto pass : GizmoPass::passes) {
        for (int type = 0; type < COMPONENT_TYPE_COUNT; ++type) {
            for (ComponentList::iterator comp = components[type].begin (); comp != components[type].end (); ++comp) {
                if ((*comp)->active && (*comp)->showGizmos && (*comp)->gizmosPass == pass)
                    (*comp)->drawGizmos ();
            }
        }
    }
}