#include <TransformationMatrix.h>

#define _USE_MATH_DEFINES
#include <math.h>

TransformationMatrix TransformationMatrix::Translation (Vector2 translation) {
    TransformationMatrix result;
    result.matrix[0][0] = 1.0;
    result.matrix[0][2] = translation.x;
    result.matrix[1][1] = 1.0;
    result.matrix[1][2] = translation.y;
    result.matrix[2][2] = 1.0;
    return result;
}

TransformationMatrix TransformationMatrix::Rotation (double rotation) {
    double cosAngle = cos (rotation), sinAngle = sin (rotation);
    TransformationMatrix result;
    result.matrix[0][0] = 1.0;
    result.matrix[0][0] = cosAngle;
    result.matrix[0][1] = -sinAngle;
    result.matrix[1][1] = 1.0;
    result.matrix[1][0] = sinAngle;
    result.matrix[1][1] = cosAngle;
    result.matrix[2][2] = 1.0;
    return result;
}

TransformationMatrix TransformationMatrix::Scale (Vector2 scale) {
    TransformationMatrix result;
    result.matrix[0][0] = scale.x;
    result.matrix[1][1] = scale.y;
    result.matrix[2][2] = 1.0;
    return result;
}

TransformationMatrix::TransformationMatrix () {
    memset (matrix, 0, sizeof (matrix));
}

TransformationMatrix::TransformationMatrix (Vector2 translation, double rotation, Vector2 scale) {

    double cosAngle = cos (rotation), sinAngle = sin (rotation);
    matrix[0][0] = scale.x * cosAngle;
    matrix[0][1] = -scale.y * sinAngle;
    matrix[0][2] = translation.x;
    matrix[1][0] = scale.x * sinAngle;
    matrix[1][1] = scale.y * cosAngle;
    matrix[1][2] = translation.y;
    matrix[2][0] = 0.0;
    matrix[2][1] = 0.0;
    matrix[2][2] = 1.0;

}

Vector2 TransformationMatrix::getTranslation () {
    return Vector2 (matrix[0][2], matrix[1][2]);
}

Vector2 TransformationMatrix::getScaleSqr () {
    double xSqr = matrix[0][0] * matrix[0][0] + matrix[1][0] * matrix[1][0];
    double ySqr = matrix[0][1] * matrix[0][1] + matrix[1][1] * matrix[1][1];
    return Vector2 (xSqr, ySqr);
}

double TransformationMatrix::getClampedRotation () {
    if (abs (matrix[0][0]) < FLT_EPSILON)
        return M_PI_2;
    double tan = matrix[1][0] / matrix[0][0];
    return atan (tan);
}

TransformationMatrix TransformationMatrix::getInverse () {

    TransformationMatrix result;
    double determinant = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];

    result.matrix[0][0] = matrix[1][1];
    result.matrix[0][1] = - matrix[0][1];
    result.matrix[0][2] = matrix[0][1] * matrix[1][2] - matrix[0][2] * matrix[1][1];
    result.matrix[1][0] = - matrix[1][0];
    result.matrix[1][1] = matrix[0][0];
    result.matrix[1][2] = matrix[0][2] * matrix[1][0] - matrix[0][0] * matrix[1][2];
    result.matrix[2][2] = 1.0;

    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 3; j++)
            result.matrix[i][j] /= determinant;

    return result;
}

Vector2 TransformationMatrix::delocalizeVector (Vector2 vector) {
    return (getInverse () * Translation (vector) * (*this)).getTranslation ();
}

Vector2 TransformationMatrix::operator* (const Vector2& point) {
    return Vector2 (matrix[0][0] * point.x + matrix[0][1] * point.y + matrix[0][2],
                    matrix[1][0] * point.x + matrix[1][1] * point.y + matrix[1][2]);
}

TransformationMatrix TransformationMatrix::operator* (const TransformationMatrix& m) {
    TransformationMatrix result;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k) {
                result.matrix[i][j] += matrix[i][k] * m.matrix[k][j];
            }
    return result;
}

