#ifndef RATRIANGLE_H
#define RATRIANGLE_H

#include "../headers/RAVector.h"
#include "../headers/RAMatrix.h"

struct RATriangle2
{
	RAVector2 a;
	RAVector2 b;
	RAVector2 c;
	RAVector2 at;
	RAVector2 bt;
	RAVector2 ct;
	RAVector3 an;
	RAVector3 bn;
	RAVector3 cn;
};
typedef struct RATriangle2 RATriangle2;

RATriangle2 RATriangle2Make(RAVector2 a, RAVector2 b, RAVector2 c, RAVector2 at, RAVector2 bt, RAVector2 ct, RAVector3 an, RAVector3 bn, RAVector3 cn);
void RATriangle2CalculateBarycentricCoordinates(RATriangle2 t, RAVector2 v, float* lambda1, float* lambda2, float* lambda3);
RATriangle2 RATriangle2Transform(RATriangle2 t, RAMatrix3 transform);

struct RATriangle3
{
	RAVector3 a;
	RAVector3 b;
	RAVector3 c;
	RAVector2 at;
	RAVector2 bt;
	RAVector2 ct;
	RAVector3 an;
	RAVector3 bn;
	RAVector3 cn;
};
typedef struct RATriangle3 RATriangle3;

RATriangle3 RATriangle3Make(RAVector3 a, RAVector3 b, RAVector3 c, RAVector2 at, RAVector2 bt, RAVector2 ct, RAVector3 an, RAVector3 bn, RAVector3 cn);
RATriangle3 RATriangle3MakeFromTriangle2(RATriangle2 t);
RATriangle3 RATriangle3Transform(RATriangle3 t, RAMatrix4 transform);
RATriangle2 RATriangle3Project(RATriangle3 t);


struct RATriangle4
{
	RAVector4 a;
	RAVector4 b;
	RAVector4 c;
	RAVector2 at;
	RAVector2 bt;
	RAVector2 ct;
	RAVector3 an;
	RAVector3 bn;
	RAVector3 cn;
};
typedef struct RATriangle4 RATriangle4;

RATriangle4 RATriangle4Make(RAVector4 a, RAVector4 b, RAVector4 c, RAVector2 at, RAVector2 bt, RAVector2 ct, RAVector3 an, RAVector3 bn, RAVector3 cn);
RATriangle4 RATriangle4Transform(RATriangle4 t, RAMatrix4 m);
RATriangle4 RATriangle4Homogenize(RATriangle4 t);
RATriangle3 RATriangle4Project(RATriangle4 t);
RATriangle3 RATriangle4HomogenizeAndProject(RATriangle4 t);

#endif