#include "../headers/RAScene.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define PI (4*atan(1))
#define MAXFLOAT 3.40282347e+38F

struct RAScene
{
	RAVector3 eyePoint;
	RAVector3 lookAtPoint;
	RAVector3 upVector;
	float fieldOfViewAngleY;
	float aspectRatio;
	float nearDistance;
	float farDistance;

	int triangleGroupCount;
	RATriangleGroupRef* triangleGroups;

	RAColor ambientLightColor;

	int lightCount;
	RALight* lights;

	RARasterRef raster;
};
typedef struct RAScene RAScene;

// Constructors
RASceneRef RASceneLoad(char* path)
{
	// Open the file and verify it is a 3D UofU scene file
	FILE* f = fopen(path, "r");
	if (f == NULL)
		return NULL;
	char magic1 = fgetc(f);
	char magic2 = fgetc(f);
	if (magic1 != 'U' && magic2 != '3')
	{
		fclose(f);
		return NULL;
	}

	// Create a scene object with default values
	RASceneRef scene = (RASceneRef)malloc(sizeof(RAScene));
	scene->eyePoint = RAVector3Make(0.0f, 0.0f, 0.0f);
	scene->lookAtPoint = RAVector3Make(0.0f, 0.0f, -1.0f);
	scene->upVector = RAVector3Make(0.0f, 1.0f, 0.0f);
	scene->fieldOfViewAngleY = PI / 2;
	scene->aspectRatio = 1.0f;
	scene->nearDistance = 1.0f;
	scene->farDistance = 1000.0f;
	scene->triangleGroupCount = 0;
	scene->triangleGroups = NULL;
	scene->ambientLightColor = RAColorWhite;
	scene->lightCount = 0;
	scene->lights = NULL;
	scene->raster = NULL;

	// Get output raster size and build rasters
	int rasterWidth = 0;
	int rasterHeight = 0;
	fscanf(f, "%i %i", &rasterWidth, &rasterHeight);
	scene->raster = RARasterMake(rasterWidth, rasterHeight, RAColorBlack, MAXFLOAT);

	// Get camera location, direction, and orientation
	fscanf(f, "%f %f %f", &scene->eyePoint.x, &scene->eyePoint.y, &scene->eyePoint.z);
	fscanf(f, "%f %f %f", &scene->lookAtPoint.x, &scene->lookAtPoint.y, &scene->lookAtPoint.z);
	fscanf(f, "%f %f %f", &scene->upVector.x, &scene->upVector.y, &scene->upVector.z);

	// Get projection parameters
	fscanf(f, "%f %f %f %f", &scene->fieldOfViewAngleY, &scene->aspectRatio, &scene->nearDistance, &scene->farDistance);

	// Get geometry and transformations
	int directive;
	do
	{
		directive = '\0';
		while (directive != 'l' && directive !='g' && directive != 'c' && directive != 't' & directive != 'r' && directive != 's' && directive != EOF)
			directive = fgetc(f);
		if (directive == 'g')
		{
			char fileName[1024] = {'\0'};
			fscanf(f, "%s", &fileName);
			printf("Triangle Group File: %s\n", fileName);

			RASceneAddBlankTriangleGroup(scene);
			if (strlen(fileName) > 4 && fileName[strlen(fileName) - 4] == '.' && fileName[strlen(fileName) - 3] == 'o' && fileName[strlen(fileName) - 2] == 'b' && fileName[strlen(fileName) - 1] == 'j')
				RATriangleGroupAddTrianglesFromOBJFile(RASceneGetLastTriangleGroup(scene), fileName);
			else
				RATriangleGroupAddTrianglesFromRawFile(RASceneGetLastTriangleGroup(scene), fileName);
		}
		if (directive == 'c')
		{
			RAColor materialColor = RAColorBlack;
			fscanf(f, "%f %f %f %f", &materialColor.a, &materialColor.r, &materialColor.g, &materialColor.b);
			printf("Material Color: (%f %f %f %f)\n", materialColor.a, materialColor.r, materialColor.g, materialColor.b);

			if (RASceneGetLastTriangleGroup(scene) != NULL)
				RATriangleGroupSetMaterialColor(RASceneGetLastTriangleGroup(scene), materialColor);
		}
		if (directive == 'l')
		{
			float lx = 0.0f;
			float ly = 0.0f;
			float lz = 0.0f;
			float lr = 1.0f;
			float lg = 1.0f;
			float lb = 1.0f;
			fscanf(f, "%f %f %f %f %f %f", &lx, &ly, &lz, &lr, &lg, &lb);
			RASceneAddLight(scene, RALightMake(RAVector4Make(lx, ly, lz, 1.0f), RAColorMake(1.0f, lr, lg, lb)));
			printf("Light: (%f %f %f), (%f %f %f)\n", lx, ly, lz, lr, lg, lb);
		}
		if (directive == 't')
		{
			float tx = 0.0f;
			float ty = 0.0f;
			float tz = 0.0f;
			fscanf(f, "%f %f %f", &tx, &ty, &tz);
			printf("Translate transform (%f %f %f)\n", tx, ty, tz);

			RAMatrix4 t = RAMatrix4MakeTranslate(tx, ty, tz);
			if (RASceneGetLastTriangleGroup(scene) != NULL)
				RATriangleGroupAddTransformation(RASceneGetLastTriangleGroup(scene), t);
		}
		if (directive == 'r')
		{
			float theta = 0.0f;
			RAVector3 a = RAVector3Zero;
			fscanf(f, "%f %f %f %f", &theta, &a.x, &a.y, &a.z);
			printf("Rotate transform (%f), (%f %f %f)\n", theta, a.x, a.y, a.z);

			RAMatrix4 t = RAMatrix4MakeRotate(theta, a);
			if (RASceneGetLastTriangleGroup(scene) != NULL)
				RATriangleGroupAddTransformation(RASceneGetLastTriangleGroup(scene), t);
		}
		if (directive == 's')
		{
			float sx = 0.0f;
			float sy = 0.0f;
			float sz = 0.0f;
			fscanf(f, "%f %f %f", &sx, &sy, &sz);
			printf("Scale transform (%f %f %f)\n", sx, sy, sz);

			RAMatrix4 t = RAMatrix4MakeScale(sx, sy, sz);
			if (RASceneGetLastTriangleGroup(scene) != NULL)
				RATriangleGroupAddTransformation(RASceneGetLastTriangleGroup(scene), t);
		}
	}
	while (directive != EOF);

	//Close file
	fclose(f);

	printf("Loading finished!\n");

	return scene;
}

