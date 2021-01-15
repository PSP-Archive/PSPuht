/*
 *	Simple RGBA png loader by mcidclan
 */

#ifndef H_PNGLOADER
#define H_PNGLOADER

#include "global.h"

typedef struct {
	unsigned int w, h;
	int colt, bitd;
	unsigned char __attribute__((aligned(16))) * data;
	unsigned int id;
} ImgData;

class pngloader {

	public:
		ImgData pngInfo;		
		pngloader(void);
		~pngloader(void);
		ImgData load(string filename);

	private:
		unsigned char *imgPtr;
};

#endif

