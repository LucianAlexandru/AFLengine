#pragma once
#include <RigidBody.h>

class CameraBody : public RigidBody {

public:

    CameraBody ();

    virtual void update (UpdatePass::Pass pass) override;

    void addFieldOfView (double deltaFieldOfView);
    double getFieldOfView ();

private:

    double fieldOfView;

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};

typedef std::shared_ptr<CameraBody> CameraBodyRef;


BOOST_CLASS_EXPORT_KEY2 (CameraBody, "Camera Body")

template <class Archive>
void CameraBody::serialize (Archive &ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP (RigidBody);
    ar & boost::serialization::make_nvp ("FieldOfView", fieldOfView);
}
