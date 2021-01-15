/*
 *	uht_gu example 1
 *	Author : mcidclan
 */

#include "necessary.h"

PSP_MODULE_INFO("bstracker", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(-1024);

Uht *myUht;
u32 *framebuffer;
static pngloader *RedSquare_png;
static void *fRender_Mem, *fbp0, *bwork;

void showCamStream(){

	sceGuDisable(GU_BLEND);
	sceGuTexFilter( GU_LINEAR, GU_LINEAR);
		int quadP[4] = {240, 136, 160, -120};
		unsigned short int bufD[2] = {WW, HH};
		showBuffer((char*)framebuffer, quadP, bufD);
	sceGuTexFilter(GU_NEAREST,GU_NEAREST);
	sceGuEnable(GU_BLEND);
}

vec2i out;
void showRedSquare(){

	getOrtPosition(myUht->pOut, &out);

	int quadP[4] = {
		240+out.x*2, 136+out.y*2,
		68, 68
	};

	unsigned short int bufD[2] = {64, 64};
	showBuffer((char*)(RedSquare_png->pngInfo.data), quadP, bufD);
}

void DrawScene(){

	sceKernelDcacheWritebackInvalidateAll();
	RENDER_begin(fRender_Mem);
		
		if(myUht->fm_isWaiting()) {

			sceGuCopyImage(GU_PSM_8888,0,0,WW,HH,WW,
			(void*)myUht->FB_MPEG,0,0,WW, (void*)framebuffer);
			myUht->fm_nextFrame();
		}

		showCamStream();
		showRedSquare();

	RENDER_end(&fbp0);
}

void ExitAndCleanGame(){

	sceGuFinish();
	sceGuTerm();
	free(fRender_Mem);

	delete myUht;
	free(bwork);

	sceKernelExitGame();
}

int main(int argc, char **argv){

	sceKernelDcacheWritebackInvalidateAll();
	// Direct Render 0x22240 else 0x34E40
	bwork = malloc(0x34E40);

	RedSquare_png = new pngloader();
	RedSquare_png->load("redsquare.png");

	fRender_Mem = memalign(16, 3*1024*1024);
	initGuContext(fRender_Mem, &fbp0);

	framebuffer = (u32*) getStaticVramTexture(WW, HH, GU_PSM_8888);

	myUht = new Uht();
	myUht->fm_activated();

	// Direct Render = false // not use FB_565
	myUht->initBuffer(bwork, false, false);

	vec3i *grid = new vec3i[1];
	grid[0].x = 8; grid[0].y = 8; grid[0].z = 32;
	myUht->setGrid(1, grid);
	myUht->setMinDim(20, 20);

	myUht->initCam();

	u32 key = 0;
	while(key != PSP_CTRL_SELECT){

		key = getKey();
		DrawScene();
	}

	ExitAndCleanGame();

	return 0;
}
