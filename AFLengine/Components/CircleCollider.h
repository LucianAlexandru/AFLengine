#pragma once

#include <Collider.h>

class CircleCollider : public Collider {

public:

    CircleCollider ();

    virtual void update (UpdatePass::Pass pass) override;

    AABoundingBoxRef getAABoundingBox () override;
    ProjectionAxisList getProjectionAxes (Collider* other) override;
    ShapeProjection projectOntoAxis (Vector2 axis) override;

    double momentOfInertiaCoefficient () override;

    VertexList getVertexList () override;

private:

    int triangleCount;
    Vector2 startFrameOrigin;
    double startFrameRadius;

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};


BOOST_CLASS_EXPORT_KEY2 (CircleCollider, "Circle Collider")

template <class Archive>
void CircleCollider::serialize (Archive &ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP (Collider);
    ar & boost::serialization::make_nvp ("TriangleCount", triangleCount);
}