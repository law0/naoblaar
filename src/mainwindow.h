/* Author Lawrencio Rasamoelison, Demailly Benjamin
 * This class is a subclass of QMainWindow
 * it is in charge of the main window of the application
 * Instanciated in launchview.cpp
 * Use Qt
 */

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
#include <mutex>
#include <string>

#include "scriptlauncher.h"
#include "filelinebutton.h"
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
		void addScriptLauncher(ScriptLauncher* sl);
		void addOscillators(Oscillator * osc1, Oscillator * osc2);
		void closeEvent(QCloseEvent* event);

		void chooseIpPort(QString ip, int port);
		void chooseJoint(int joint);
		void chooseNaoqiPath(QString path);
		void chooseMainScriptPath(QString path);
		void choosePythonPath(QString path);
		void chooseOscillator(int choice);
		void chooseSharedMemoryPath(QString path);
		~MainWindow();

		void setSensibilityPtr(float* sensibility);

	private slots :
		void closeExperience();
		void configNaoConnection();
		void connectNao();

		void dataClick();
		void bothClick();
		void movieClick();
		//void dataManagement();
		void saveConfig();

		void sensiChanged(double value);

		//void connectToNao();

	private:
		int connectToNao();
		void disconnectToNao();
		void dataManagement(string name);
		void movieManagement(string name);
		void clickButton(QPushButton * button);
		void breakButton(QPushButton * button);
		string getName(bool isData);

		bool* _isClosed;
	   	Ui::MainWindow *ui;
		Movie* _movie;
		Savedata * _sd;
		QPushButton* play;
		QPushButton* data;
		QPushButton* both;
		QLineEdit* _title;
		FileLineButton* _repository;
		bool runningVideo;
		//Oscillator* _oscH; //la vue ne doit pas contenir les osc, par respect d'archi
		//Oscillator* _oscV;
		ScriptLauncher* _sl;
		QString _last_error;
		QMenu* _connection_menu;
		QAction* _config_menu_action;
		QAction* _connect_nao_action;
		QAction* _save_config_action;

		QMenuBar* _menuBar;

		float* _sensibility;
		QDoubleSpinBox* _sensi_spin;
		bool runningData;
		bool runningBoth;
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
