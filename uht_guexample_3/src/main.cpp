/*
 *	uht_gu example 3
 *	Author : mcidclan
 */

#include "necessary.h"

PSP_MODULE_INFO("uht_gu_3", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(-1024);


 u32* B_Text;
 u32* B_Fram;
void* B_Trac;

void* fbp0;
void* trac_Mem;
void* rend_Mem;
void* text_Mem;

Uht *myUht;
pngloader *RedSquare_png;

bool fbMpeg = false;
static char text_Str[64];


void initTrac(){
	
	myUht = new Uht();
	myUht->fm_activated();
	myUht->initBuffer(trac_Mem, false, false);

	vec3i *grid = new vec3i[2];
	grid[0].x = 10; grid[0].y = 10; grid[0].z = 40;
	grid[1].x = 20; grid[1].y = 20; grid[1].z = 399;
	myUht->setGrid(2, grid);

	myUht->setMinDim(20, 20);
	myUht->setMaxMrg(20, 20);

	//Use this if you are under a yellowish light
	myUht->checkAmbientLight = true;

	myUht->setYcbcrThresh(YCBCR_APPROPRIATE_THRESH);
	myUht->initCam();
}


void initText(){

	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0xFF0000FF);

	RENDER_begin(text_Mem);
	B_Text = (u32*)sceGuGetMemory (512*272*4*2);
	RENDER_end(&fbp0, false);

	pspDebugScreenSetBase(B_Text);
}


void B_WORKToB_Fram(){
	
	u16 i=SIZE_FBu8;
	
	while(i){
		i--;
		if(myUht->B_WORK[i])
			B_Fram[i] = 0xFFFFFFFF;
		else	B_Fram[i] = 0x0;
	}
}


void showCamStream(){

	sceGuDisable(GU_BLEND);
	sceGuTexFilter( GU_LINEAR, GU_LINEAR);
	drawTexPlane(240, 136, 160, -120, (char*)B_Fram, WW, HH);
	sceGuTexFilter(GU_NEAREST,GU_NEAREST);
	sceGuEnable(GU_BLEND);
}


vec2i out_1, out_2;
void showRedSquare(){

	getOrtPosition(myUht->pOut, &out_1);
	getSqrDimension(myUht->pOut, &out_2);

	drawTexPlane(240+out_1.x*2, 136+out_1.y*2, out_2.x, out_2.y,
	(char*)(RedSquare_png->pngInfo.data), 64, 64);
}

void DrawScene(){

	sceKernelDcacheWritebackAll();

	RENDER_begin(rend_Mem);
		
		if(myUht->fm_isWaiting()) {

			if(fbMpeg){
				sceGuCopyImage(GU_PSM_8888,0,0,WW,HH,WW,
				(void*)myUht->FB_MPEG,0,0,WW, (void*)B_Fram);

			} else B_WORKToB_Fram();

			float e = evalStdHand(myUht->pOut);

			sprintf(text_Str, "PSPuht, gu_example 3.\n%3d %3d %3d %3d\n%f",
			myUht->pOut.x, myUht->pOut.y, myUht->pOut.z, myUht->pOut.w, e);
			
			myUht->fm_nextFrame();
		}

		showCamStream();
		showRedSquare();
		showText(text_Str, B_Text, 24, 4, 12, 256, -12);

	RENDER_end(&fbp0);
}


void ExitAndCleanGame(){

	sceGuFinish();
	sceGuTerm();
	free(rend_Mem);

	delete myUht;
	free(trac_Mem);

	//pspDebugScreenSetBase();
	//free(text_Mem);
	
	sceKernelExitGame();
}


int main(int argc, char **argv){

	sceKernelDcacheWritebackInvalidateAll();

	RedSquare_png = new pngloader();
	RedSquare_png->load("redsquare.png");

	//Direct Render 0x22240 else 0x34E40
	trac_Mem = memalign(64, 0x34E40);
	rend_Mem = memalign(16, 3*1024*1024);
	text_Mem = memalign(32, 512*274*4);

	initGuContext(rend_Mem, &fbp0);

	B_Fram = (u32*) getStaticVramTexture(WW, HH, GU_PSM_8888);

	initTrac();
	initText();

	u32 key = 0;

	while(key != PSP_CTRL_SELECT){

		if(getKeyUp(PSP_CTRL_TRIANGLE, key))
			fbMpeg = !fbMpeg;

		DrawScene();
	}

	ExitAndCleanGame();

	return 0;
}


