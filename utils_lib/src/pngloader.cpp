/*
 *	Simple RGBA png loader by mcidclan
 */

#include "pngloader.h"

pngloader::pngloader(void){

	imgPtr = NULL;
}

pngloader::~pngloader(void){

	if(imgPtr != NULL) free(imgPtr);
}

ImgData pngloader::load(string filename){

	png_structp png_ptr;
	png_infop info_ptr;
	ImgData pngOut;
	png_bytep *row_pointers = NULL;

	pngOut.data = NULL;
	pngOut.id = 0;

	png_ptr =
	png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	FILE *fp = NULL;	

	if(!(fp = fopen (filename.c_str(), "rb"))) {
		return pngOut;
	}

	png_init_io (png_ptr, fp);

	info_ptr = png_create_info_struct (png_ptr);
	png_read_info (png_ptr, info_ptr);

	png_get_IHDR (png_ptr, info_ptr,
			(png_uint_32*)&(pngOut.w),
			(png_uint_32*)&(pngOut.h),
			&(pngOut.bitd), &(pngOut.colt),
			NULL, NULL, NULL);	

	imgPtr = (unsigned char __attribute__((aligned(16)))*)
	memalign(16, pngOut.w * pngOut.h * 4 * sizeof(unsigned char));

	pngOut.data = imgPtr;
	row_pointers = new png_bytep[pngOut.h];
	
	png_uint_32 i;
	for (i = 0; i < pngOut.h; ++i){
		row_pointers[i] =
		(png_bytep)(pngOut.data + (pngOut.h - (i + 1)) * pngOut.w * 4);
	}

	png_read_image (png_ptr, row_pointers);
	png_read_end (png_ptr, NULL);
	png_destroy_read_struct (&png_ptr, &info_ptr, NULL);
	delete [] (row_pointers);

	fclose (fp);
	pngInfo = pngOut;

	return pngOut;
}

