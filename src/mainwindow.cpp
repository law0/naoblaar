#include <mutex>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utilities.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	runningVideo(false)
{
//	setFixedSize(1300, 620);

	//image camera
	QFrame *webcam = new QFrame(this);
	webcam -> setGeometry(0, 0, 640, 480);

	//image mouvement
	QLabel *move = new QLabel(this);
	move -> setGeometry(0, 650, 640, 480);
	//QPixmap *picture = new QPixmap("BOB1.jpg");

	move -> setPixmap(QPixmap("BOB1.jpg"));
	move -> show();
	update();

    //
	play = new QPushButton("Enregistrer", this);
	play -> setGeometry(240, 485, 100, 30);

	QPushButton *pause = new QPushButton("stop", this);
	pause -> setGeometry(345, 485, 80, 30);

	connect(play, SIGNAL(pressed()), this, SLOT(movieManagement()));
	connect(pause, SIGNAL(pressed()), this, SLOT(closeExperience()));

	//printf("salut\n");
	//this->getStream();
}

void MainWindow::movieManagement()
{
	if (runningVideo)
	{
		_movie->stopCapture();
		play->setText("Enregistrer");
		runningVideo = false;
	}
	else
	{
		_movie->startCapture();
		play->setText("Pause");
		runningVideo = true;
	}
}

void MainWindow::closeExperience()
{
	//printf("salut\n");
	_movie->stopCapture();
}

void MainWindow::addMovie(Movie* movie)
{
	 _movie = movie;
}

MainWindow::~MainWindow()
{
    delete ui;
}

