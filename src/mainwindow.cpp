#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utilities.h"

MainWindow::MainWindow(char* frameptr, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	_frameptr(frameptr)
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

	//connect(play, SIGNAL(clicked()), this, SLOT(actionButton()));
	//    connect(pause, SIGNAL(clicked()), this, SLOT(quit()));

	//printf("salut\n");
	//this->getStream();
}

/*void MainWindow::actionButton()
{
    printf("bonjour\n");
    this->getStream();
}*/

/*void MainWindow::getStream()
{
        char* vec = frame->imageData;

}*/

void MainWindow::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);

    // When no image is loaded, paint the window with black
    /*if (!_image)
    {
        painter.fillRect(QRectF(QPoint(0, 0), QSize(width(), height())), Qt::black);
        QWidget::paintEvent(e);
        return;
    }*/

	unsigned int size = global_WIDTH * global_HEIGHT;

	unsigned char out[size];

	char_bgr_to_rgba(out, (unsigned char *)_frameptr, size * 3);

	QImage image(out, global_WIDTH, global_HEIGHT, QImage::Format_RGBA8888);

	painter.drawImage(QRectF(QPoint(0, 0), QSize(width(), height())), image, QRectF(QPoint(0, 0), image.size()));

	QWidget::paintEvent(e);
}


MainWindow::~MainWindow()
{
    delete ui;
}
