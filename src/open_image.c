#include "open_image.h"

void printImgError(const char * str)
{
	printf("%s", str);
}

unsigned img_width(Image *img)
{
	return img->png.width;
}

unsigned img_height(Image *img)
{
	return img->png.height;
}

size_t img_nbpixels(Image *img)
{
	return img->png.width * img->png.height;
}

void img_close(Image *img)
{
	png_close_file(&img->png);
}

void img_free(Image *img)
{
	free(img->pixel_data); free(img);
}


uint32_t *img_pixels(Image *img)  // Get the buffer from an image that already has a buffer.
{
	return img->pixel_data;
}

uint32_t *img_alloc(Image *img) // Allocate a buffer large enough to store pixel data for given image.
{

	size_t num_pixels = img_nbpixels(img);

	return (uint32_t *)malloc(num_pixels * sizeof(uint32_t));

}



// Read an image from a file.

Image *img_read(const char *filename)
{

	Image *img = (Image *)malloc(sizeof(Image));

	if (png_open_file_read(&img->png, filename) != PNG_NO_ERROR)
	{

		printImgError("Couldn't open image\n");

	}



	printf("Reading %s...\n", filename);

	//	printf("colortype=%d\n", img->png.color_type);

	//	printf("bpp=%u\n", img->png.bpp);



	if (img->png.color_type != PNG_TRUECOLOR_ALPHA)
	{

		printImgError("Only true color alpha images supported\n");

	}

	img->pixel_data = img_alloc(img);

	if (png_get_data(&img->png, (unsigned char *)img->pixel_data) != PNG_NO_ERROR)
	{

		printImgError("Could not read image data\n");

	}

	return img;

}



// Save a transformed image.

void img_write(const char *filename, uint32_t *img_data, const Image *orig_img)
{

	png_t out;

	if (png_open_file_write(&out, filename) != PNG_NO_ERROR)
	{

		printImgError("Could not open image to save\n");

	}



	if (png_set_data(&out,

		orig_img->png.width,

		orig_img->png.height,

		orig_img->png.depth,

		orig_img->png.color_type,

		(unsigned char *)img_data) != PNG_NO_ERROR)

	{

		printImgError("Could not save image data\n");

	}

	png_close_file(&out);

}


