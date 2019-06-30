#pragma once

#include <ComponentDefines.h>
#include <ObjectDefines.h>

#include <Constraint.h>

#include <Vector2.h>

class SpringConstraint : public Constraint {

public:

    SpringConstraint ();

    virtual void update (UpdatePass::Pass pass) override;
    virtual void drawGizmos () override;

public:

    ColliderWeakRef colliderSelf, colliderTarget;
    Vector2 localPivotSelf, localPivotTarget;

    double restLength;
    double springConstant;
    double forceDistribution;

private:

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};

typedef std::shared_ptr<SpringConstraint> SpringConstraintRef;


BOOST_CLASS_EXPORT_KEY2 (SpringConstraint, "Spring Constraint")

template <class Archive>
void SpringConstraint::serialize (Archive &ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP (Constraint);
    ar & boost::serialization::make_nvp ("ColliderSelf", colliderSelf);
    ar & boost::serialization::make_nvp ("LocalPivotSelf", localPivotSelf);
    ar & boost::serialization::make_nvp ("ColliderTarget", colliderTarget);
    ar & boost::serialization::make_nvp ("LocalPivotTarget", localPivotTarget);
    ar & boost::serialization::make_nvp ("RestLength", restLength);
    ar & boost::serialization::make_nvp ("SpringConstant", springConstant);
    ar & boost::serialization::make_nvp ("ForceDistribution", forceDistribution);
}