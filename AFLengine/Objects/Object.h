#pragma once

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/export.hpp>

typedef unsigned int Identifier;

class Object {

public:

    static Identifier Identifier_Null;

    const Identifier objectID;
    bool active;
    bool nonPersistent;

    template <template <typename, typename> class ContainerT, typename ValueT>
    static ContainerT<ValueT, std::allocator<ValueT>> filterPersistentObjects (const ContainerT<ValueT, std::allocator<ValueT>>& objects);

protected:

    Object (Identifier objectID);

    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};


template <template <typename, typename> class ContainerT, typename ValueT>
ContainerT<ValueT, std::allocator<ValueT>> Object::filterPersistentObjects (const ContainerT<ValueT, std::allocator<ValueT>>& objects) {

    ContainerT<ValueT, std::allocator<ValueT>> filteredObjects;

    for (ContainerT<ValueT, std::allocator<ValueT>>::const_iterator object = objects.begin (); object != objects.end (); object++) {
        if (!(*object)->nonPersistent) {
            filteredObjects.push_back (*object);
        }
    }

    return filteredObjects;
}


BOOST_SERIALIZATION_ASSUME_ABSTRACT (Object)

template <class Archive>
void Object::serialize (Archive &ar, const unsigned int version) {
    ar & boost::serialization::make_nvp ("ObjectID", const_cast <Identifier&> (objectID));
    ar & boost::serialization::make_nvp ("Active", active);
}
