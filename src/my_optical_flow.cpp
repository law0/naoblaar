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

#define ITER_NB 50

// Access red, green, blue, and alpha component values in a 32-bit unsigned RG$
#define ALPHA(pixel)  ( (pixel)>>24)
#define BLUE(pixel)   (((pixel)>>16) & 0xFF)
#define GREEN(pixel)  (((pixel)>>8)  & 0xFF)
#define RED(pixel)    ( (pixel)      & 0xFF)

// Encode a 32-bit unsigned RGBA value from individual
// red, green, blue, and alpha component values.
#define RGBA(r,g,b,a) ((((a) << 24)) | (((b) << 16)) | (((g) << 8)) | ((r)))


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


void optical_flow(unsigned int* out, unsigned char* It1, unsigned char* It2, int w, int h)
{

	int i, j, p, q;
	int iter;
	int largeur = 0, hauteur = 0;
	int nb_iterations = 1;
	float alpha = 1.;
	int wh = w * h;
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
	float iv[wh];

	memset(U, 0, wh * sizeof(float));
	memset(V, 0, wh * sizeof(float));

	nb_iterations = ITER_NB;

	largeur = w;
	hauteur = h;

	unsigned int p1;
	unsigned int plargeur;
	unsigned int p1largeur;
	unsigned int plargeur_1;
	unsigned int p_largeur_1;
	unsigned int p_largeur;
	unsigned int p_1;


	//printf("bordel\n");


	div_coeff = 1. / 4.;
	for (p = wh_1; p >= 0 ; p--)
	{
//		printf("%d\n", p);

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
//			printf("boucle 2 %d\n",p);
//			printf("%d %d\n", U[p], V[p]);

			M_u = (U[p_largeur_1] + U[p_largeur + 1] + U[plargeur_1] + U[p1largeur]) * div_coeff + (U[p_largeur] + U[p_1] + U[p1] + U[p_largeur])  * div_coeff2;
			M_v = (V[p_largeur_1] + V[p_largeur + 1] + V[plargeur_1] + V[p1largeur]) * div_coeff  + (V[p_largeur] + V[p_1] + V[p_1] + V[p_largeur])  * div_coeff2;

			v_Ix = Ix[p];
			v_Iy = Iy[p];
			U[p] = M_u - ((v_Ix * (v_Ix * M_u + v_Iy * M_v + Idt[p])) / (alpha + v_Ix * v_Ix + v_Iy * v_Iy));
			V[p] = M_v - ((v_Iy * (v_Ix * M_u + v_Iy * M_v + Idt[p])) / (alpha + v_Ix * v_Ix + v_Iy * v_Iy));


		}
	}

	for (p = wh_1; p >=0 ; p--)
	{
		unsigned char red = (unsigned char)((U[p] * 3.5f) * + 127.f);
		unsigned char blue = (unsigned char)((V[p] * 3.5f) * + 127.f);
		out[p] = RGBA(red, 0, blue, 255);
//		printf("p: %d, %f, %f\n", p, U[p], V[p]);
//		printf("%d %d\n", red, blue);
	}


	//memcpy(It1, It, largeur * hauteur * sizeof(unsigned char));

}


int main(int argc, char **argv)
{
	std::string infile("img1.png");
	std::string infile2("img2.png");
	std::string outfile("img_out.png");
	sf::Image img1;
	sf::Image img2;

	img1.loadFromFile(infile);
	img2.loadFromFile(infile2);

	int w = img1.getSize().x;
	int h = img1.getSize().y;
	int wh = w * h;
	printf("w: %d, h: %d\n", w, h);

	size_t img1_size = w * h * sizeof(unsigned int);

	unsigned int img1_data_t[wh];
	unsigned int img2_data_t[wh];
	unsigned char* img1_data = (unsigned char*)malloc(wh * sizeof(unsigned char));
	unsigned char* img2_data = (unsigned char*)malloc(wh * sizeof(unsigned char));
	unsigned int* out_data = (unsigned int*)malloc(img1_size);
	//Allocating dynamically to avoid stackoverflow on very large images...
	//and avoid moving megabits of stack context

	memcpy(img1_data_t, img1.getPixelsPtr(), img1_size);
	memcpy(img2_data_t, img2.getPixelsPtr(), img1_size);

	rgba_to_intensity(img1_data, img1_data_t, w*h);
	rgba_to_intensity(img2_data, img2_data_t, w*h);

	memset(out_data, 0, img1_size);
	optical_flow(out_data, img1_data, img2_data, w, h);

//	intensity_to_rgba(out_data, img1_data, w*h);

	sf::Image out;
	out.create(w, h, (const unsigned char *)out_data);
	out.saveToFile(outfile);
	free(img1_data);
	free(img2_data);
	free(out_data);

	return 0;
}
