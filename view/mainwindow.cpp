#include "mainwindow.h"
#include "ui_mainwindow.h"


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
    CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );

    if ( !capture )
    {
        fprintf( stderr, "ERROR: capture is NULL \n" );
        getchar();
        //return -1;
    }

    cvNamedWindow( "mywindow", CV_WINDOW_AUTOSIZE );

    while ( 1 )
    {
        IplImage* frame = cvQueryFrame( capture );
        if ( !frame )
        {
            fprintf( stderr, "ERROR: frame is null...\n" );
            getchar();
            break;
        }

        char* vec = frame->imageData;
            //vec = frame->imageData;
	for(int i = 0; i < frame->imageSize ; i++)
	{
		printf("%d\n", frame->widthStep);
	}

        cvShowImage( "mywindow", frame );
            // Do not release the frame!

        if ( (cvWaitKey(10) & 255) == 's' )
        {
            CvSize size = cvGetSize(frame);
            IplImage* img= cvCreateImage(size, IPL_DEPTH_16S, 1);
            img = frame;
            cvSaveImage("matteo.jpg",&img);
        }


        if ( (cvWaitKey(10) & 255) == 27 )
        {
            break;
        }
            //free(vec);
            //i++;
    }


}




MainWindow::~MainWindow()
{
    delete ui;
}