void RASceneDestroy(RASceneRef scene)
{
	for (int triangleGroupIndex = 0; triangleGroupIndex < RASceneGetTriangleGroupCount(scene); triangleGroupIndex++)
		RATriangleGroupDestroy(RASceneGetTriangleGroup(scene, triangleGroupIndex));
	free(scene->triangleGroups);
	RARasterDestroy(scene->raster);
	free(scene);
}

// Scene view
RAVector3 RASceneGetEyePoint(RASceneRef scene)
{
	return scene->eyePoint;
}

void RASceneSetEyePoint(RASceneRef scene, RAVector3 eyePoint)
{
	scene->eyePoint = eyePoint;
}

RAVector3 RASceneGetLookAtPoint(RASceneRef scene)
{
	return scene->lookAtPoint;
}

void RASceneSetLookAtPoint(RASceneRef scene, RAVector3 lookAtPoint)
{
	scene->lookAtPoint = lookAtPoint;
}

RAVector3 RASceneGetUpVctor(RASceneRef scene)
{
	return scene->upVector;
}

void RASceneSetUpVctor(RASceneRef scene, RAVector3 upVector)
{
	scene->upVector = upVector;
}

// Scene camera parameters
float RASceneGetFieldOfViewAngleY(RASceneRef scene)
{
	return scene->fieldOfViewAngleY;
}

void RASceneSetFieldOfViewAngleY(RASceneRef scene, float fieldOfViewAngleY)
{
	scene->fieldOfViewAngleY = fieldOfViewAngleY;
}

float RASceneGetAspectRatio(RASceneRef scene)
{
	return scene->aspectRatio;
}

void RASceneSetAspectRatio(RASceneRef scene, float aspectRatio)
{
	scene->aspectRatio = aspectRatio;
}

float RASceneGetNearDistance(RASceneRef scene)
{
	return scene->nearDistance;
}

void RASceneSetNearDistance(RASceneRef scene, float nearDistance)
{
	scene->nearDistance = nearDistance;
}

float RASceneGetFarDistance(RASceneRef scene)
{
	return scene->farDistance;
}

void RASceneSetFarDistance(RASceneRef scene, float farDistance)
{
	scene->farDistance = farDistance;
}


