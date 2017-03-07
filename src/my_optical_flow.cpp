#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <stdarg.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include "utilities.h"

#define ITER_NB 2

#define MAX_IMAGE_COUNT 7

// static because we don't want other module to see these variables
extern unsigned int global_WIDTH;
extern unsigned int global_HEIGHT;


//----------------------------------------------------------------------------------------------------------------------------------------------------

int getNextImage(unsigned char* img)
{
        int w = global_WIDTH;
        int h = global_HEIGHT;
        int wh = w * h;

        int i = getNextImage_Counter;

//      if(i > MAX_IMAGE_COUNT)
//      {
//              return 0;
//      }

        if(i > MAX_IMAGE_COUNT)
        {
                getNextImage_Counter = 0;
                i = 0;
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
		printf("            \r");
		printf("H: %f, V: %f", fp.x, fp.y);
		fp.x = 0;
		fp.y = 0;
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
