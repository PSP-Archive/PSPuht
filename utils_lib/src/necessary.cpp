/*
 *	Necessary file contend utils functions
 *	Author : mcidclan
 */

#include "necessary.h"

ScePspFVector4 quad[NVQ] = {

	{ 0, 1, -1.0f, -1.0f},
	{ 0, 0, -1.0f, 1.0f},
	{ 1, 0, 1.0f, 1.0f},
	{ 1, 0, 1.0f, 1.0f},
	{ 1, 1, 1.0f, -1.0f},
	{ 0, 1, -1.0f, -1.0f}
};

void initGuContext(void *dList, void **fbp0){

	sceGuInit();
	sceGuStart( GU_DIRECT, dList );

		*fbp0 = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);
		void* fbp1 = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);

		sceGuDrawBuffer(GU_PSM_8888, *fbp0, BUF_WIDTH);
		sceGuDispBuffer(BUF_WIDTH, SCR_HEIGHT, fbp1, BUF_WIDTH);

		//sceGuDisable(GU_SCISSOR_TEST);
		//sceGuScissor( 0, 24, SCR_WIDTH, SCR_HEIGHT-24);
		sceGuScissor( 0, 0, SCR_WIDTH, SCR_HEIGHT);
		sceGuEnable( GU_SCISSOR_TEST );
		sceGuClearColor(0x30303030);

		sceGuTexMode(GU_PSM_8888, 0, 1, 0);
		sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
		sceGuTexFilter(GU_NEAREST,GU_NEAREST);	
		sceGuTexWrap(GU_CLAMP, GU_CLAMP);

		sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
		sceGuEnable(GU_BLEND);
		sceGuEnable(GU_TEXTURE_2D);

		sceGuFinish();
	sceGuSync(GU_SYNC_FINISH, GU_SYNC_WHAT_DONE);
	sceGuDisplay(GU_TRUE);
}

unsigned int getKey(){

	SceCtrlData pad;
	sceCtrlReadBufferPositive(&pad, 1);
	return pad.Buttons;
}

bool getKeyUp(unsigned int keyCode, unsigned int &oldPad){

	unsigned int curPad;
	bool ret = !((curPad = getKey()) & keyCode) && (oldPad & keyCode);
	oldPad = curPad;
	return ret;
}

void showText(const char* text, u32 *dtext, u16 h, int X, int Y, int Rx, int Ry){

	pspDebugScreenSetXY(0,0);
	pspDebugScreenPrintf(text);
	int quadP[4] = {X+Rx, Y, Rx, Ry};
	unsigned short int bufD[2] = {512, h};
	showBuffer((char*)dtext, quadP, bufD);
}

void showBuffer(char* buf, int quadParams[], unsigned short int bufDim[],
		u32 color, float angle){

	
	drawTexPlane(	quadParams[0], quadParams[1],
			quadParams[2], quadParams[3],
			buf, bufDim[0], bufDim[1], color, angle );
}

void showPng(	pngloader *png,
		short int x, short int y,
		short int w, short int h,
		u32 color, float angle){

	int quadParams[4] = {abs(w /= 2)+x, abs(h /= 2)+y, w, h};
	unsigned short int bufDim[2] = {png->pngInfo.w, png->pngInfo.h};

	showBuffer((char*)png->pngInfo.data, quadParams, bufDim, color, angle);
}

void drawTexPlane(	int X, int Y, int Rx, int Ry, char *texData,
			unsigned int u, unsigned int v,
			u32 color, float angle){
	int i;

	short uu = u;
	short vv = v;

	short fd = getDigitByRevStep(9, u);
	if((1<<fd)-u) uu = 1<<(fd+1);
 
	fd = getDigitByRevStep(9, v);
	if((1<<fd)-v) vv = 1<<(fd+1);

	TCPVertex __attribute__((aligned(16))) *rect =
	(TCPVertex*) sceGuGetMemory (NVQ * sizeof(TCPVertex));

	float *ff = new float [16];
	rotZMatrix(ff, angle);

	for(i=0; i<NVQ; i+=1) {
		rect[i].u = quad[i].x * u;
		rect[i].v = quad[i].y * v;
		rect[i].color = color;
		rect[i].x = ff[0] * quad[i].z * Rx + ff[1] * quad[i].w * Ry + X;
		rect[i].y = ff[4] * quad[i].z * Rx + ff[5] * quad[i].w * Ry + Y;
		rect[i].z = 0.0f;
	}

	delete [] ff;
	
	sceKernelDcacheWritebackInvalidateAll();
	sceGuTexImage(0, uu, vv, u, (void*)texData);
	sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_BITS|GU_COLOR_8888|
		GU_TRANSFORM_2D|GU_VERTEX_32BITF, NVQ, 0, rect);
}

void RENDER_begin(void *dList){

	sceGuStart( GU_DIRECT, dList );
	sceGuClear(GU_COLOR_BUFFER_BIT);
}

void RENDER_end(void **fbp0, bool swap){

	sceGuFinish();
	sceGuSync(GU_SYNC_FINISH, GU_SYNC_WHAT_DONE);

	if(swap) {
		sceDisplayWaitVblankStart();
		*fbp0 = sceGuSwapBuffers();
	}
}

short int getDigitByRevStep(short int step, unsigned int in){

	if(in & (1<<step) || !step) return step;
		else step = getDigitByRevStep(step-1, in);
	return step;
}

void rotZMatrix(float *m, float rz){

	__asm__ volatile (
		"mtv     %1, S000\n"
		"vcst.s  S001, VFPU_2_PI\n"
		"vmul.s  S000, S000, S001\n"
		"vmidt.q M100\n"
		"vrot.q  C100, S000, [ c, s, 0, 0]\n"
		"vrot.q  C110, S000, [-s, c, 0, 0]\n"
		"sv.q    C100, 0  + %0\n"
		"sv.q    C110, 16 + %0\n"
		"sv.q    C120, 32 + %0\n"
		"sv.q    C130, 48 + %0\n"
		
		: "=m"(*m) : "r"(rz)
	);
}

