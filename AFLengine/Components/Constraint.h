#pragma once

#include <Component.h>

class Constraint : public Component {

public:

    virtual ~Constraint ();

protected:

    Constraint ();
    Constraint (UpdatePass::Pass updatePass, GizmoPass::Pass gizmoPass);

private:

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};


BOOST_SERIALIZATION_ASSUME_ABSTRACT (Constraint)

template <class Archive>
void Constraint::serialize (Archive &ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP (Component);
}
