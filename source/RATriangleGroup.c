#include "../headers/RATriangleGroup.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAXFLOAT 3.40282347e+38F

struct RATriangleGroup
{
	int triangleCount;
	RATriangle4* triangles;

	int transformationCount;
	RAMatrix4* transformations;

	RAColor materialColor;

	RARasterRef texture;
};
typedef struct RATriangleGroup RATriangleGroup;

// Constructors
RATriangleGroupRef RATriangleGroupMake(void)
{
	RATriangleGroupRef group = (RATriangleGroupRef)malloc(sizeof(RATriangleGroup));
	group->triangleCount = 0;
	group->triangles = NULL;
	group->transformationCount = 0;
	group->transformations = NULL;
	group->materialColor = RAColorRed;
	group->texture = NULL;

	return group;
}

void RATriangleGroupDestroy(RATriangleGroupRef group)
{
	free(group->triangles);
	free(group->transformations);
	free(group);
}

// Triangles
int RATriangleGroupGetTriangleCount(RATriangleGroupRef group)
{
	return group->triangleCount;
}

RATriangle4 RATriangleGroupGetTriangle(RATriangleGroupRef group, int triangleIndex)
{
	return group->triangles[triangleIndex];
}

void RATriangleGroupAddTriangle(RATriangleGroupRef group, RATriangle4 t)
{
	group->triangleCount++;
	group->triangles = realloc(group->triangles, sizeof(RATriangle4) * group->triangleCount);
	group->triangles[group->triangleCount - 1] = t;
}

void RATriangleGroupAddTrianglesFromRawFile(RATriangleGroupRef group, char* path)
{
	FILE* f = fopen(path, "r");
	if (f == NULL)
		return;
	float minX = MAXFLOAT;
	float minY = MAXFLOAT;
	float minZ = MAXFLOAT;
	float maxX = -MAXFLOAT;
	float maxY = -MAXFLOAT;
	float maxZ = -MAXFLOAT;

	RATriangle4 t;
	while (fscanf(f, "%f %f %f %f %f %f %f %f %f", &t.a.x, &t.a.y, &t.a.z, &t.b.x, &t.b.y, &t.b.z, &t.c.x, &t.c.y, &t.c.z) == 9)
	{
		t.a.w = 1.0f;
		t.b.w = 1.0f;
		t.c.w = 1.0f;
		t.at = RAVector3Project(RAVector4Project(t.a));
		t.bt = RAVector3Project(RAVector4Project(t.b));
		t.ct = RAVector3Project(RAVector4Project(t.c));
		t.an = RAVector3Normalize(RAVector3Cross(RAVector4Project(RAVector4Subtract(t.b, t.a)), RAVector4Project(RAVector4Subtract(t.c, t.a))));
		t.bn = t.an;
		t.cn = t.an;

		RATriangleGroupAddTriangle(group, t);

		minX = fminf(minX, t.a.x);
		minX = fminf(minX, t.b.x);
		minX = fminf(minX, t.c.x);
		minY = fminf(minY, t.a.y);
		minY = fminf(minY, t.b.y);
		minY = fminf(minY, t.c.y);
		minZ = fminf(minZ, t.a.z);
		minZ = fminf(minZ, t.b.z);
		minZ = fminf(minZ, t.c.z);
		
		maxX = fmaxf(maxX, t.a.x);
		maxX = fmaxf(maxX, t.b.x);
		maxX = fmaxf(maxX, t.c.x);
		maxY = fmaxf(maxY, t.a.y);
		maxY = fmaxf(maxY, t.b.y);
		maxY = fmaxf(maxY, t.c.y);
		maxZ = fmaxf(maxZ, t.a.z);
		maxZ = fmaxf(maxZ, t.b.z);
		maxZ = fmaxf(maxZ, t.c.z);
	}
	fclose(f);

		printf("RAW File: %s (%f, %f, %f)->(%f, %f, %f)\n", path, minX, minY, minZ, maxX, maxY, maxZ);
}

