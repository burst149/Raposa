#ifndef RAVector_H
#define RAVector_H

#include <stdio.h>
#include <math.h>

struct RAVector2
{
	float x;
	float y;
};
typedef struct RAVector2 RAVector2;

RAVector2 RAVector2Make(float x, float y);
RAVector2 RAVector2Scale(RAVector2 v, float scalar);
RAVector2 RAVector2Add(RAVector2 a, RAVector2 b);
RAVector2 RAVector2Subtract(RAVector2 a, RAVector2 b);


struct RAVector3
{
	float x;
	float y;
	float z;
};
typedef struct RAVector3 RAVector3;

RAVector3 RAVector3Make(float x, float y, float z);
RAVector3 RAVector3MakeFromVector2(RAVector2 v);
RAVector3 RAVector3Scale(RAVector3 v, float scalar);
float RAVector3Magnitude(RAVector3 v);
RAVector3 RAVector3Normalize(RAVector3 v);
RAVector3 RAVector3Add(RAVector3 a, RAVector3 b);
RAVector3 RAVector3Subtract(RAVector3 a, RAVector3 b);
float RAVector3Dot(RAVector3 a, RAVector3 b);
RAVector3 RAVector3Cross(RAVector3 a, RAVector3 b);
RAVector2 RAVector3Project(RAVector3 v);

struct RAVector4
{
	float x;
	float y;
	float z;
	float w;
};
typedef struct RAVector4 RAVector4;

RAVector4 RAVector4Make(float x, float y, float z, float w);
RAVector4 RAVector4MakeFromVector3(RAVector3 v);
RAVector4 RAVector4Scale(RAVector4 v, float scalar);
RAVector4 RAVector4Normalize(RAVector4 v);
RAVector4 RAVector4Add(RAVector4 a, RAVector4 b);
RAVector4 RAVector4Subtract(RAVector4 a, RAVector4 b);
RAVector4 RAVector4CrossXYZ(RAVector4 a, RAVector4 b);
RAVector4 RAVector4Homogenize(RAVector4 v);
RAVector3 RAVector4Project(RAVector4 v);
RAVector3 RAVector4HomogenizeAndProject(RAVector4 v);

#define RAVector2Zero RAVector2Make(0.0f, 0.0f)
#define RAVector3Zero RAVector3Make(0.0f, 0.0f, 0.0f)
#define RAVector4Zero RAVector4Make(0.0f, 0.0f, 0.0f, 0.0f)

#endif