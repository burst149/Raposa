#include <stdio.h>
#include "../headers/RAScene.h"
#define PI (4*atan(1))
#define MAXFLOAT 3.40282347e+38F

int main (int argc, const char * argv[])
{
	// Load the scene
	RASceneRef scene = RASceneLoad("geometry/teapot_scene.u3d");
	RASceneSetAmbientLightColor(scene, RAColorMake(1.0f, 0.1f, 0.1f, 0.1f));
	RASceneAddLight(scene, RALightSunAfterNoonWhite);
	
	RARasterRef texture = NULL;//RARasterMakeFromBMP("textureC.bmp");

	if (texture != NULL)
		RATriangleGroupSetTexture(RASceneGetLastTriangleGroup(scene), texture);

	if (scene != NULL)
	{
		int steps = 32;
		for (int i = 0; i < steps; i++)
		{
			// Draw the scene
			RARasterClear(RASceneGetRaster(scene), true, RAColorBlack, true, MAXFLOAT);
			RASceneDraw(scene);
			
			// Save the scene
			char path[1024];
			sprintf(path, "%s%i.bmp", "output/image", i);
			RARasterSaveToBMP(RASceneGetRaster(scene), path);

			// Move to the next camera location
			RAVector3 eyePoint = RASceneGetEyePoint(scene);
			RAVector3 lookAtPoint = RASceneGetLookAtPoint(scene);
			RAVector3 look = RAVector3Subtract(eyePoint, lookAtPoint);
			float distance = RAVector3Magnitude(RAVector3Make(look.x, 0.0f, look.z));
			float theta = atan2f(look.z, look.x) + 2.0f * PI / (float)steps;
			eyePoint = RAVector3Add(lookAtPoint, RAVector3Make(distance * cosf(theta), eyePoint.y, distance * sinf(theta)));
			RASceneSetEyePoint(scene, eyePoint);
		}
	}
	

	RARasterRef img;// = RARasterMakeFromPPM("delete.ppm");
	//RARasterSaveToPPM(img, "output.ppm");
	RARasterSaveToBMP(img, "output.bmp");
	char a;
	printf("DONE!");
	scanf("%c",&a);

	return 0;
}