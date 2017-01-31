#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <stdarg.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

// Access red, green, blue, and alpha component values in a 32-bit unsigned RGBA $

#define RED(pixel)   ( (pixel)>>24)

#define GREEN(pixel) (((pixel)>>16)	& 0x000000FF)

#define BLUE(pixel)  (((pixel)>>8)  	& 0x000000FF)

#define ALPHA(pixel) ( (pixel)		& 0x000000FF)



// Encode a 32-bit unsigned RGBA value from individual

// red, green, blue, and alpha component values.

#define RGBA(r,g,b,a) ((((r) << 24)) | (((g) << 16)) | (((b) << 8)) | ((a)))


void rgba_to_gray(unsigned int* img, unsigned int* out, size_t img_size)
{
	int p, m;
	int i;
	int img_size_1 = img_size - 1;
	for(i = img_size_1; i!=0; i--)
	{
		int p = img[i];
		m = RED(p);
		m += GREEN(p) + BLUE(p);
		m /= 3;
		out[i] = RGBA(m, m, m, ALPHA(p));
	}
}

/*

void optical_flow(int* It1, int* It2, int* out, int w, int h)
{
	int l, i, j, p, q;
	int iter;
	int largeur = 0, hauteur = 0;
	int nb_iterations = 1;
	float alpha = 1.;

	unsigned char *It = NULL, *It1 = NULL;
	float M_u, M_v;
	float *U = NULL, *V = NULL;

	float *Ix = NULL, *Iy = NULL, *Idt = NULL;
	float div_coeff, div_coeff2;

	float pt_c, pt_d, pt_b, pt_bd;
	float pt1_c, pt1_d, pt1_b, pt1_bd;
	float v_Ix, v_Iy;


	U = my_data->U;
	V = my_data->V;
	Ix = my_data->Ix;
	Iy = my_data->Iy;
	Idt = my_data->Idt;


	alpha = my_data->alpha;
	nb_iterations = my_data->nb_iterations;

	largeur = prom_It->sx;
	hauteur = prom_It->sy;


	div_coeff = 1. / 4.;
	for (j = hauteur - 2; j--;) //pourrait au moins faire for(j = ...; j!=0; j--) pour la clarté!
	{
		q = (j + 1) * largeur + 1;
		for (i = largeur - 2; i--;)
		{
			p = i + q;
			pt_c = It[p];
			pt_d = It[p + 1];
			pt_b = It[p + largeur];
			pt_bd = It[p + 1 + largeur];

			pt1_c = It1[p];
			pt1_d = It1[p + 1];
			pt1_b = It1[p + largeur];
			pt1_bd = It1[p + 1 + largeur];


			Iy[p] = ((pt_bd + pt_b - pt_c - pt_d) + (pt1_bd + pt1_b - pt1_c - pt1_d)) * div_coeff;

			Ix[p] = ((pt_d + pt_bd - pt_c - pt_b) + (pt1_d + pt1_bd - pt1_c - pt1_b)) * div_coeff;

			Idt[p] = ((pt1_c + pt1_b + pt1_d + pt1_bd) - (pt_c + pt_b + pt_d + pt_bd)) * div_coeff;
		}
	}

	div_coeff = 1. / 12.;
	div_coeff2 = 1. / 6.;
	for (iter = nb_iterations; iter--;)
	{

		for (j = hauteur - 2; j--;)
		{
			q = (j + 1) * largeur + 1;
			for (i = largeur - 2; i--;)
			{
				p = i + q;
				M_u = (U[p - largeur - 1] + U[p - largeur + 1] + U[p + largeur - 1] + U[p + largeur + 1]) * div_coeff + (U[p - largeur] + U[p - 1] + U[p + 1] + U[p + largeur]) * div_coeff2;

				M_v = (V[p - largeur - 1] + V[p - largeur + 1] + V[p + largeur - 1] + V[p + largeur + 1]) * div_coeff + (V[p - largeur] + V[p - 1] + V[p + 1] + V[p + largeur]) * div_coeff2;

				v_Ix = Ix[p];
				v_Iy = Iy[p];
				U[p] = M_u - ((v_Ix * (v_Ix * M_u + v_Iy * M_v + Idt[p])) / (alpha + v_Ix * v_Ix + v_Iy * v_Iy));
				V[p] = M_v - ((v_Iy * (v_Ix * M_u + v_Iy * M_v + Idt[p])) / (alpha + v_Ix * v_Ix + v_Iy * v_Iy));
			}
		}
	}


	memcpy(It1, It, largeur * hauteur * sizeof(unsigned char));

} */


int main(int argc, char **argv)
{
	std::string infile("img1.png");
	std::string outfile("img_out.png");
	sf::Image img1;
	img1.loadFromFile(infile);
	int w = img1.getSize().x;
	int h = img1.getSize().y;
	int wh = w * h;

	size_t img1_size = w * h * sizeof(unsigned int);

	unsigned int* img1_data = (unsigned int*)malloc(img1_size);
	unsigned int* out_data = (unsigned int*)malloc(img1_size);

	memcpy(img1_data, img1.getPixelsPtr(), img1_size);

	rgba_to_gray(img1_data, out_data, w*h);

	sf::Image out;
	out.create(w, h, (unsigned char*)out_data);
	out.saveToFile(outfile);
//	free(img1_data);
	free(out_data);

	return 0;
}
