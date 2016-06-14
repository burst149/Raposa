#ifndef RACOLOR_H
#define RACOLOR_H

#include <stdint.h>

// Integer-based colors
typedef uint32_t RAColorARGB;
typedef uint8_t RAChannelByte;

RAColorARGB RAColorARGBMake(RAChannelByte a, RAChannelByte r, RAChannelByte g, RAChannelByte b);
void RAColorARGBSeparateChannels(RAColorARGB c, RAChannelByte* a, RAChannelByte* r, RAChannelByte* g, RAChannelByte* b);

//Float-based colors
typedef float RAChannel;
struct RAColor
{
	RAChannel a;
	RAChannel r;
	RAChannel g;
	RAChannel b;
};
typedef struct RAColor RAColor;

RAColor RAColorMake(RAChannel a, RAChannel r, RAChannel g, RAChannel b);

RAColorARGB RAColorARGBValue(RAColor c);
RAColor RAColorFromARGBValue(RAColorARGB c);
RAColor RAColorScale(RAColor c, float scalar);
RAColor RAColorAdd(RAColor c, RAColor c2);
RAColor RAColorMultiply(RAColor c, RAColor c2);
RAColor RAColorClamp(RAColor c);

#define RAColorClear RAColorMake(0.0f, 0.0f, 0.0f, 0.0f)
#define RAColorBlack RAColorMake(1.0f, 0.0f, 0.0f, 0.0f)
#define RAColorRed RAColorMake(1.0f, 1.0f, 0.0f, 0.0f)
#define RAColorGreen RAColorMake(1.0f, 0.0f, 1.0f, 0.0f)
#define RAColorBlue RAColorMake(1.0f, 0.0f, 0.0f, 1.0f)
#define RAColorWhite RAColorMake(1.0f, 1.0f, 1.0f, 1.0f)

#endif