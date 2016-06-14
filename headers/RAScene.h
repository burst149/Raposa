#ifndef RASCENE_H
#define RASCENE_H

#include "../headers/RARaster.h"
#include "../headers/RALight.h"
#include "../headers/RATriangleGroup.h"

// Data Types
typedef struct RAScene* RASceneRef;

// Constructors
RASceneRef RASceneLoad(char* path);
void RASceneDestroy(RASceneRef scene);

// Scene view
RAVector3 RASceneGetEyePoint(RASceneRef scene);
void RASceneSetEyePoint(RASceneRef scene, RAVector3 eyePoint);
RAVector3 RASceneGetLookAtPoint(RASceneRef scene);
void RASceneSetLookAtPoint(RASceneRef scene, RAVector3 lookAtPoint);
RAVector3 RASceneGetUpVctor(RASceneRef scene);
void RASceneSetUpVctor(RASceneRef scene, RAVector3 upVector);
float RASceneGetFieldOfViewAngleY(RASceneRef scene);
void RASceneSetFieldOfViewAngleY(RASceneRef scene, float fieldOfViewAngleY);
float RASceneGetAspectRatio(RASceneRef scene);
void RASceneSetAspectRatio(RASceneRef scene, float aspectRatio);
float RASceneGetNearDistance(RASceneRef scene);
void RASceneSetNearDistance(RASceneRef scene, float nearDistance);
float RASceneGetFarDistance(RASceneRef scene);
void RASceneSetFarDistance(RASceneRef scene, float farDistance);

RAColor RASceneAmbientLightColor(RASceneRef scene);
void RASceneSetAmbientLightColor(RASceneRef scene, RAColor ambientLightColor);
int RASceneGetLightCount(RASceneRef scene);
RALight RASceneGetLight(RASceneRef scene, int lightIndex);
RALight RASceneGetLastLight(RASceneRef scene);
void RASceneAddLight(RASceneRef scene, RALight light);

// Scene triangle groups
int RASceneGetTriangleGroupCount(RASceneRef scene);
RATriangleGroupRef RASceneGetTriangleGroup(RASceneRef scene, int triangleGroupIndex);
RATriangleGroupRef RASceneGetLastTriangleGroup(RASceneRef scene);
void RASceneAddBlankTriangleGroup(RASceneRef scene);

// Scene drawing
RARasterRef RASceneGetRaster(RASceneRef scene);
void RASceneDraw(RASceneRef scene);

#endif