// Lights
RAColor RASceneGetAmbientLightColor(RASceneRef scene)
{
	return scene->ambientLightColor;
}

void RASceneSetAmbientLightColor(RASceneRef scene, RAColor ambientLightColor)
{
	scene->ambientLightColor = ambientLightColor;
}

int RASceneGetLightCount(RASceneRef scene)
{
	return scene->lightCount;
}

RALight RASceneGetLight(RASceneRef scene, int lightIndex)
{
	return scene->lights[lightIndex];
}

RALight RASceneGetLastLight(RASceneRef scene)
{
	if (scene->lightCount == 0)
		return RALightOriginWhite;
	else
		return RASceneGetLight(scene, scene->lightCount - 1);
}

void RASceneAddLight(RASceneRef scene, RALight light)
{
	scene->lightCount++;
	scene->lights = (RALight*)realloc(scene->lights, sizeof(RALight) * scene->lightCount);
	scene->lights[scene->lightCount - 1] = light;
}


// Scene triangle groups
int RASceneGetTriangleGroupCount(RASceneRef scene)
{
	return scene->triangleGroupCount;
}

RATriangleGroupRef RASceneGetTriangleGroup(RASceneRef scene, int triangleGroupIndex)
{
	return scene->triangleGroups[triangleGroupIndex];
}

RATriangleGroupRef RASceneGetLastTriangleGroup(RASceneRef scene)
{
	if (scene->triangleGroupCount == 0)
		return NULL;
	else
		return RASceneGetTriangleGroup(scene, scene->triangleGroupCount - 1);
}

void RASceneAddBlankTriangleGroup(RASceneRef scene)
{
	RATriangleGroupRef group = RATriangleGroupMake();
	scene->triangleGroupCount++;
	scene->triangleGroups = (RATriangleGroupRef*)realloc(scene->triangleGroups, sizeof(RATriangleGroupRef) * scene->triangleGroupCount);
	scene->triangleGroups[scene->triangleGroupCount - 1] = group;
}

// Scene drawing
RARasterRef RASceneGetRaster(RASceneRef scene)
{
	return scene->raster;
}

