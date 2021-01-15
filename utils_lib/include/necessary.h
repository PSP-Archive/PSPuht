#ifndef NECESSARY_H
#define NECESSARY_H

#include "global.h"

#define BUF_WIDTH	512
#define SCR_WIDTH	480
#define SCR_HEIGHT	272
#define WHITE		0xFFFFFFFF
#define NVQ		6

struct TCPVertex{
	float u, v;
	unsigned int color;
	float x, y, z;
};

typedef struct {
	float x, y, z;
} vec3f;

/*typedef struct {
	float x, y;
} vec2f;*/

void initGuContext(void *dList, void **fbp0);
unsigned int getKey();

void showText(const char* text, u32 *dtext, u16 h, int X, int Y, int Rx, int Ry);
void showBuffer(char*, int[], unsigned short int[],
		u32 color = WHITE, float angle = 0.0f);

void drawTexPlane(int X, int Y, int Rx, int Ry, char *texData,
		  unsigned int u, unsigned int v,
		  u32 color = WHITE, float angle = 0.0f);

void RENDER_begin(void*);
void RENDER_end(void **fbp0, bool swap = true);

void showPng(pngloader *png,
		short int x, short int y, short int w, short int h,
		u32 color = WHITE, float angle = 0.0f);

bool getKeyUp(unsigned int keyCode, unsigned int &oldPad);
short int getDigitByRevStep(short int step, unsigned int in);
void rotZMatrix(float *m, float rz);

#endif