void RATriangleGroupAddTrianglesFromOBJFile(RATriangleGroupRef group, char* path)
{
	FILE* f = fopen(path, "r");
	if (f == NULL)
		return;
	float minX = MAXFLOAT;
	float minY = MAXFLOAT;
	float minZ = MAXFLOAT;
	float maxX = -MAXFLOAT;
	float maxY = -MAXFLOAT;
	float maxZ = -MAXFLOAT;

	// Setup date structures to store vertices, texture cooridnates, and normals
	int vertexCount = 0;
	RAVector4* vertices = NULL;
	int textureCoordinateCount = 0;
	RAVector2* textureCoordinates = NULL;
	int normalCount = 0;
	RAVector3* normals = NULL;

	// Read in vertices, texture coordinates, normals and faces
	//TODO: Negative offset, faces with more than 3 elements, vertex-only faces, and materials
	bool done = false;
	while (done == false)
	{
		// Read the directive on the line, then put it back in the file
		int directive0 = fgetc(f);
		int directive1 = fgetc(f);
		ungetc(directive1, f);
		ungetc(directive0, f);

		// Determine the vertex data type on this line
		if (directive0 == 'v')
		{
			if (directive1 == 't')
			{
				// Capture texture coordinate
				RAVector2 t = RAVector2Zero;
				fscanf(f, "vt %f %f", &t.x, &t.y);

				//Store texture coordinate
				textureCoordinateCount++;
				textureCoordinates = (RAVector2*)realloc(textureCoordinates, sizeof(RAVector2) * textureCoordinateCount);
				textureCoordinates[textureCoordinateCount - 1] = t;
			}
			else if (directive1 == 'n')
			{
				RAVector3 n = RAVector3Zero;
				fscanf(f, "vn %f %f %f", &n.x, &n.y, &n.z);

				normalCount++;
				normals = (RAVector3*)realloc(normals, sizeof(RAVector3) * normalCount);
				normals[normalCount - 1] = n;
			}
			else
			{
				RAVector4 v = RAVector4Zero;
				fscanf(f, "v %f %f %f", &v.x, &v.y, &v.z);
				v.w = 1.0f;

				vertexCount++;
				vertices = (RAVector4*)realloc(vertices, sizeof(RAVector4) * vertexCount);
				vertices[vertexCount - 1] = v;
			}
		}
		else if (directive0 == 'f')
		{
			int avIndex = 0;
			int bvIndex = 0;
			int cvIndex = 0;
			int dvIndex = 0;

			int atIndex = 0;
			int btIndex = 0;
			int ctIndex = 0;
			int dtIndex = 0;

			int anIndex = 0;
			int bnIndex = 0;
			int cnIndex = 0;
			int dnIndex = 0;

			fscanf(f, "f %i/%i/%i %i/%i/%i %i/%i/%i %i/%i/%i",
				&avIndex, &atIndex, &anIndex,
				&bvIndex, &btIndex, &bnIndex,
				&cvIndex, &ctIndex, &cnIndex,
				&dvIndex, &dtIndex, &dnIndex);

			RATriangle4 t;
			if (avIndex != 0 && (avIndex -1) < vertexCount)
				t.a = vertices[avIndex - 1];
			if (bvIndex != 0 && (bvIndex -1) < vertexCount)
				t.b = vertices[bvIndex - 1];
			if (cvIndex != 0 && (cvIndex -1) < vertexCount)
				t.c = vertices[cvIndex - 1];

			if (atIndex != 0 && (atIndex -1) < textureCoordinateCount)
				t.at = textureCoordinates[atIndex - 1];
			if (btIndex != 0 && (btIndex -1) < textureCoordinateCount)
				t.bt = textureCoordinates[btIndex - 1];
			if (ctIndex != 0 && (ctIndex -1) < textureCoordinateCount)
				t.ct = textureCoordinates[ctIndex - 1];

			if (anIndex != 0 && (anIndex -1) < normalCount)
				t.an = normals[anIndex - 1];
			if (bnIndex != 0 && (bnIndex -1) < normalCount)
				t.bn = normals[bnIndex - 1];
			if (cnIndex != 0 && (cnIndex -1) < normalCount)
				t.cn = normals[cnIndex - 1];
			
			t.a.w = 1.0f;
			t.b.w = 1.0f;
			t.c.w = 1.0f;

			//Store the triangle
			RATriangleGroupAddTriangle(group, t);

			minX = fminf(minX, t.a.x);
			minX = fminf(minX, t.b.x);
			minX = fminf(minX, t.c.x);
			minY = fminf(minY, t.a.y);
			minY = fminf(minY, t.b.y);
			minY = fminf(minY, t.c.y);
			minZ = fminf(minZ, t.a.z);
			minZ = fminf(minZ, t.b.z);
			minZ = fminf(minZ, t.c.z);
			
			maxX = fmaxf(maxX, t.a.x);
			maxX = fmaxf(maxX, t.b.x);
			maxX = fmaxf(maxX, t.c.x);
			maxY = fmaxf(maxY, t.a.y);
			maxY = fmaxf(maxY, t.b.y);
			maxY = fmaxf(maxY, t.c.y);
			maxZ = fmaxf(maxZ, t.a.z);
			maxZ = fmaxf(maxZ, t.b.z);
			maxZ = fmaxf(maxZ, t.c.z);

			if (dvIndex != 0 && dtIndex != 0 && dnIndex != 0)
			{
				RATriangle4 t2;
				if (cvIndex != 0 && (cvIndex -1) < vertexCount)
					t2.a = vertices[cvIndex - 1];
				if (dvIndex != 0 && (dvIndex -1) < vertexCount)
					t2.b = vertices[dvIndex - 1];
				if (avIndex != 0 && (avIndex -1) < vertexCount)
					t2.c = vertices[avIndex - 1];

				if (ctIndex != 0 && (ctIndex -1) < textureCoordinateCount)
					t2.at = textureCoordinates[ctIndex - 1];
				if (dtIndex != 0 && (dtIndex -1) < textureCoordinateCount)
					t2.bt = textureCoordinates[dtIndex - 1];
				if (atIndex != 0 && (atIndex -1) < textureCoordinateCount)
					t2.ct = textureCoordinates[atIndex - 1];

				if (cnIndex != 0 && (cnIndex -1) < normalCount)
					t2.an = normals[cnIndex - 1];
				if (dnIndex != 0 && (dnIndex -1) < normalCount)
					t2.bn = normals[dnIndex - 1];
				if (anIndex != 0 && (anIndex -1) < normalCount)
					t2.cn = normals[anIndex - 1];
				
				t2.a.w = 1.0f;
				t2.b.w = 1.0f;
				t2.c.w = 1.0f;

				//Store the triangle
				RATriangleGroupAddTriangle(group, t2);

				minX = fminf(minX, t2.a.x);
				minX = fminf(minX, t2.b.x);
				minX = fminf(minX, t2.c.x);
				minY = fminf(minY, t2.a.y);
				minY = fminf(minY, t2.b.y);
				minY = fminf(minY, t2.c.y);
				minZ = fminf(minZ, t2.a.z);
				minZ = fminf(minZ, t2.b.z);
				minZ = fminf(minZ, t2.c.z);
				
				maxX = fmaxf(maxX, t2.a.x);
				maxX = fmaxf(maxX, t2.b.x);
				maxX = fmaxf(maxX, t2.c.x);
				maxY = fmaxf(maxY, t2.a.y);
				maxY = fmaxf(maxY, t2.b.y);
				maxY = fmaxf(maxY, t2.c.y);
				maxZ = fmaxf(maxZ, t2.a.z);
				maxZ = fmaxf(maxZ, t2.b.z);
				maxZ = fmaxf(maxZ, t2.c.z);
			}


		}
		else
		{
			// Check characters until a newline
			int character;
			do
			{
				character = fgetc(f);
				if (character == EOF)
					done = true;
			}
			while(character != '\n' && character != EOF);
		}
	}

	fclose(f);
	printf("Wavefront OBJ File: %s (%f,%f,%f)->(%f,%f,%f) Vertices:%i TexCoords:%i Normals:%i Triangles:%i \n", path, minX, minY, minZ, maxX, maxY, maxZ, vertexCount, textureCoordinateCount, normalCount, RATriangleGroupGetTriangleCount(group));
}

// Transformations
int RATriangleGroupGetTransformationCount(RATriangleGroupRef group)
{
	return group->transformationCount;
}

RAMatrix4 RATriangleGroupGetTransformation(RATriangleGroupRef group, int transformationIndex)
{
	return group->transformations[transformationIndex];
}

void RATriangleGroupAddTransformation(RATriangleGroupRef group, RAMatrix4 transformation)
{
	group->transformationCount++;
	group->transformations = realloc(group->transformations, sizeof(RAMatrix4) * group->transformationCount);
	group->transformations[group->transformationCount - 1] = transformation;
}

// Colors
RAColor RATriangleGroupGetMaterialColor(RATriangleGroupRef group)
{
	return group->materialColor;
}

void RATriangleGroupSetMaterialColor(RATriangleGroupRef group, RAColor c)
{
	group->materialColor = c;
}

RARasterRef RATriangleGroupGetTexture(RATriangleGroupRef group)
{
	return group->texture;
}

void RATriangleGroupSetTexture(RATriangleGroupRef group, RARasterRef texture)
{
	group->texture = texture;
}