#include "../headers/RARaster.h"
#include <stdio.h>
#include <stdlib.h>
#define MAXFLOAT 3.40282347e+38F

struct RARaster
{
	int width;
	int height;
	RAColor* pixels;
	float* depths;

};
typedef struct RARaster RARaster;

RARasterRef RARasterMake(int width, int height, RAColor fillColor, float fillDepth)
{
	RARasterRef raster = (RARasterRef)malloc(sizeof(RARaster));
	
	raster->width = width;
	raster->height = height;
	raster->pixels = (RAColor*)malloc(sizeof(RAColor) * raster->width * raster->height);
	raster->depths = (float*)malloc(sizeof(float) * raster->width * raster->height);
	RARasterClear(raster, true, fillColor, true, fillDepth);
	return raster;
}

void RARasterDestroy(RARasterRef raster)
{
	free(raster->pixels);
	free(raster->depths);
	free(raster);
}

int RARasterGetWidth(RARasterRef raster)
{
	return raster->width;
}

int RARasterGetHeight(RARasterRef raster)
{
	return raster->height;
}

RAColor RARasterGetPixel(RARasterRef raster, int x, int y)
{
	return raster->pixels[y * raster->width + x];
}

void RARasterSetPixel(RARasterRef raster, int x, int y, RAColor color)
{
	raster->pixels[y * raster->width + x] = color;
}

void RARasterBlendPixel(RARasterRef raster, int x, int y, RAColor color)
{
	RAColor destinationPixel = RARasterGetPixel(raster, x, y);
	RAColor sourcePixel = color;
	float blend = sourcePixel.a;
	float destinationContribution = 1.0f - blend;
	float sourceContribution = blend;
	color = RAColorAdd(RAColorScale(destinationPixel, destinationContribution), RAColorScale(sourcePixel, sourceContribution));
	RARasterSetPixel(raster, x, y, color);
}

float RARasterGetDepth(RARasterRef raster, int x, int y)
{
	return raster->depths[y * raster->width + x];
}

void RARasterSetDepth(RARasterRef raster, int x, int y, float depth)
{
	raster->depths[y * raster->width + x] = depth;
}

void RARasterClear(RARasterRef raster, bool clearPixels, RAColor clearColor, bool clearDepths, float clearDepth)
{
	if (clearPixels)
		RARasterDrawRectangle(raster, 0, 0, raster->width, raster->height, clearColor);
	if (clearDepths)
	{
		for (int y = 0; y < raster->height; y++)
		{
			for (int x = 0; x < raster->width; x++)
			{
				raster->depths[y * raster->width + x] = clearDepth;
			}
		}
	}
}

void RARasterDrawRectangle(RARasterRef raster, int minX, int minY, int maxX, int maxY, RAColor fillColor)
{
	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			raster->pixels[y * raster->width + x] = fillColor;
		}
	}
}

RARasterRef RARasterMakeFromPPM(char* path)
{
	// Open a file for writing
	FILE* f = fopen(path, "rb");
	if (f==NULL)
	{
		printf("Couldn't open file %s for reading", path);
		return NULL;
	}

	int width;
	int height;
	int depth;

	// Read information required by plain PPM header definition
	fscanf(f, "P3\n");
	fscanf(f, "%i %i\n", &width, &height);
	fscanf(f, "%i\n", &depth);
	
	RARasterRef raster = RARasterMake(width, height, RAColorBlack, MAXFLOAT);

	// Read pixel information
	for (int y = height - 1 ; y >= 0; y--)
	{
		for (int x = 0; x < width; x++)
		{
			int r, g, b;

			fscanf(f, "%i\n", &r);
			fscanf(f, "%i\n", &g);
			fscanf(f, "%i\n", &b);

			RAColor color = RAColorFromARGBValue(RAColorARGBMake(255, r, g, b));
			RARasterSetPixel(raster, x, y, color);
		}
	}
	fclose(f);
	return raster;
}

void RARasterSaveToPPM(RARasterRef raster, char* path)
{
	// Open a file for writing
	FILE* f = fopen(path, "wb");
	if (f==NULL)
	{
		printf("Couldn't open file %s for writing", path);
		return;
	}

	// Write out information required by plain PPM header definition
	fprintf(f, "P3\n");
	fprintf(f, "%i %i\n", raster->width, raster->height);
	fprintf(f, "%i\n", 255);
	
	// Write out pixel information
	for (int y = raster->height - 1 ; y >= 0; y--)
	{
		for (int x = 0; x < raster->width; x++)
		{
			RAColor c = raster->pixels[y * raster->width + x];
			RAColorARGB color = RAColorARGBValue(c);
			RAChannelByte r, g, b;
			RAColorARGBSeparateChannels(color, NULL, &r, &g, &b);
			fprintf(f, "%i\n",r);
			fprintf(f, "%i\n",g);
			fprintf(f, "%i\n",b);
		}
	}
	fclose(f);
}

