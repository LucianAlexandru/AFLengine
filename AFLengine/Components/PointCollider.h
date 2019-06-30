#pragma once

#include <Collider.h>

class PointCollider : public Collider {

public:

    PointCollider ();

    virtual void update (UpdatePass::Pass pass) override;

    AABoundingBoxRef getAABoundingBox () override;
    ProjectionAxisList getProjectionAxes (Collider* other) override;
    ShapeProjection projectOntoAxis (Vector2 axis) override;

    double momentOfInertiaCoefficient () override;

    VertexList getVertexList () override;

private:

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};


BOOST_CLASS_EXPORT_KEY2 (PointCollider, "Point Collider")

template <class Archive>
void PointCollider::serialize (Archive &ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP (Collider);
}
