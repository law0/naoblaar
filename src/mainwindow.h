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

#include "movie.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

	public :
		explicit MainWindow(QWidget *parent = 0);
		void addMovie(Movie* movie);
		~MainWindow();

	private slots :
        	void movieManagement();
		void closeExperience();

	private:
	   	Ui::MainWindow *ui;
		Movie* _movie;
		QPushButton* play;
		bool runningVideo;


};

#endif // MAINWINDOW_H
