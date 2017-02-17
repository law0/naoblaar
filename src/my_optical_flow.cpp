#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <stdarg.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "blc_image.h"

#define ITER_NB 2

// Access red, green, blue, and alpha component values in a 32-bit unsigned RG$
#define ALPHA(pixel)  ( (pixel)>>24)
#define BLUE(pixel)   (((pixel)>>16) & 0xFF)
#define GREEN(pixel)  (((pixel)>>8)  & 0xFF)
#define RED(pixel)    ( (pixel)      & 0xFF)

// Encode a 32-bit unsigned RGBA value from individual
// red, green, blue, and alpha component values.
#define RGBA(r,g,b,a) ((((a) << 24)) | (((b) << 16)) | (((g) << 8)) | ((r)))

#define IMAGE_PREFIX "balld"

#define IMAGE_SUFFIX ".png"

#define MAX_IMAGE_COUNT 3

typedef struct float_pair
{
	float x;
	float y;
} float_pair;

typedef struct getNextImage_args
{

} getNextImage_args;

// static because we don't want other module to see these variables
static unsigned int global_WIDTH;
static unsigned int global_HEIGHT;
static unsigned int getNextImage_Counter = 0;
static blc_array current_image;
static sf::Image sfml_current_image;




//----------------------------------------------------------------------------------------------------------------------------------------------------






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





//----------------------------------------------------------------------------------------------------------------------------------------------------







int getNextImage(unsigned char* img)
{
	int w = global_WIDTH;
	int h = global_HEIGHT;
	int wh = w * h;

	int i = getNextImage_Counter;

	if(i > MAX_IMAGE_COUNT)
	{
		return 0;
	}

	sfml_update_image(img, i, wh);

	getNextImage_Counter++;

	return 1;
}


float_pair optical_flow(unsigned int* out, int (*getNextImage)(unsigned char*))
{


	int p;
	int iter;
	int largeur = global_WIDTH;
	int hauteur = global_HEIGHT;
	int nb_iterations = 1;
	float alpha = 1.;
	int wh = largeur * hauteur;
	int wh_1 = wh - 1;

	float M_u, M_v;

	float div_coeff, div_coeff2;

	float pt_c, pt_d, pt_b, pt_bd;
	float pt1_c, pt1_d, pt1_b, pt1_bd;
	float v_Ix, v_Iy;

	float U[wh];
	float V[wh];
	float Ix[wh];
	float Iy[wh];
	float Idt[wh];
//	float iv[wh];

	memset(U, 0, wh * sizeof(float));
	memset(V, 0, wh * sizeof(float));

	nb_iterations = ITER_NB;

	unsigned int p1;
	unsigned int plargeur;
	unsigned int p1largeur;
	unsigned int plargeur_1;
	unsigned int p_largeur_1;
	unsigned int p_largeur;
	unsigned int p_1;


	//could be allocated on the stack, but we ve already done that a lot before
	unsigned char* It1 = (unsigned char*)malloc(sizeof(unsigned char) * wh);
	unsigned char* It2 = (unsigned char*)malloc(sizeof(unsigned char) * wh);
	getNextImage(It1);
	getNextImage(It2);
	int valid_img = 1;

	float_pair fp = {0.f, 0.f};

	div_coeff = 1. / 4.;

	while(valid_img != 0)
	{

		for (p = wh_1; p >= 0 ; p--)
		{

			p1 = p == wh_1 ? p : p + 1;
			plargeur = p <= wh_1 - largeur ? p + largeur : p;
			p1largeur = p <= wh_1 - largeur - 1 ? p + largeur + 1 : p;

			pt_c = (float)(It1[p]);
			pt_d = (float)(It1[p1]);
			pt_b = (float)(It1[plargeur]);
			pt_bd = (float)(It1[p1largeur]);


			pt1_c = (float)(It2[p]);
			pt1_d = (float)(It2[p1]);
			pt1_b = (float)(It2[plargeur]);
			pt1_bd = (float)(It2[p1largeur]);


			//dérivées

			Iy[p] = ((pt_bd + pt_b - pt_c - pt_d) + (pt1_bd + pt1_b - pt1_c - pt1_d)) * div_coeff;

			Ix[p] = ((pt_d + pt_bd - pt_c - pt_b) + (pt1_d + pt1_bd - pt1_c - pt1_b))  * div_coeff;

			Idt[p] = ((pt1_c + pt1_b + pt1_d + pt1_bd) - (pt_c + pt_b + pt_d + pt_bd)) * div_coeff;

		}

		div_coeff = 1. / 12.;
		div_coeff2 = 1. / 6.;
		for (iter = nb_iterations; iter--;)
		{
			for (p = wh_1 ; p >= 0 ; p--)
			{
				p1 = p == wh_1 ? p : p + 1;
				plargeur = p <= wh_1 - largeur ? p + largeur : p;
				p1largeur = p <= wh_1 - largeur - 1 ? p + largeur + 1 : p;
				plargeur_1 = p <= wh_1 - largeur + 1 ? p -1 + largeur : p;
				p_largeur_1 = p >= largeur + 1 ? p - largeur -1 : p;
				p_largeur = p >= largeur ? p - largeur : p;
				p_1 = p >= 1 ? p - 1 : p;


				// Moyennes (U horizontal, V vertical)
				M_u = (U[p_largeur_1] + U[p_largeur + 1] + U[plargeur_1] + U[p1largeur]) * div_coeff + (U[p_largeur] + U[p_1] + U[p1] + U[plargeur])  * div_coeff2;
				M_v = (V[p_largeur_1] + V[p_largeur + 1] + V[plargeur_1] + V[p1largeur]) * div_coeff + (V[p_largeur] + V[p_1] + V[p1] + V[plargeur])  * div_coeff2;

				v_Ix = Ix[p];
				v_Iy = Iy[p];

				//Vecteur vitesses
				U[p] = M_u - ((v_Ix * (v_Ix * M_u + v_Iy * M_v + Idt[p])) / (alpha + v_Ix * v_Ix + v_Iy * v_Iy));
				V[p] = M_v - ((v_Iy * (v_Ix * M_u + v_Iy * M_v + Idt[p])) / (alpha + v_Ix * v_Ix + v_Iy * v_Iy));
			}
		}

		for (p = wh_1; p >=0 ; p--)
		{
			unsigned char red = (unsigned char)( ((U[p] < 0.f ? U[p] : 0.f) + (V[p] < 0.f ? V[p] : 0.f)) * -3.5f);
			unsigned char green = (unsigned char)(fabs(U[p]) * 7.f);
			unsigned char blue = (unsigned char)(fabs(V[p]) * 7.f);
			out[p] += RGBA(red, green, blue, 255);
			fp.x+= U[p];
			fp.y+= V[p];
	//		printf("p: %d, %f, %f\n", p, U[p], V[p]);
	//		printf("%d %d\n", red, blue);
		}

		memcpy(It1, It2, largeur * hauteur * sizeof(unsigned char));
		valid_img = getNextImage(It2);
	}
	free(It1);
	free(It2);

	return fp;

}

int main(int argc, char **argv)
{
	sfml_init_image();
	int wh = global_WIDTH * global_HEIGHT;
	printf("w: %d, h: %d\n", global_WIDTH, global_HEIGHT);

	size_t img_size = wh * sizeof(unsigned int);

	unsigned int* out_data = (unsigned int*)malloc(img_size);
	memset(out_data, 0, img_size);

	float_pair fp = optical_flow(out_data, getNextImage);

	sfml_save_image(out_data);

	free(out_data);

	printf("horizontal = %f, vertical = %f\n", fp.x, fp.y);
	return 0;
}
