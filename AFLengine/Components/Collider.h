#pragma once

#include <CommonDefines.h>
#include <ComponentDefines.h>
#include <Component.h>

#include <TransformationMatrix.h>
#include <AABoundingBox.h>
#include <Vector2.h>
#include <Color.h>
#include <ColliderExtras.h>


class Collider : public Component {

public:

    virtual ~Collider ();

    enum Shape {
        Shape_Point,
        Shape_Circle,
        Shape_Rectangle,
        Shape_Polygon
    };

    const Shape shape;
    Vector2 positionOffset;

    void drawGizmos () override;

    virtual AABoundingBoxRef getAABoundingBox () = 0;
    virtual ProjectionAxisList getProjectionAxes (Collider* other) = 0;
    virtual ShapeProjection projectOntoAxis (Vector2 axis) = 0;

    virtual Vector2 getGlobalCenterPosition ();

    virtual double momentOfInertiaCoefficient () = 0;

    typedef std::list<Vector2> VertexList;
    virtual VertexList getVertexList () = 0;

protected:

    Collider (Shape shape);

private:

    // serialization
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive &ar, const unsigned int version);

};


BOOST_SERIALIZATION_ASSUME_ABSTRACT (Collider)

template <class Archive>
void Collider::serialize (Archive &ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP (Component);
    ar & boost::serialization::make_nvp ("PositionOffset", positionOffset);
}
