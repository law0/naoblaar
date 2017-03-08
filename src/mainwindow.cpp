#include <mutex>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utilities.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	setFixedSize(1300, 520);

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
	QPushButton *play = new QPushButton("play", this);
	play -> setGeometry(560, 485, 80, 30);
	QPushButton *pause = new QPushButton("stop", this);
	pause -> setGeometry(650, 485, 80, 30);

	connect(play, SIGNAL(clicked()), this, SLOT(actionButton()));
	connect(pause, SIGNAL(clicked()), this, SLOT(quit()));

	//printf("salut\n");
	//this->getStream();
}

void MainWindow::actionButton()
{
    printf("bonjour\n");
    this->getStream();
}

void MainWindow::getStream()
{
 //       char* vec = frame->imageData;

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::quit()
{
	printf("quit\n");
}
