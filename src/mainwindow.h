#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <QVector>
#include <QMainWindow>
#include <QtWidgets>
#include <QPushButton>
#include <QObject>
#include <mutex>

#include "slideritem.h"
#include "plotitem.h"
#include "oscillator.h"
#include "movie.h"
#include "savedata.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

	public :
		explicit MainWindow(bool* isClosed, QWidget *parent = 0);
		void addMovie(Movie* movie);
		void addSaveData(Savedata * sd);
		void addOscillators(Oscillator * osc1, Oscillator * osc2);
		void closeEvent(QCloseEvent* event);
		~MainWindow();

	private slots :
        	void movieManagement();
		void closeExperience();
		void dataManagement();
		//void connectToNao();

	private:
		bool* _isClosed;
	   	Ui::MainWindow *ui;
		Movie* _movie;
		Savedata * _sd;
		QPushButton* play;
		QPushButton* data;
		bool runningVideo;
		bool runningData;
//		Oscillator* _oscH;
//		Oscillator* _oscV;
		PlotItem* _plotitemX;
		PlotItem* _plotitemY;
		PlotItem* _plotitemOSCX;
		PlotItem* _plotitemOSCY;
		SliderItem* _coupleSlider1;
		SliderItem* _coupleSlider2;
		SliderItem* _alphaSlider1;
		SliderItem* _alphaSlider2;
		SliderItem* _betaSlider1;
		SliderItem* _betaSlider2;


};

#endif // MAINWINDOW_H