void RASceneDraw(RASceneRef scene)
{
	// Retrieve current time
	clock_t drawBeginTime = clock();

	// Build a viewport transform to go to the raster's coordinate system
	RAMatrix4 viewportTransform = RAMatrix4MakeViewport(0.0f, 0.0f, RARasterGetWidth(scene->raster), RARasterGetHeight(scene->raster));

	// Build a projection transform to view the world through
	RAMatrix4 projectionTransform = RAMatrix4MakePerspective(scene->fieldOfViewAngleY, scene->aspectRatio, scene->nearDistance, scene->farDistance);

	// Build a camera transform to position the world relative to the viewer
	RAMatrix4 cameraTransform = RAMatrix4MakeLookAt(scene->eyePoint, scene->lookAtPoint, scene->upVector);
	// Draw triangle groups to raster
	int triangleCount = 0;
	int trianglesDrawn = 0;
	for (int triangleGroupIndex = 0; triangleGroupIndex < RASceneGetTriangleGroupCount(scene); triangleGroupIndex++)
	{

		RATriangleGroupRef group = RASceneGetTriangleGroup(scene, triangleGroupIndex);

		// Build full model-view transform for group
		RAMatrix4 modelViewTransform = cameraTransform;
		for (int transformIndex = 0; transformIndex < RATriangleGroupGetTransformationCount(group); transformIndex++)
			modelViewTransform = RAMatrix4Concatinate(modelViewTransform, RATriangleGroupGetTransformation(group, transformIndex));

		// Transform and draw triangles
		for (int triangleIndex = 0; triangleIndex < RATriangleGroupGetTriangleCount(group); triangleIndex++)
		{
			RATriangle4 t = RATriangleGroupGetTriangle(group, triangleIndex);
			triangleCount++;

			// Transform by model-view matrix
			t = RATriangle4Transform(t, modelViewTransform);

			// Apply back-face culling
			RAVector3 triangleNormal = RAVector3Cross(RAVector4Project(RAVector4Subtract(t.b, t.a)), RAVector4Project(RAVector4Subtract(t.c, t.a)));
			if (RAVector3Dot(triangleNormal, RAVector4Project(t.a)) > 0.0f)
				continue;

			// Determine surface color
			RAColor surfaceColor = RATriangleGroupGetMaterialColor(group);

			// Calculate ambient lighting contribution (approximates global radient lighting)
			RAColor ambientColorA = RAColorMultiply(surfaceColor, scene->ambientLightColor);
			RAColor ambientColorB = RAColorMultiply(surfaceColor, scene->ambientLightColor);
			RAColor ambientColorC = RAColorMultiply(surfaceColor, scene->ambientLightColor);

			// Calcualte diffuse lighting (approximates material diffuse scattering according to Gourad)
			RAColor diffuseColorA = RAColorBlack;
			RAColor diffuseColorB = RAColorBlack;
			RAColor diffuseColorC = RAColorBlack;

			RAColor specularColorA = RAColorBlack;
			RAColor specularColorB = RAColorBlack;
			RAColor specularColorC = RAColorBlack;

			for (int lightIndex = 0; lightIndex < RASceneGetLightCount(scene); lightIndex++)
			{
				RALight light = RASceneGetLight(scene, lightIndex);
				light.position = RAMatrix4Multiply(cameraTransform, light.position);

				RAVector3 aNormal  = RAVector3Normalize(t.an);
				RAVector3 bNormal  = RAVector3Normalize(t.bn);
				RAVector3 cNormal  = RAVector3Normalize(t.cn);

				RAVector3 aLight = RAVector3Normalize(RAVector4Project(RAVector4Subtract(light.position, t.a)));
				float diffuseContributionA = fmaxf(0.0f, RAVector3Dot(aNormal, aLight));
				diffuseColorA = RAColorAdd(diffuseColorA, RAColorMultiply(surfaceColor, RAColorScale(light.color, diffuseContributionA)));

				if (diffuseContributionA > 0)
				{
					RAVector3 aReflection = RAVector3Subtract(RAVector3Scale(RAVector3Scale(aNormal, (RAVector3Dot(aLight, aNormal))), 2.0f), aLight);
					RAVector3 aView = RAVector3Normalize(RAVector4Project(RAVector4Subtract(RAVector4MakeFromVector3(scene->eyePoint), t.a)));
					float specularContributionA = fmaxf(0.0f, pow(RAVector3Dot(aView, aReflection), 50.0f));
					if (specularContributionA > 0)
						specularColorA = RAColorAdd(specularColorA, RAColorMultiply(surfaceColor, RAColorScale(light.color, specularContributionA)));
				}

				RAVector3 bLight = RAVector3Normalize(RAVector4Project(RAVector4Subtract(light.position, t.b)));
				float diffuseContributionB = fmaxf(0.0f, RAVector3Dot(bNormal, bLight));
				diffuseColorB = RAColorAdd(diffuseColorB, RAColorMultiply(surfaceColor, RAColorScale(light.color, diffuseContributionB)));

				if (diffuseContributionB > 0)
				{
					RAVector3 bReflection = RAVector3Subtract(RAVector3Scale(RAVector3Scale(bNormal, (RAVector3Dot(bLight, bNormal))), 2.0f), bLight);
					RAVector3 bView = RAVector3Normalize(RAVector4Project(RAVector4Subtract(RAVector4MakeFromVector3(scene->eyePoint), t.b)));
					float specularContributionB = fmaxf(0.0f, pow(RAVector3Dot(bView, bReflection), 50.0f));
					if (specularContributionB > 0)
						specularColorB = RAColorAdd(specularColorB, RAColorMultiply(surfaceColor, RAColorScale(light.color, specularContributionB)));
				}

				RAVector3 cLight = RAVector3Normalize(RAVector4Project(RAVector4Subtract(light.position, t.c)));
				float diffuseContributionC = fmaxf(0.0f, RAVector3Dot(cNormal, cLight));
				diffuseColorC = RAColorAdd(diffuseColorC, RAColorMultiply(surfaceColor, RAColorScale(light.color, diffuseContributionC)));
				
				if (diffuseContributionC > 0)
				{
					RAVector3 cReflection = RAVector3Subtract(RAVector3Scale(RAVector3Scale(cNormal, (RAVector3Dot(cLight, cNormal))), 2.0f), cLight);
					RAVector3 cView = RAVector3Normalize(RAVector4Project(RAVector4Subtract(RAVector4MakeFromVector3(scene->eyePoint), t.c)));
					float specularContributionC = fmaxf(0.0f, pow(RAVector3Dot(cView, cReflection), 50.0f));
					if (specularContributionC > 0)
						specularColorC = RAColorAdd(specularColorC, RAColorMultiply(surfaceColor, RAColorScale(light.color, specularContributionC)));
				}

			}

			// Sum contributions from lights and clamp
			RAColor colorA = RAColorClamp(RAColorAdd(ambientColorA, RAColorAdd(diffuseColorA, specularColorA)));
			RAColor colorB = RAColorClamp(RAColorAdd(ambientColorB, RAColorAdd(diffuseColorB, specularColorB)));
			RAColor colorC = RAColorClamp(RAColorAdd(ambientColorC, RAColorAdd(diffuseColorC, specularColorC)));

			// Transform by projection matrix
			t = RATriangle4Transform(t, projectionTransform);

			// Homogenize triangle by dividing by w-coordinates
			t = RATriangle4Homogenize(t);

			// Apply poor-man's clipping
			float eyeMinX = fminf(t.a.x, fminf(t.b.x, t.c.x));
			float eyeMaxX = fmaxf(t.a.x, fmaxf(t.b.x, t.c.x));
			float eyeMinY = fminf(t.a.y, fminf(t.b.y, t.c.y));
			float eyeMaxY = fmaxf(t.a.y, fmaxf(t.b.y, t.c.y));
			float eyeMinZ = fminf(t.a.z, fminf(t.b.z, t.c.z));
			float eyeMaxZ = fmaxf(t.a.z, fmaxf(t.b.z, t.c.z));

			if (eyeMinX < -1.0f || eyeMaxX > 1.0f || eyeMinY < -1.0f || eyeMaxY > 1.0f || eyeMinZ < -1.0f || eyeMaxZ > 1.0f)
			{
				colorA = RAColorRed;
				colorB = RAColorRed;
				colorC = RAColorRed;
				//continue;
			}

			// Transform by viewport matrix
			t = RATriangle4Transform(t, viewportTransform);

			// Rasterize triangle
			// Determine bounds of triangle
			int minX = (int)floorf(fmaxf(0.0f, fminf(t.a.x, fminf(t.b.x, t.c.x))));
			int maxX = (int)ceilf(fminf((float)RARasterGetWidth(scene->raster), fmaxf(t.a.x, fmaxf(t.b.x, t.c.x))));
			int minY = (int)floorf(fmaxf(0.0f, fminf(t.a.y, fminf(t.b.y, t.c.y))));
			int maxY = (int)ceilf(fminf((float)RARasterGetHeight(scene->raster), fmaxf(t.a.y, fmaxf(t.b.y, t.c.y))));
			
			RAVector3 Bp = RAVector4Project(t.a);

			RAVector3 Up = RAVector3Subtract(RAVector4Project(t.b), RAVector4Project(t.a));
			RAVector3 Vp = RAVector3Subtract(RAVector4Project(t.c), RAVector4Project(t.a));

			RAVector3 sZ = RAVector3Cross(Up, Vp);								
			RAVector3 sU = RAVector3Cross(Vp, Bp);	
			RAVector3 sV = RAVector3Cross(Bp, Up);

			sZ.x = Up.y * Vp.z - Vp.y * Up.z;
        	sZ.y = Vp.x * Up.z - Up.x * Vp.z;
        	sZ.z = Up.x * Vp.y - Vp.x * Up.y;

        	sU.x = Vp.y * Bp.z - Bp.y * Vp.z;
        	sU.y = Bp.x * Vp.z - Vp.x * Bp.z;
        	sU.z = Vp.x * Bp.y - Bp.x * Vp.y;

        	sV.x = Bp.y * Up.z - Up.y * Bp.z;
        	sV.y = Up.x * Bp.z - Bp.x * Up.z;
        	sV.z = Bp.x * Up.y - Up.x * Bp.y;

			// Iterate over all the pixels the triangle could cover
			for (int y = minY; y < maxY; y++)
			{
				for (int x = minX; x < maxX; x++)
				{
				
					// Center sampling!
					// PIXEL AND SUB-PIXEL SAMPLING ARE A BIG DEAL!
					RAVector2 v = RAVector2Make((float)x + 0.5f, (float)y + 0.5f);

					// Calculate barycentric coordinates
					float lambda1 = 0.0f;
					float lambda2 = 0.0f;
					float lambda3 = 0.0f;

					RATriangle2CalculateBarycentricCoordinates(RATriangle3Project(RATriangle4Project(t)), v, &lambda1, &lambda2, &lambda3);

					// Validate barycentric coordinates and interpolate color contributions
					if (lambda1 >= 0.0f && lambda2 >= 0.0f && lambda3 >= 0.0f)// && fminf(lambda1, fminf(lambda2, lambda3)) < 0.1f)
					{
						// Perform depth check
						float depth = t.a.z * lambda1 + t.b.z * lambda2 + t.c.z * lambda3;

						if (RARasterGetDepth(scene->raster, x, y) > depth)
						{
							RARasterSetDepth(scene->raster, x, y, depth);

							// Determine material color at pixel
							RAColor color = RAColorBlue;
							if (RATriangleGroupGetTexture(group) != NULL)
							{
								RARasterRef texture = RATriangleGroupGetTexture(group);

								float z = ((1.0f/t.a.z) * lambda1) + ((1.0f/t.b.z) * lambda2) + ((1.0f/t.c.z) * lambda3);

								RAVector2 aContribution = RAVector2Scale(RAVector2Scale(t.at, 1.0f/t.a.z), lambda1);
								RAVector2 bContribution = RAVector2Scale(RAVector2Scale(t.bt, 1.0f/t.b.z), lambda2);
								RAVector2 cContribution = RAVector2Scale(RAVector2Scale(t.ct, 1.0f/t.c.z), lambda3);

								//RAVector2 textureCoordinate = RAVector2Add(aContribution, RAVector2Add(bContribution, cContribution));
								
								//int textureCoordinateX = 
								//(int)floorf(fmaxf(0.0f, fminf((float)RARasterGetWidth(texture), (textureCoordinate.x / z) * (float)RARasterGetWidth(texture))));
								//int textureCoordinateY = 
								//(int)floorf(fmaxf(0.0f, fminf((float)RARasterGetHeight(texture), (1.0f - (textureCoordinate.y / z)) * (float)RARasterGetHeight(texture))));
								
								/*f (0)
								{

									printf("Vertices: %f %f %f %f, %f %f %f %f, %f %f %f %f\n", t.a.x, t.a.y, t.a.z, t.a.w, t.b.x, t.b.y, t.b.z, t.b.w, t.c.x, t.c.y, t.c.z, t.c.w);
									printf("Tex: %f %f, %f %f, %f %f\n", t.at.x, t.at.y, t.bt.x, t.bt.y, t.ct.x, t.ct.y);
									printf("Lambda: %f %f %f\n", lambda1, lambda2, lambda3);
									printf("Contribution: %f %f, %f %f, %f %f\n", aContribution.x, aContribution.y, bContribution.x, bContribution.y, cContribution.x, cContribution.y);
									printf("Z: %f\n", z);
									printf("TexCoord: %f %f\n", textureCoordinate.x, textureCoordinate.y);
									printf("FinalTexCoord: %d %d\n", textureCoordinateX, textureCoordinateY);
								}*/

								float zZ = sZ.z + sZ.y * (float)y + sZ.x * (float)x;
					        	float uZ = sU.z + sU.y * (float)y + sU.x * (float)x;
					        	float vZ = sV.z + sV.y * (float)y + sV.x * (float)x;

					        	int textureCoordinateX = 
								(int)floorf(fmaxf(0.0f, fminf((float)RARasterGetWidth(texture), (vZ/zZ) * (float)RARasterGetWidth(texture))));
								int textureCoordinateY = 
								(int)floorf(fmaxf(0.0f, fminf((float)RARasterGetHeight(texture), (uZ/zZ) * (float)RARasterGetHeight(texture))));
								

								color = RARasterGetPixel(texture, textureCoordinateX, textureCoordinateY);
							}
							else
							{
								RAColor aContribution = RAColorScale(colorA, lambda1);
								RAColor bContribution = RAColorScale(colorB, lambda2);
								RAColor cContribution = RAColorScale(colorC, lambda3);
								color = RAColorAdd(aContribution, RAColorAdd(bContribution, cContribution));
							}

							//TODO: Per-pixel lighting
							RARasterBlendPixel(scene->raster, x, y, color);
						}
					}
				}
			}
			trianglesDrawn++;
		}
	}

	// Report statistics
	printf("Draw %f%% (%i of %i) of triangles in scene in %f seconds\n", (float)trianglesDrawn/(float)triangleCount, trianglesDrawn, triangleCount, (double)(clock() - drawBeginTime) / (double)CLOCKS_PER_SEC);
}