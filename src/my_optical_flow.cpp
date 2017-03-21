#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <stdarg.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <chrono>

#include <QGraphicsView>
#include <QGraphicsScene>

#include "utilities.h"
#include "streamcatcher.h"
#include "mainwindow.h"
#include "videoitem.h"
#include "oscillator.h"
#include "plotitem.h"

#define ITER_NB 2

#define MAX_IMAGE_COUNT 7

static StreamCatcher * global_Streamcatcher; //Singleton

int getNextImage(unsigned char* img)
{
        int w = global_Streamcatcher->getWidth();
        int h = global_Streamcatcher->getHeight();
        int size = w * h * 3;

	char* frameptr = global_Streamcatcher->getFramePtr();

	std::mutex mtx;
	mtx.lock();
	char_bgr_to_intensity(img, (unsigned char*)frameptr, size);
	mtx.unlock();

        return 1;
}

int nextImage(unsigned char* img)
{
	int w = global_Streamcatcher->getWidth();
	int h = global_Streamcatcher->getHeight();

	global_Streamcatcher->getGRAY(img, w*h);
	return 1;
}


//float_pair optical_flow(unsigned int* out, int (*getNextImage)(unsigned char*))
void optical_flow(int width, int height, Oscillator& oscillator1, Oscillator& oscillator2, int (*getNextImage)(unsigned char*) = nextImage)
{

	int p;
	int iter;
	int largeur = width;
	int hauteur = height;
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
	auto start = std::chrono::high_resolution_clock::now();


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
		/*	unsigned char red = (unsigned char)( ((U[p] < 0.f ? U[p] : 0.f) + (V[p] < 0.f ? V[p] : 0.f)) * -3.5f);
			unsigned char green = (unsigned char)(fabs(U[p]) * 7.f);
			unsigned char blue = (unsigned char)(fabs(V[p]) * 7.f);
			out[p] += RGBA(red, green, blue, 255);*/
			fp.x+= U[p];
			fp.y+= V[p];
	//		printf("p: %d, %f, %f\n", p, U[p], V[p]);
	//		printf("%d %d\n", red, blue);
		}

		memcpy(It1, It2, largeur * hauteur * sizeof(unsigned char));
		valid_img = getNextImage(It2);


		auto finish = std::chrono::high_resolution_clock::now();

		float h = fp.x;
		h *= 0.000001f;

		float v = fp.y;
		v *= 0.000001f;
		if(std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() > 30000000)
		{
			//printf("            \r");
			//printf("H: %f, V: %f", fp.x, fp.y);
			/*if(fp.x > 0)
				printf("<-- ");
			else
				printf("--> ");
			if(fp.y > 0)
				printf("V\n");
			else
				printf("A\n");*/

			float osc1 = oscillator1(h);

			printf("%f %f\n", osc1, h);

			oscillator2(v);

			//std::this_thread::sleep_for(std::chrono::nanoseconds(10000));

			start = std::chrono::high_resolution_clock::now();


		}

		fp.x = 0;
		fp.y = 0;

	}
	free(It1);
	free(It2);



    // operation to be timed ...


//	return fp;

}

void launchView(int argc, char **argv, Oscillator* osc, Oscillator* osc2)
{
	QApplication a(argc, argv);

	MainWindow mw;

	QGraphicsScene scene;

	QGraphicsView view(&scene);

	mw.setCentralWidget(&view);

	VideoItem vi(global_Streamcatcher);

	scene.addItem(&vi);

	PlotItem plotitemX(*osc, 0);
	PlotItem plotitemY(*osc2, 0);
	PlotItem plotitemOSCX(*osc, 1);
	PlotItem plotitemOSCY(*osc2, 1);

	QDockWidget* DWplotitemX = new QDockWidget("Horizontal flow");
	QDockWidget* DWplotitemY = new QDockWidget("Vertical flow");
	QDockWidget* DWplotitemOSCX = new QDockWidget("Horizontal oscillator");
	QDockWidget* DWplotitemOSCY = new QDockWidget("Vertical oscillator");

	DWplotitemX->setWidget(&plotitemX);
	DWplotitemY->setWidget(&plotitemY);
	DWplotitemOSCX->setWidget(&plotitemOSCX);
	DWplotitemOSCY->setWidget(&plotitemOSCY);

	DWplotitemX->setAllowedAreas(Qt::RightDockWidgetArea);
	DWplotitemY->setAllowedAreas(Qt::RightDockWidgetArea);
	DWplotitemOSCX->setAllowedAreas(Qt::RightDockWidgetArea);
	DWplotitemOSCY->setAllowedAreas(Qt::RightDockWidgetArea);

/*	QGraphicsProxyWidget* plotProxyX = scene.addWidget(&plotitemX);
	QGraphicsProxyWidget* plotProxyY = scene.addWidget(&plotitemY);
	QGraphicsProxyWidget* plotProxyOSCX = scene.addWidget(&plotitemOSCX);
	QGraphicsProxyWidget* plotProxyOSCY = scene.addWidget(&plotitemOSCY);*/

/*	plotProxyX->setPos(global_Streamcatcher->getWidth() + 30, 0);
	plotProxyOSCX->setPos(global_Streamcatcher->getWidth() + 30, 125);
	plotProxyY->setPos(global_Streamcatcher->getWidth() + 30, 250);
	plotProxyOSCY->setPos(global_Streamcatcher->getWidth() + 30, 375);*/

	mw.addDockWidget(Qt::RightDockWidgetArea, DWplotitemX);
	mw.addDockWidget(Qt::RightDockWidgetArea, DWplotitemOSCX);
	mw.addDockWidget(Qt::RightDockWidgetArea, DWplotitemY);
	mw.addDockWidget(Qt::RightDockWidgetArea, DWplotitemOSCY);

//	scene.setSceneRect(0, 0, global_Streamcatcher->getWidth() + 320, scene.height());

	view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        view.setFixedSize(global_Streamcatcher->getWidth() + 2, global_Streamcatcher->getHeight() + 2);

	mw.show();

	a.exec();

}


int main(int argc, char **argv)
{

	global_Streamcatcher = StreamCatcher::getInstance();

	int width = global_Streamcatcher->getWidth();
	int height = global_Streamcatcher->getHeight();

	printf("w: %d, h: %d\n", width, height);

	Oscillator oscillator(0.07f, 0.15f);
	Oscillator osc2(0.07f, 0.15f);

	std::thread view_thread(launchView, argc, argv, &oscillator, &osc2);

	optical_flow(width, height, oscillator, osc2);

	getchar();

	view_thread.join();

	StreamCatcher::killInstance();

	return 0;
}
