#ifndef RAMatrix_H
#define RAMatrix_H

#include "../headers/RAVector.h"
#include <math.h>

struct RAMatrix3
{
	float m11, m21, m31;
	float m12, m22, m32;
	float m13, m23, m33;
};
typedef struct RAMatrix3 RAMatrix3;

RAMatrix3 RAMatrix3Make(
	float m11, float m21, float m31,
	float m12, float m22, float m32,
	float m13, float m23, float m33
);

RAMatrix3 RAMatrix3MakeTranslate(float tx, float ty);
RAMatrix3 RAMatrix3MakeRotate(float theta);
RAMatrix3 RAMatrix3MakeScale(float sx, float sy);

RAMatrix3 RAMatrix3Concatinate(RAMatrix3 my, RAMatrix3 m2);
RAVector3 RAMatrix3MultiplyWithZ0(RAMatrix3 m, RAVector2 v);
RAVector3 RAMatrix3MultiplyWithZ1(RAMatrix3 m, RAVector2 v);
RAVector3 RAMatrix3Multiply(RAMatrix3 m, RAVector3 v);


#define RAMatrix3Identity RAMatrix3Make( \
	1.0f, 0.0f, 0.0f, \
	0.0f, 1.0f, 0.0f, \
	0.0f, 0.0f, 1.0f)



struct RAMatrix4
{
	float m11, m21, m31, m41;
	float m12, m22, m32, m42;
	float m13, m23, m33, m43;
	float m14, m24, m34, m44;
};
typedef struct RAMatrix4 RAMatrix4;

RAMatrix4 RAMatrix4Make(
	float m11, float m21, float m31, float m41,
	float m12, float m22, float m32, float m42,
	float m13, float m23, float m33, float m43,
	float m14, float m24, float m34, float m44
);

RAMatrix4 RAMatrix4MakeTranslate(float tx, float ty, float tz);
RAMatrix4 RAMatrix4MakeRotateX(float theta);
RAMatrix4 RAMatrix4MakeRotateY(float theta);
RAMatrix4 RAMatrix4MakeRotateZ(float theta);
RAMatrix4 RAMatrix4MakeRotate(float theta, RAVector3 a);
RAMatrix4 RAMatrix4MakeScale(float sx, float sy, float sz);

RAMatrix4 RAMatrix4MakeLookAt(RAVector3 eyePoint, RAVector3 spotPoint, RAVector3 upVector);

RAMatrix4 RAMatrix4MakeOrthogonal(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
RAMatrix4 RAMatrix4MakeFrustum(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
RAMatrix4 RAMatrix4MakePerspective(float fieldOfView, float aspect, float minZ, float maxZ);

RAMatrix4 RAMatrix4MakeViewport(float x, float y, float width, float height);

RAMatrix4 RAMatrix4Concatinate(RAMatrix4 my, RAMatrix4 m2);
RAVector3 RAMatrix4MultiplyWithW0(RAMatrix4 m, RAVector3 v);
RAVector3 RAMatrix4MultiplyWithW1(RAMatrix4 m, RAVector3 v);
RAVector4 RAMatrix4Multiply(RAMatrix4 m, RAVector4 v);


#define RAMatrix4Identity RAMatrix4Make( \
	1.0f, 0.0f, 0.0f, 0.0f, \
	0.0f, 1.0f, 0.0f, 0.0f, \
	0.0f, 0.0f, 1.0f, 0.0f, \
	0.0f, 0.0f, 0.0f, 1.0f)

#endif