#include <Object.h>

Identifier Object::Identifier_Null = 0;

Object::Object (Identifier objectID)
: objectID (objectID)
, active (true)
, nonPersistent (false) {}