RARasterRef RARasterMakeFromBMP(char* path)
{

	// Open file for reading
	FILE* file = fopen(path, "rb");
	if (file == NULL)
	{
		printf("Couldn't open file %s for reading", path);
		return NULL;
	}

	// Load header data
	uint16_t magicNumber = 0x4D42;
	uint16_t reserved0 = 0;//0x4D41;
	uint16_t reserved1 = 0;//0x5454;
	uint32_t dataOffset = 0;
	uint32_t infoHeaderSize = 0;
	uint32_t width = 0;
	uint32_t height = 0;
	uint16_t colorPlanes = 0;
	uint16_t bitsPerPixel = 0;
	uint32_t compression = 0;
	uint32_t dataSize = 0;
	uint32_t horizontalResolution = 0;
	uint32_t verticalResolution = 0;
	uint32_t paletteColorCount = 0;
	uint32_t importantPaletteColorCount = 0;
	uint32_t fileSize = 0;

	// Read BMP header (Windows V3, 32bpp)
	fread(&magicNumber , sizeof(magicNumber), 1, file);
	fread(&fileSize , sizeof(fileSize), 1, file);
	fread(&reserved0 , sizeof(reserved0), 1, file);
	fread(&reserved1 , sizeof(reserved1), 1, file);
	fread(&dataOffset , sizeof(dataOffset), 1, file);
	fread(&infoHeaderSize , sizeof(infoHeaderSize), 1, file);
	fread(&width , sizeof(width), 1, file);
	fread(&height , sizeof(height), 1, file);
	fread(&colorPlanes , sizeof(colorPlanes), 1, file);
	fread(&bitsPerPixel , sizeof(bitsPerPixel), 1, file);
	fread(&compression , sizeof(compression), 1, file);
	fread(&dataSize , sizeof(dataSize), 1, file);
	fread(&horizontalResolution , sizeof(horizontalResolution), 1, file);
	fread(&verticalResolution , sizeof(verticalResolution), 1, file);
	fread(&paletteColorCount , sizeof(paletteColorCount), 1, file);
	fread(&importantPaletteColorCount , sizeof(importantPaletteColorCount), 1, file);

	RARasterRef raster = RARasterMake(width, height, RAColorBlack, MAXFLOAT);

	for (int y = 0; y < raster->height; y++)
	{
		for(int x = 0; x < raster->width; x++)
		{
			RAColorARGB color;
			fread(&color, sizeof(color), 1, file);
			RARasterSetPixel(raster, x, y, RAColorFromARGBValue(color));
			//raster->pixels[y * raster->width + x] = RAColorFromARGBValue(color);
		}
	}
	fclose(file);	
	return raster;
}

void RARasterSaveToBMP(RARasterRef raster, char* path)
{
	// Open file for writing
	FILE* file = fopen(path, "wb");
	if (file == NULL)
	{
		printf("Couldn't open file %s for writing", path);
		return;
	}

	// Define header data
	uint16_t magicNumber = 0x4D42;
	uint16_t reserved0 = 0;//0x4D41;
	uint16_t reserved1 = 0;//0x5454;
	uint32_t dataOffset = 54;
	uint32_t infoHeaderSize = 40;
	uint32_t width = raster->width;
	uint32_t height = raster->height;
	uint16_t colorPlanes = 1;
	uint16_t bitsPerPixel = 32;
	uint32_t compression = 0;
	uint32_t dataSize = width * height * bitsPerPixel / 8;
	uint32_t horizontalResolution = 2835;
	uint32_t verticalResolution = 2835;
	uint32_t paletteColorCount = 0;
	uint32_t importantPaletteColorCount = 0;
	uint32_t fileSize = 54 + dataSize;

	// Write BMP header (Windows V3, 32bpp)
	fwrite(&magicNumber , sizeof(magicNumber), 1, file);
	fwrite(&fileSize , sizeof(fileSize), 1, file);
	fwrite(&reserved0 , sizeof(reserved0), 1, file);
	fwrite(&reserved1 , sizeof(reserved1), 1, file);
	fwrite(&dataOffset , sizeof(dataOffset), 1, file);
	fwrite(&infoHeaderSize , sizeof(infoHeaderSize), 1, file);
	fwrite(&width , sizeof(width), 1, file);
	fwrite(&height , sizeof(height), 1, file);
	fwrite(&colorPlanes , sizeof(colorPlanes), 1, file);
	fwrite(&bitsPerPixel , sizeof(bitsPerPixel), 1, file);
	fwrite(&compression , sizeof(compression), 1, file);
	fwrite(&dataSize , sizeof(dataSize), 1, file);
	fwrite(&horizontalResolution , sizeof(horizontalResolution), 1, file);
	fwrite(&verticalResolution , sizeof(verticalResolution), 1, file);
	fwrite(&paletteColorCount , sizeof(paletteColorCount), 1, file);
	fwrite(&importantPaletteColorCount , sizeof(importantPaletteColorCount), 1, file);

	for (int y = 0; y < raster->height; y++)
	{
		for(int x = 0; x < raster->width; x++)
		{
			RAColorARGB color = RAColorARGBValue(RARasterGetPixel(raster, x, y)); //RAColorARGBValue(raster->pixels[y * raster->width + x]);
			fwrite(&color, sizeof(color), 1, file);
		}
	}

	fclose(file);
}
