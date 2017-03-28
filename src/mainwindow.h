#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <QWizard>
#include <QVector>
#include <QMainWindow>
#include <QtWidgets>
#include <QPushButton>
#include <QObject>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <mutex>

#include "slideritem.h"
#include "plotitem.h"
#include "oscillator.h"
#include "movie.h"
#include "scriptlauncher.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

	public :
		explicit MainWindow(bool* isClosed, QWidget *parent = 0);
		void addMovie(Movie* movie);
		void addOscillators(Oscillator * osc1, Oscillator * osc2);
		void addScriptLauncher(ScriptLauncher* sl); //constant pointer to ScriptLaucher
		void closeEvent(QCloseEvent* event);

		void chooseIpPort(QString ip, int port);
		void chooseJoint(int joint);
		void chooseNaoqiPath(QString path);
		void chooseMainScriptPath(QString path);
		void choosePythonPath(QString path);
		void chooseOscillator(int choice);
		void chooseSharedMemoryPath(QString path);

		~MainWindow();

	private slots :
        	void movieManagement();
		void closeExperience();
		void configNaoConnection();
		void connectNao();

	private:

		int connectToNao();

		bool* _isClosed;
	   	Ui::MainWindow *ui;
		Movie* _movie;
		QPushButton* play;
		bool runningVideo;
		//Oscillator* _oscH; //la vue ne doit pas contenir les osc, par respect d'archi
		//Oscillator* _oscV;
		ScriptLauncher* _sl;
		QString _last_error;
		QMenu* _connection_menu;
		QAction* _config_menu_action;
		QAction* _connect_nao_action;

		QMenuBar* _menuBar;

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
