#pragma once

#include <Collider.h>

class RectangleCollider : public Collider {

public:

    RectangleCollider ();

    virtual void update (UpdatePass::Pass pass) override;

    AABoundingBoxRef getAABoundingBox () override;
    ProjectionAxisList getProjectionAxes (Collider* other) override;
    ShapeProjection projectOntoAxis (Vector2 axis) override;

    double momentOfInertiaCoefficient () override;

    VertexList getVertexList () override;

private:

    Vector2 startFrameVertices[4];

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};


BOOST_CLASS_EXPORT_KEY2 (RectangleCollider, "Rectangle Collider")

template <class Archive>
void RectangleCollider::serialize (Archive &ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP (Collider);
}
