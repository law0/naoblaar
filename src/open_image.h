#ifndef OPEN_IMAGE_H_INCLUDED
#define OPEN_IMAGE_H_INCLUDED

#include "pnglite.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct Image
{

	png_t png;

	uint32_t *pixel_data;

} Image;



// Access red, green, blue, and alpha component values in a 32-bit unsigned RGBA pixel value.

#define ALPHA(pixel)  ( (pixel)>>24)

#define BLUE(pixel)   (((pixel)>>16) & 0xFF)

#define GREEN(pixel)  (((pixel)>>8)  & 0xFF)

#define RED(pixel)    ( (pixel)      & 0xFF)



// Encode a 32-bit unsigned RGBA value from individual

// red, green, blue, and alpha component values.

#define RGBA(r,g,b,a) ((((a) << 24)) | (((b) << 16)) | (((g) << 8)) | ((r)))



#define M 512



#define INTENSITY_LEVELS 20



#define RADIUS 5



#define BLOCK_SIZE 20



unsigned img_width(Image *img);

unsigned img_height(Image *img);

size_t img_nbpixels(Image *img);

void img_close(Image *img);

void img_free(Image *img);



uint32_t *img_pixels(Image *img);

uint32_t *img_alloc(Image *img);

Image *img_read(const char *filename);

void img_write(const char *filename, uint32_t *img_data, const Image *orig_img);

#endif //OPEN_IMAGE_H_INCLUDED
