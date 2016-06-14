#include "../headers/RALight.h"
#include <stdio.h>

RALight RALightMake(RAVector4 position, RAColor color)
{
	RALight light = {position, color};
	return light;
}