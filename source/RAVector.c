#include "../headers/RAVector.h"
#include <stdio.h>

// 2D VECTOR
RAVector2 RAVector2Make(float x, float y)
{
	RAVector2 v = {x, y};
	return v;
}

RAVector2 RAVector2Scale(RAVector2 v, float scalar)
{
	v.x = scalar * v.x;
	v.y = scalar * v.y;
	return v;
}

RAVector2 RAVector2Add(RAVector2 a, RAVector2 b)
{
	RAVector2 v = {a.x + b.x, a.y + b.y};
	return v;
}

RAVector2 RAVector2Subtract(RAVector2 a, RAVector2 b)
{
	RAVector2 v = {a.x - b.x, a.y - b.y};
	return v;
}


// 3D VECTOR
RAVector3 RAVector3Make(float x, float y, float z)
{
	RAVector3 v = {x, y, z};
	return v;
}

RAVector3 RAVector3MakeFromVector2(RAVector2 v)
{
	return RAVector3Make(v.x, v.y, 0.0f);
}

RAVector3 RAVector3Scale(RAVector3 v, float scalar)
{
	v.x = scalar * v.x;
	v.y = scalar * v.y;
	v.z = scalar * v.z;
	return v;
}

float RAVector3Magnitude(RAVector3 v)
{
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

RAVector3 RAVector3Normalize(RAVector3 v)
{
	return RAVector3Scale(v, 1.0f / RAVector3Magnitude(v));
}

RAVector3 RAVector3Add(RAVector3 a, RAVector3 b)
{
	RAVector3 v = {a.x + b.x, a.y + b.y, a.z + b.z};
	return v;
}

RAVector3 RAVector3Subtract(RAVector3 a, RAVector3 b)
{
	return RAVector3Make(a.x - b.x, a.y - b.y, a.z - b.z);
}

float RAVector3Dot(RAVector3 a, RAVector3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

RAVector3 RAVector3Cross(RAVector3 a, RAVector3 b)
{
	return RAVector3Make(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

RAVector2 RAVector3Project(RAVector3 v)
{
	return RAVector2Make(v.x, v.y);
}

// 4D Vector
RAVector4 RAVector4Make(float x, float y, float z, float w)
{
	RAVector4 v = {x, y, z, w};
	return v;
}

RAVector4 RAVector4MakeFromVector3(RAVector3 v)
{
	return RAVector4Make(v.x, v.y, v.z, 0.0f);
}

RAVector4 RAVector4Scale(RAVector4 v, float scalar)
{
	v.x = scalar * v.x;
	v.y = scalar * v.y;
	v.z = scalar * v.z;
	v.w = scalar * v.w;
	return v;
}

float RAVector4Magnitude(RAVector4 v)
{
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

RAVector4 RAVector4Normalize(RAVector4 v)
{
	return RAVector4Scale(v, 1.0f / RAVector4Magnitude(v));
}

RAVector4 RAVector4Add(RAVector4 a, RAVector4 b)
{
	RAVector4 v = {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
	return v;
}

RAVector4 RAVector4Subtract(RAVector4 a, RAVector4 b)
{
	return RAVector4Make(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

RAVector4 RAVector4CrossXYZ(RAVector4 a, RAVector4 b)
{
	return RAVector4Make(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x, 1.0f);
}

RAVector4 RAVector4Homogenize(RAVector4 v)
{
	return RAVector4Make(v.x / v.w, v.y / v.w, v.z / v.w, v.w / v.w);
}

RAVector3 RAVector4Project(RAVector4 v)
{
	return RAVector3Make(v.x, v.y, v.z);
}

RAVector3 RAVector4HomogenizeAndProject(RAVector4 v)
{
	return RAVector4Project(RAVector4Homogenize(v));
}