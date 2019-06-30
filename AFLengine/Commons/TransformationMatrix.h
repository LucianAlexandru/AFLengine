#pragma once
#include <CommonDefines.h>
#include <Vector2.h>

class TransformationMatrix {

public:

    static TransformationMatrix Translation (Vector2 translation);
    static TransformationMatrix Rotation (double rotation);
    static TransformationMatrix Scale (Vector2 scale);

public:

    TransformationMatrix ();
    TransformationMatrix (Vector2 translation, double rotation, Vector2 scale);

    Vector2 getTranslation ();
    Vector2 getScaleSqr ();
    double getClampedRotation ();

    TransformationMatrix getInverse ();

    Vector2 delocalizeVector (Vector2 vector);

    Vector2 operator* (const Vector2& point);
    TransformationMatrix operator* (const TransformationMatrix& m);

private:

    double matrix[3][3];

};

