#ifndef RALIGHT_H
#define RALIGHT_H

#include "../headers/RAVector.h"
#include "../headers/RAColor.h"

struct RALight
{
	RAVector4 position;
	RAColor color;
};
typedef struct RALight RALight;

RALight RALightMake(RAVector4 position, RAColor color);

#define RALightOriginWhite RALightMake(RAVector4Zero, RAColorWhite)
#define RALightSunNoonWhite RALightMake(RAVector4Make(0.0f, 100000.0f, 0.0f, 1.0f), RAColorWhite)
#define RALightSunAfterNoonWhite RALightMake(RAVector4Make(-9687.0f, 7986.0f, 8976.0f, 1.0f), RAColorWhite)

#endif