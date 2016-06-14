#include "../headers/RATriangle.h"
#include <stdio.h>

// 2D TRIANGLE
RATriangle2 RATriangle2Make(RAVector2 a, RAVector2 b, RAVector2 c, RAVector2 at, RAVector2 bt, RAVector2 ct, RAVector3 an, RAVector3 bn, RAVector3 cn)
{
	RATriangle2 t = {a, b, c, at, bt, ct, an, bn, cn};
	return t;
}

void RATriangle2CalculateBarycentricCoordinates(RATriangle2 t, RAVector2 v, float* lambda1, float* lambda2, float* lambda3)
{
	// Calculate the barysentric coordinates of the provided point v with respect to the triangle t
	float lambda1Local =
	((t.b.y - t.c.y) * (  v.x - t.c.x) + (t.c.x - t.b.x) * (  v.y - t.c.y)) /
	((t.b.y - t.c.y) * (t.a.x - t.c.x) + (t.c.x - t.b.x) * (t.a.y - t.c.y));
	float lambda2Local =
	((t.c.y - t.a.y) * (  v.x - t.c.x) + (t.a.x - t.c.x) * (  v.y - t.c.y)) /
	((t.b.y - t.c.y) * (t.a.x - t.c.x) + (t.c.x - t.b.x) * (t.a.y - t.c.y));
	float lambda3Local = 1.0f - lambda1Local - lambda2Local;

	// Return values requested
	if (lambda1 != NULL)
		*lambda1 = lambda1Local;
	if (lambda2 != NULL)
		*lambda2 = lambda2Local;
	if (lambda3 != NULL)
		*lambda3 = lambda3Local;
}

RATriangle2 RATriangle2Transform(RATriangle2 t, RAMatrix3 transform)
{
	t.a = RAVector3Project(RAMatrix3MultiplyWithZ1(transform, t.a));
	t.b = RAVector3Project(RAMatrix3MultiplyWithZ1(transform, t.b));
	t.c = RAVector3Project(RAMatrix3MultiplyWithZ1(transform, t.c));
	return t;
}

// 3D TRIANGLE
RATriangle3 RATriangle3Make(RAVector3 a, RAVector3 b, RAVector3 c, RAVector2 at, RAVector2 bt, RAVector2 ct, RAVector3 an, RAVector3 bn, RAVector3 cn)
{
	RATriangle3 t = {a, b, c, at, bt, ct, an, bn, cn};
	return t;
}

RATriangle3 RATriangle3MakeFromTriangle2(RATriangle2 t)
{
	return RATriangle3Make(RAVector3MakeFromVector2(t.a), RAVector3MakeFromVector2(t.b), RAVector3MakeFromVector2(t.c), t.at, t.bt, t.ct, t.an, t.bn, t.cn);
}

RATriangle3 RATriangle3Transform(RATriangle3 t, RAMatrix4 transform)
{
	t.a = RAMatrix4MultiplyWithW1(transform, t.a);
	t.b = RAMatrix4MultiplyWithW1(transform, t.b);
	t.c = RAMatrix4MultiplyWithW1(transform, t.c);
	t.an = RAMatrix4MultiplyWithW0(transform, t.an);
	t.bn = RAMatrix4MultiplyWithW0(transform, t.bn);
	t.cn = RAMatrix4MultiplyWithW0(transform, t.cn);
	return t;
}

RATriangle2 RATriangle3Project(RATriangle3 t)
{
	return RATriangle2Make(RAVector3Project(t.a), RAVector3Project(t.b), RAVector3Project(t.c), t.at, t.bt, t.ct, t.an, t.bn, t.cn);
}

//4D TRIANGLE
RATriangle4 RATriangle4Make(RAVector4 a, RAVector4 b, RAVector4 c, RAVector2 at, RAVector2 bt, RAVector2 ct, RAVector3 an, RAVector3 bn, RAVector3 cn)
{
	RATriangle4 t = {a, b, c, at, bt, ct, an, bn, cn};
	return t;
}

RATriangle4 RATriangle4MakeFromTriangle3(RATriangle3 t)
{
	return RATriangle4Make(RAVector4MakeFromVector3(t.a), RAVector4MakeFromVector3(t.b), RAVector4MakeFromVector3(t.c), t.at, t.bt, t.ct, t.an, t.bn, t.cn);
}

RATriangle4 RATriangle4Transform(RATriangle4 t, RAMatrix4 transform)
{
	t.a = RAMatrix4Multiply(transform, t.a);
	t.b = RAMatrix4Multiply(transform, t.b);
	t.c = RAMatrix4Multiply(transform, t.c);
	t.an = RAMatrix4MultiplyWithW0(transform, t.an);
	t.bn = RAMatrix4MultiplyWithW0(transform, t.bn);
	t.cn = RAMatrix4MultiplyWithW0(transform, t.cn);
	return t;
}

RATriangle4 RATriangle4Homogenize(RATriangle4 t)
{
	return RATriangle4Make(RAVector4Homogenize(t.a), RAVector4Homogenize(t.b), RAVector4Homogenize(t.c), t.at, t.bt, t.ct, t.an, t.bn, t.cn);
}

RATriangle3 RATriangle4Project(RATriangle4 t)
{
	return RATriangle3Make(RAVector4Project(t.a), RAVector4Project(t.b), RAVector4Project(t.c), t.at, t.bt, t.ct, t.an, t.bn, t.cn);
}

RATriangle3 RATriangle4HomogenizeAndProject(RATriangle4 t)
{
	return RATriangle3Make(RAVector4HomogenizeAndProject(t.a), RAVector4HomogenizeAndProject(t.b), RAVector4HomogenizeAndProject(t.c), t.at, t.bt, t.ct, t.an, t.bn, t.cn);
}