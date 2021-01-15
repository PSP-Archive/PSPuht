#ifndef UHT_NECESSARY_H
#define UHT_NECESSARY_H

#include <pspkernel.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define YCBCR_RESTRICTIVE_THRESH 0
#define YCBCR_APPROPRIATE_THRESH 1
#define YCBCR_PERMISSIVE_THRESH 2

typedef struct vec2i{
	int x, y;
} vec2i;

typedef struct vec3i{
	int x, y, z;
} vec3i;

typedef struct vec4i{
	int x, y, z, w;
} vec4i;

void getAbsPosition(const vec4i in, vec2i *out);
void getOrtPosition(const vec4i in, vec2i *out);

void getSqrDimension(const vec4i sqr, vec2i *out);
//float evalOpnHand(const vec4i sqr, u8 *buf);
float evalStdHand(const vec4i sqr);

#endif
