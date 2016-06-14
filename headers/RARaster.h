#ifndef RARASTER_H
#define RARASTER_H

#include "../headers/RAColor.h"
#include "../headers/RAVector.h"
#include "../headers/RATriangle.h"
#include <stdbool.h>

// Data Types
typedef struct RARaster* RARasterRef;

// Constructors
RARasterRef RARasterMake(int width, int height, RAColor fillColor, float fillDepth);
void RARasterDestroy(RARasterRef raster);

// Accessors
int RARasterGetWidth(RARasterRef raster);
int RARasterGetHeight(RARasterRef raster);
RAColor RARasterGetPixel(RARasterRef raster, int x, int y);
void RARasterSetPixel(RARasterRef raster, int x, int y, RAColor color);
void RARasterBlendPixel(RARasterRef raster, int x, int y, RAColor color);
float RARasterGetDepth(RARasterRef raster, int x, int y);
void RARasterSetDepth(RARasterRef raster, int x, int y, float depth);


// Methods
void RARasterClear(RARasterRef raster, bool clearPixels, RAColor clearColor, bool clearDepths, float clearDepth);
void RARasterDrawRectangle(RARasterRef raster, int minX, int minY, int maxX, int maxY, RAColor fillColor);
RARasterRef RARasterMakeFromPPM(char* path);
void RARasterSaveToPPM(RARasterRef raster, char* path);
RARasterRef RARasterMakeFromBMP(char* path);
void RARasterSaveToBMP(RARasterRef raster, char* path);

#endif