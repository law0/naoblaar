#ifndef UTILITIES_H_INCLUDED
#define UTILITIES_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
/*#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <blc_image/blc_image.h>*/

#define IMAGE_PREFIX "ballo"

#define IMAGE_SUFFIX ".png"

// Access red, green, blue, and alpha component values in a 32-bit unsigned RG$
#define ALPHA(pixel)  ( (pixel)>>24)
#define BLUE(pixel)   (((pixel)>>16) & 0xFF)
#define GREEN(pixel)  (((pixel)>>8)  & 0xFF)
#define RED(pixel)    ( (pixel)      & 0xFF)

// Encode a 32-bit unsigned RGBA value from individual
// red, green, blue, and alpha component values.
#define RGBA(r,g,b,a) ((((a) << 24)) | (((b) << 16)) | (((g) << 8)) | ((r)))



typedef struct float_pair
{
        float x;
        float y;
} float_pair;

typedef struct getNextImage_args
{

} getNextImage_args;

//static blc_array current_image;
//static sf::Image sfml_current_image;


void rgba_to_gray(unsigned int* out, unsigned int* img, size_t img_size);
void char_bgr_to_intensity(unsigned char* out, unsigned char* img, size_t img_size);
void char_bgr_to_rgba(unsigned char* out, unsigned char* img, size_t img_size);
void rgba_to_intensity(unsigned char* out, unsigned int* img, size_t img_size);
void intensity_to_rgba(unsigned int* out, unsigned char* img, size_t img_size);
/*inline void sfml_init_image();
inline void sfml_update_image(unsigned char* img, int i, int wh);
inline void sfml_save_image(unsigned int* out_data);
inline void blaar_init_image();
inline void blaar_update_image(unsigned char* img, int i, int wh);
inline void blaar_save_image(unsigned int* out_data);
void round_robin_sum(float x, float y);*/

#endif //UTILITIES_H_INCLUDED
