#include "../headers/RAColor.h"
#include <stdio.h>
#include <math.h>

RAColorARGB RAColorARGBMake(RAChannelByte a, RAChannelByte r, RAChannelByte g, RAChannelByte b)
{
	return ((RAColorARGB)a << 24) | ((RAColorARGB)r << 16) | ((RAColorARGB)g << 8) | ((RAColorARGB)b << 0);
}

void RAColorARGBSeparateChannels(RAColorARGB c, RAChannelByte* a, RAChannelByte* r, RAChannelByte* g, RAChannelByte* b)
{
	if (a != NULL)
		*a = (RAChannelByte)((c & 0xFF000000) >> 24);
	if (r != NULL)
		*r = (RAChannelByte)((c & 0x00FF0000) >> 16);
	if (g != NULL)
		*g = (RAChannelByte)((c & 0x0000FF00) >> 8);
	if (b != NULL)
		*b = (RAChannelByte)((c & 0x000000FF) >> 0);
}



RAColor RAColorMake(RAChannel a, RAChannel r, RAChannel g, RAChannel b)
{
	RAColor c = {a, r, g, b};
	return c;
}

RAColorARGB RAColorARGBValue(RAColor c)
{
	RAChannelByte a = (RAChannelByte)floorf(c.a * 255.0f);
	RAChannelByte r = (RAChannelByte)floorf(c.r * 255.0f);
	RAChannelByte g = (RAChannelByte)floorf(c.g * 255.0f);
	RAChannelByte b = (RAChannelByte)floorf(c.b * 255.0f);
	return RAColorARGBMake(a, r, g, b);
}


RAColor RAColorFromARGBValue(RAColorARGB c)
{
	RAChannelByte a;
	RAChannelByte r;
	RAChannelByte g;
	RAChannelByte b;
	RAColorARGBSeparateChannels(c, &a, &r, &g, &b);

	RAChannel alpha = ((RAChannel)a) / 255.0f;
	RAChannel red = ((RAChannel)r) / 255.0f;
	RAChannel green = ((RAChannel)g) / 255.0f;
	RAChannel blue = ((RAChannel)b) / 255.0f;
	return RAColorMake(alpha, red, green, blue);

}

RAColor RAColorScale(RAColor c, float scalar)
{
	return RAColorMake(c.a, c.r * scalar, c.g * scalar, c.b * scalar);
}

RAColor RAColorAdd(RAColor c, RAColor c2)
{
	return RAColorMake(c.a, c.r + c2.r, c.g + c2.g, c.b + c2.b);
}

RAColor RAColorMultiply(RAColor c, RAColor c2)
{
	return RAColorMake(c.a, c.r * c2.r, c.g * c2.g, c.b * c2.b);
}

RAColor RAColorClamp(RAColor c)
{
	return RAColorMake(fmaxf(0.0f, fminf(1.0f, c.a)), fmaxf(0.0f, fminf(1.0f, c.r)), fmaxf(0.0f, fminf(1.0f, c.g)), fmaxf(0.0f, fminf(1.0f, c.b)));
}