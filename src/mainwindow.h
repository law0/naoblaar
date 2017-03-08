#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <cv.h>
#include <highgui.h>

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
    explicit MainWindow(char* frameptr, QWidget *parent = 0);
    void getStream();
    ~MainWindow();

public slots :
        void actionButton();

private:
   	Ui::MainWindow *ui;
	char* _frameptr;


};

#endif // MAINWINDOW_H
