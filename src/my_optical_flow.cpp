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

#define ITER_NB 5

// Access red, green, blue, and alpha component values in a 32-bit unsigned RGBA $

inline unsigned int RED(unsigned int pixel)
{
	return pixel >> 24;
}

inline unsigned int GREEN(unsigned int pixel)
{
	return (pixel >> 16) & 0x000000FF;
}

inline unsigned int BLUE(unsigned int pixel)
{
	return (pixel >> 8) & 0x000000FF;
}

inline unsigned int ALPHA(unsigned int pixel)
{
	return pixel & 0x000000FF;
}


// Encode a 32-bit unsigned RGBA value from individual

// red, green, blue, and alpha component values.

inline unsigned int RGBA(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
 	return ((((r) << 24)) | (((g) << 16)) | (((b) << 8)) | ((a)));
}


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
		out[i] = RGBA(m, m, m, ALPHA(p));
	}
	p = img[0];
	m = RED(p) + GREEN(p) + BLUE(p);
	m /= 3;
	out[0] = RGBA(m, m, m, ALPHA(p));
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
		out[i] = RGBA(p, p, p, p);
	}
	p = img[0];
	out[0] = RGBA(p, p, p, p);
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

	float U[wh + 2 * w + 2];
	float V[wh + 2 * w + 2];
	float Ix[wh];
	float Iy[wh];
	float Idt[wh];
	float iv[wh];

	memset(U, 0, wh * sizeof(float));
	memset(V, 0, wh * sizeof(float));

	nb_iterations = ITER_NB;

	largeur = w;
	hauteur = h;


	//printf("bordel\n");


	div_coeff = 1. / 4.;
	for (p = wh_1; p >= 0 ; p--)
	{
//		printf("%d\n", p);

		pt_c = (float)(It1[p]);
		pt_d = (float)(It1[p + 1]);
		pt_b = (float)(It1[p + largeur]);
		pt_bd = (float)(It1[p + 1 + largeur]);


		pt1_c = (float)(It2[p]);
		pt1_d = (float)(It2[p + 1]);
		pt1_b = (float)(It2[p + largeur]);
		pt1_bd = (float)(It2[p + 1 + largeur]);


		Iy[p] = ((pt_bd + pt_b - pt_c - pt_d) + (pt1_bd + pt1_b - pt1_c - pt1_d)) * div_coeff;

		Ix[p] = ((pt_d + pt_bd - pt_c - pt_b) + (pt1_d + pt1_bd - pt1_c - pt1_b)) * div_coeff;

		Idt[p] = ((pt1_c + pt1_b + pt1_d + pt1_bd) - (pt_c + pt_b + pt_d + pt_bd)) * div_coeff;


	}

	div_coeff = 1. / 12.;
	div_coeff2 = 1. / 6.;
	for (iter = nb_iterations; iter--;)
	{
		for (i = wh_1 ; i >= 0 ; i--)
		{
/*			p = i + largeur + 1;
//			printf("boucle 2 %d\n",p);
//			printf("%d %d\n", U[p], V[p]);

			M_u = (U[p - largeur - 1] + U[p - largeur + 1] + U[p + largeur - 1] + U[p + largeur + 1]) * div_coeff + (U[p - largeur] + U[p - 1] + U[p + 1] + U[p + largeur]) * div_coeff2;
			M_v = (V[p - largeur - 1] + V[p - largeur + 1] + V[p + largeur - 1] + V[p + largeur + 1]) * div_coeff + (V[p - largeur] + V[p - 1] + V[p + 1] + V[p + largeur]) * div_coeff2;

			v_Ix = Ix[i];
			v_Iy = Iy[i];
			U[p] = M_u - ((v_Ix * (v_Ix * M_u + v_Iy * M_v + Idt[i])) / (alpha + v_Ix * v_Ix + v_Iy * v_Iy));
			V[p] = M_v - ((v_Iy * (v_Ix * M_u + v_Iy * M_v + Idt[i])) / (alpha + v_Ix * v_Ix + v_Iy * v_Iy));

*/

		}
	}

/*	int wh_largeur_1 = wh + largeur + 1;
	for (p = largeur + 1; p < wh_largeur_1; p++)
	{
		unsigned char red = (unsigned char) ((U[p] + 0.5f) * 255.f);
		unsigned char blue = (unsigned char)((V[p] + 0.5f) * 255.f);
		out[p] = RGBA(RED(c), GREEN(0), BLUE(blue), ALPHA(255));
		printf("%f, %f\n", U[p], V[p]);
		}
	}*/


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
	unsigned char img1_data[wh + 8];
	unsigned char img2_data[wh + 8];
	unsigned int out_data[wh];

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

	return 0;
}
