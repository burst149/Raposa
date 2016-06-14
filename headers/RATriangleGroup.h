#ifndef RATRIANGLEGROUP_H
#define RATRIANGLEGROUP_H

#include "../headers/RAColor.h"
#include "../headers/RARaster.h"
#include "../headers/RATriangle.h"

typedef struct RATriangleGroup* RATriangleGroupRef;

// Constructors
RATriangleGroupRef RATriangleGroupMake(void);
void RATriangleGroupDestroy(RATriangleGroupRef group);

// Triangles
int RATriangleGroupGetTriangleCount(RATriangleGroupRef group);
RATriangle4 RATriangleGroupGetTriangle(RATriangleGroupRef group, int triangleIndex);
void RATriangleGroupAddTriangle(RATriangleGroupRef group, RATriangle4 t);
void RATriangleGroupAddTrianglesFromRawFile(RATriangleGroupRef group, char* path);
void RATriangleGroupAddTrianglesFromOBJFile(RATriangleGroupRef group, char* path);


// Transformations
int RATriangleGroupGetTransformationCount(RATriangleGroupRef group);
RAMatrix4 RATriangleGroupGetTransformation(RATriangleGroupRef group, int transformationIndex);
void RATriangleGroupAddTransformation(RATriangleGroupRef group, RAMatrix4 transformation);

// Colors
RAColor RATriangleGroupGetMaterialColor(RATriangleGroupRef group);
void RATriangleGroupSetMaterialColor(RATriangleGroupRef group, RAColor c);
RARasterRef RATriangleGroupGetTexture(RATriangleGroupRef group);
void RATriangleGroupSetTexture(RATriangleGroupRef group, RARasterRef texture);

#endif
