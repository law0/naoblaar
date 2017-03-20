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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

	public :
		explicit MainWindow(QWidget *parent = 0);
		void getStream();
		~MainWindow();

	public slots :
        	void actionButton();
		void quit();

	private:
	   	Ui::MainWindow *ui;


};

#endif // MAINWINDOW_H
