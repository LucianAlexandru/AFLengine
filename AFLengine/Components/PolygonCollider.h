#pragma once

#include <Collider.h>
#include <vector>

#include <boost/serialization/vector.hpp>

#define POLYGON_MAX_VERTICES 50

class PolygonCollider : public Collider {

public:

    PolygonCollider ();

    virtual void update (UpdatePass::Pass pass) override;

    AABoundingBoxRef getAABoundingBox () override;
    ProjectionAxisList getProjectionAxes (Collider* other) override;
    ShapeProjection projectOntoAxis (Vector2 axis) override;

    double momentOfInertiaCoefficient () override;

    VertexList getVertexList () override;

    void setVertices (std::vector<Vector2>& vertices);

private:

    std::vector<Vector2> vertices;
    std::vector<Vector2> startFrameVertices;
    Vector2 startFrameOrigin;

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};


BOOST_CLASS_EXPORT_KEY2 (PolygonCollider, "Polygon Collider")

template <class Archive>
void PolygonCollider::serialize (Archive &ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP (Collider);
    ar & boost::serialization::make_nvp ("Vertices", vertices);
}
