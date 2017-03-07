#include <stdlib.h>
#include <stdio.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "blc_image.h"

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

unsigned int global_WIDTH;
unsigned int global_HEIGHT;
static unsigned int getNextImage_Counter = 0;
static blc_array current_image;
static sf::Image sfml_current_image;


void rgba_to_gray(unsigned int* out, unsigned int* img, size_t img_size)
{
        unsigned int p, m;
        int i;
        int img_size_1 = img_size - 1;
        for(i = img_size_1; i!=0; i--)
        {
                p = img[i];
                m = RED(p) + GREEN(p) + BLUE(p);
                m /= 3;
                out[i] = RGBA(m, m, m, 255);
        }
        p = img[0];
        m = RED(p) + GREEN(p) + BLUE(p);
        m /= 3;
        out[0] = RGBA(m, m, m, 255);
}

void rgba_to_intensity(unsigned char* out, unsigned int* img, size_t img_size)
{
        unsigned int p, m;
        int i;
        int img_size_1 = img_size - 1;
        for(i = img_size_1; i!=0; i--)
        {
                p = img[i];
                m = RED(p) + GREEN(p) + BLUE(p);
                m /= 3;
                out[i] = m;
        }
        p = img[0];
        m = RED(p) + GREEN(p) + BLUE(p);
        m /= 3;
        out[0] = m;
}

void intensity_to_rgba(unsigned int* out, unsigned char* img, size_t img_size)
{
        unsigned int p;
        int i;
        int img_size_1 = img_size - 1;
        for(i = img_size_1; i!=0; i--)
        {
                p = (unsigned int)(img[i]);
                out[i] = RGBA(p, p, p, 255);
        }
        p = img[0];
        out[0] = RGBA(p, p, p, 255);
}

inline void sfml_init_image()
{
        char input_filename[100];
        sprintf(input_filename, "%s%d%s", IMAGE_PREFIX, 0, IMAGE_SUFFIX);
        sfml_current_image.loadFromFile(std::string(input_filename));

        global_WIDTH = sfml_current_image.getSize().x;
        global_HEIGHT = sfml_current_image.getSize().y;
}

inline void sfml_update_image(unsigned char* img, int i, int wh)
{
        char input_filename[100];
        sprintf(input_filename, "%s%d%s", IMAGE_PREFIX, i, IMAGE_SUFFIX);
        sfml_current_image.loadFromFile(std::string(input_filename));

        unsigned int img_data_t[wh];

        memcpy(img_data_t, sfml_current_image.getPixelsPtr(), wh * sizeof(unsigned int));

        rgba_to_intensity(img, img_data_t, wh);
}

inline void sfml_save_image(unsigned int* out_data)
{
        sf::Image out;
        out.create(global_WIDTH, global_HEIGHT, (const unsigned char *)out_data);
        out.saveToFile("img_out.png");
}



inline void blaar_init_image()
{
        char input_filename[100];
        sprintf(input_filename, "%s%d%s", IMAGE_PREFIX, 0, IMAGE_SUFFIX);
        blc_image_def_with_png_file(&current_image, input_filename, 'UIN8', 'Y800');
        current_image.allocate();

        global_WIDTH = current_image.dims[current_image.dims_nb - 2].length;
        global_HEIGHT = current_image.dims[current_image.dims_nb - 1].length;
}

inline void blaar_update_image(unsigned char* img, int i, int wh)
{
        char input_filename[100];
        sprintf(input_filename, "%s%d%s", IMAGE_PREFIX, i, IMAGE_SUFFIX);

        blc_image_update_with_png_file(&current_image, input_filename);

        memcpy(img, current_image.uchars, wh * sizeof(unsigned char));
}

inline void blaar_save_image(unsigned int* out_data)
{
        blc_array image_out;
        blc_image_init(&image_out, 'UIN8', 'RGBA', global_WIDTH, global_HEIGHT);
        memcpy(image_out.uchars, out_data, global_WIDTH * global_HEIGHT * sizeof(unsigned int));
        blc_image_save_png_file(&image_out, "img_out.png");
}


void round_robin_sum(float x, float y)
{

}