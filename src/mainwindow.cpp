#include <mutex>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utilities.h"

MainWindow::MainWindow(bool* isClosed, QWidget *parent) :
	QMainWindow(parent),
	_isClosed(isClosed),
	ui(new Ui::MainWindow),
	runningVideo(false),
	_sl(NULL),
	_last_error(""),
	_config_menu(new QMenu("Configuration")),
	_config_menu_action(new QAction(tr("&euh... Configuration?"), this))
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

	_menuBar = this->menuBar();
	_config_menu->addAction(_config_menu_action);
	_menuBar->addMenu(_config_menu);


	connect(_config_menu_action, SIGNAL(triggered()), this, SLOT(configNaoConnection()));
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

void MainWindow::addOscillators(Oscillator * osc1, Oscillator * osc2)
{

	/*_oscH = osc1; //la vue ne doit pas contenir les oscillateur quand même!
	_oscV = osc2;*/
//Dockwidget features
        QDockWidget::DockWidgetFeatures DWflag = QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable;

        //plots (graph) -------------------------------
        _plotitemX = new PlotItem(*osc1, Oscillator::GetVal);
        _plotitemY = new PlotItem(*osc2, Oscillator::GetVal);
        _plotitemOSCX = new PlotItem(*osc1, Oscillator::GetOut);
        _plotitemOSCY = new PlotItem(*osc2, Oscillator::GetOut);

        //dockwdiget of plots
                QDockWidget* DWplotitemX = new QDockWidget("Horizontal flow");
                QDockWidget* DWplotitemY = new QDockWidget("Vertical flow");
                QDockWidget* DWplotitemOSCX = new QDockWidget("Horizontal oscillator");
                QDockWidget* DWplotitemOSCY = new QDockWidget("Vertical oscillator");

                DWplotitemX->setWidget(_plotitemX);
                DWplotitemY->setWidget(_plotitemY);
                DWplotitemOSCX->setWidget(_plotitemOSCX);
                DWplotitemOSCY->setWidget(_plotitemOSCY);

                DWplotitemX->setFeatures(DWflag);
                DWplotitemY->setFeatures(DWflag);
                DWplotitemOSCX->setFeatures(DWflag);
                DWplotitemOSCY->setFeatures(DWflag);

                DWplotitemX->setAllowedAreas(Qt::RightDockWidgetArea);
                DWplotitemY->setAllowedAreas(Qt::RightDockWidgetArea);
                DWplotitemOSCX->setAllowedAreas(Qt::RightDockWidgetArea);
                DWplotitemOSCY->setAllowedAreas(Qt::RightDockWidgetArea);

                DWplotitemX->setFixedSize(400, 125);
                DWplotitemY->setFixedSize(400, 125);
                DWplotitemOSCX->setFixedSize(400, 125);
                DWplotitemOSCY->setFixedSize(400, 125);

                addDockWidget(Qt::RightDockWidgetArea, DWplotitemX);
                addDockWidget(Qt::RightDockWidgetArea, DWplotitemOSCX);
                addDockWidget(Qt::RightDockWidgetArea, DWplotitemY);
                addDockWidget(Qt::RightDockWidgetArea, DWplotitemOSCY);
        //end of plots (graph) --------------------------

        //sliders ---------------------------------------
        _coupleSlider1 = new SliderItem(osc1, Oscillator::SetCouple);
        _coupleSlider2 = new SliderItem(osc2, Oscillator::SetCouple);
        _alphaSlider1 = new SliderItem(osc1, Oscillator::SetAlpha);
        _alphaSlider2 = new SliderItem(osc2, Oscillator::SetAlpha);
        _betaSlider1 = new SliderItem(osc1, Oscillator::SetBeta);
        _betaSlider2 = new SliderItem(osc2, Oscillator::SetBeta);

        _coupleSlider1->setOrientation(Qt::Horizontal);
        _coupleSlider2->setOrientation(Qt::Horizontal);
        _alphaSlider1->setOrientation(Qt::Horizontal);
        _alphaSlider2->setOrientation(Qt::Horizontal);
        _betaSlider1->setOrientation(Qt::Horizontal);
        _betaSlider2->setOrientation(Qt::Horizontal);

        _coupleSlider1->setMinimum(0);
        _coupleSlider1->setMaximum(300); //makes it between 0 and 0.3 with 0.01 step precision
        _coupleSlider1->setValue(200);
        _coupleSlider2->setMinimum(0);
        _coupleSlider2->setMaximum(300);
        _coupleSlider2->setValue(200);

        //coupleSlider1->setTickInterval(10);
        //coupleSlider2->setTickInterval(10);

        _alphaSlider1->setMinimum(0);
        _alphaSlider1->setMaximum(100); //makes it between 0 and 1 with 0.01 step precision
        _alphaSlider1->setValue(7);
        _alphaSlider2->setMinimum(0);
        _alphaSlider2->setMaximum(100);
        _alphaSlider2->setValue(7);

        //alphaSlider1->setTickInterval(10);
        //alphaSlider2->setTickInterval(10);

        _betaSlider1->setMinimum(100);
        _betaSlider1->setMaximum(2500); //makes it between 0.1 and 2.5 with 0.001 step precision
        _betaSlider1->setValue(150);
        _betaSlider2->setMinimum(100);
        _betaSlider2->setMaximum(2500);
        _betaSlider2->setValue(150);

        //betaSlider1->setTickInterval(100);
        //betaSlider2->setTickInterval(100);

        /*coupleSlider1->setTickPosition(QSlider::TicksAbove);
        coupleSlider2->setTickPosition(QSlider::TicksAbove);
        alphaSlider1->setTickPosition(QSlider::TicksAbove);
        alphaSlider2->setTickPosition(QSlider::TicksAbove);
        betaSlider1->setTickPosition(QSlider::TicksAbove);
        betaSlider2->setTickPosition(QSlider::TicksAbove);*/


        //sliders dockwidget
                QDockWidget* DWcoupleSlider1 = new QDockWidget("Couple Slider Flux Horizontal");
                QDockWidget* DWcoupleSlider2 = new QDockWidget("Couple Slider Flux Vertical");
                QDockWidget* DWalphaSlider1 = new QDockWidget("Alpha Slider Flux Horizontal");
                QDockWidget* DWalphaSlider2 = new QDockWidget("Alpha Slider Flux Vertical");
                QDockWidget* DWbetaSlider1 = new QDockWidget("Beta Slider Flux Horizontal");
                QDockWidget* DWbetaSlider2 = new QDockWidget("Beta Slider Flux Vertical");

		DWcoupleSlider1->setWidget(_coupleSlider1);
                DWcoupleSlider2->setWidget(_coupleSlider2);
                DWalphaSlider1->setWidget(_alphaSlider1);
                DWalphaSlider2->setWidget(_alphaSlider2);
                DWbetaSlider1->setWidget(_betaSlider1);
                DWbetaSlider2->setWidget(_betaSlider2);

                DWcoupleSlider1->setFeatures(DWflag);
                DWcoupleSlider2->setFeatures(DWflag);
                DWalphaSlider1->setFeatures(DWflag);
                DWalphaSlider2->setFeatures(DWflag);
                DWbetaSlider1->setFeatures(DWflag);
                DWbetaSlider2->setFeatures(DWflag);

                DWcoupleSlider1->setAllowedAreas(Qt::BottomDockWidgetArea);
                DWcoupleSlider2->setAllowedAreas(Qt::BottomDockWidgetArea);
                DWalphaSlider1->setAllowedAreas(Qt::BottomDockWidgetArea);
                DWalphaSlider2->setAllowedAreas(Qt::BottomDockWidgetArea);
                DWbetaSlider1->setAllowedAreas(Qt::BottomDockWidgetArea);
                DWbetaSlider2->setAllowedAreas(Qt::BottomDockWidgetArea);

                addDockWidget(Qt::BottomDockWidgetArea, DWcoupleSlider1);
                addDockWidget(Qt::BottomDockWidgetArea, DWcoupleSlider2);
                addDockWidget(Qt::BottomDockWidgetArea, DWalphaSlider1);
                addDockWidget(Qt::BottomDockWidgetArea, DWalphaSlider2);
                addDockWidget(Qt::BottomDockWidgetArea, DWbetaSlider1);
                addDockWidget(Qt::BottomDockWidgetArea, DWbetaSlider2);

                splitDockWidget(DWcoupleSlider1, DWcoupleSlider2, Qt::Vertical);
                splitDockWidget(DWalphaSlider1, DWalphaSlider2, Qt::Vertical);
                splitDockWidget(DWbetaSlider1, DWbetaSlider2, Qt::Vertical);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	std::mutex mtx;
	mtx.lock();
	*_isClosed = true;
	mtx.unlock();
	event->accept();
}

void MainWindow::addScriptLauncher(ScriptLauncher* sl)
{
	_sl = sl;
}

int MainWindow::connectToNao()
{
	int ret = 0;
	if(_sl != NULL)
	{
		int r = _sl->connect();
		if(r != 0) //failed by wrong parameter
		{
			_last_error = QString::fromStdString(_sl->getError());
			ret = 1;
		}
		else //may not be wrong parameter but may fail for other reason (like Nao is not there)
		{
			int s = _sl->getStatus();
			if(WIFEXITED(s) == true) //if acceptable parameter but exited immediately...
			{
				_last_error = QString::fromStdString(_sl->getError());
				ret = 2;
			}
			//else process is still continuing so it's alright
		}
	}
	else
	{
		_last_error = "ScriptLauncher ptr is NULL";
		ret = 3;
	}

	return ret;
}

void MainWindow::chooseIpPort(QString ip, int port)
{
	if(_sl != NULL)
	{
		_sl->setIp(ip.toUtf8().constData());
		_sl->setPort(port);
	}
}

void MainWindow::chooseJoint(int joint)
{
	if(_sl != NULL)
	{
		_sl->setJoint(joint);
	}
}

void MainWindow::chooseNaoqiPath(QString path)
{
	if(_sl != NULL)
	{
		_sl->setNaoqiPath(path.toUtf8().constData());
	}
}

void MainWindow::chooseMainScriptPath(QString path)
{
	if(_sl != NULL)
	{
		_sl->setMainScriptPath(path.toUtf8().constData());
	}
}

void MainWindow::choosePythonPath(QString path)
{
	if(_sl != NULL)
	{
		_sl->setPythonPath(path.toUtf8().constData());
	}
}

void MainWindow::chooseOscillator(int choice)
{
	if(_sl != NULL)
	{
		_sl->chooseOscillator(choice);
	}
}

void MainWindow::chooseSharedMemoryPath(QString path)
{
	if(_sl != NULL)
	{
		_sl->setSharedMemoryPath(path.toUtf8().constData());
	}
}

void MainWindow::configNaoConnection()
{
	if(_sl == NULL)
	{
		;
	}
	else
	{
		QWizardPage *page = new QWizardPage;
		page->setTitle("Configuration");

		QFormLayout* formLayout = new QFormLayout;

		QLineEdit* oscillator_line = new QLineEdit(QString::fromStdString(_sl->getChoosenOscillator()));
		QLineEdit* joint_line = new QLineEdit(QString::number(_sl->getJoint()));
		QLineEdit* ip_line = new QLineEdit(QString::fromStdString(_sl->getIp()));
		QLineEdit* port_line = new QLineEdit(QString::number(_sl->getPort()));
		QLineEdit* naoqi_line = new QLineEdit(QString::fromStdString(_sl->getNaoqiPath()));
		QLineEdit* mainscript_line = new QLineEdit(QString::fromStdString(_sl->getMainScriptPath()));
		QLineEdit* python_line = new QLineEdit(QString::fromStdString(_sl->getPythonPath()));
		QLineEdit* shm_line = new QLineEdit(QString::fromStdString(_sl->getSharedMemoryPath()));

		formLayout->addRow("Oscillator to connect to", oscillator_line);
		formLayout->addRow("Joint (0 to 9)", joint_line);
		formLayout->addRow("IP", ip_line);
		formLayout->addRow("PORT", port_line);
		formLayout->addRow("(Advanced) path to naoqi", naoqi_line);
		formLayout->addRow("(Advanced) path to main.py", mainscript_line);
		formLayout->addRow("(Advanced) path to python", python_line);
		formLayout->addRow("(Advanced) Shared memory file", shm_line);

		page->setLayout(formLayout);

		QWizard wizard;
		wizard.addPage(page);
		int r = wizard.exec(); //blocking on purpose

		if(r == 1) //accepted
		{
			if(oscillator_line->text() == "horizontal" || oscillator_line->text() == "Horizontal")
			{
				_sl->chooseOscillator(0);
			}
			else if(oscillator_line->text() == "vertical" || oscillator_line->text() == "Vertical")
			{
				_sl->chooseOscillator(1);
			}

			_sl->setJoint(joint_line->text().toInt());
			_sl->setIp(ip_line->text().toUtf8().constData());
			_sl->setPort(port_line->text().toInt());
			_sl->setNaoqiPath(naoqi_line->text().toUtf8().constData());
			_sl->setMainScriptPath(mainscript_line->text().toUtf8().constData());
			_sl->setPythonPath(python_line->text().toUtf8().constData());
			_sl->setSharedMemoryPath(shm_line->text().toUtf8().constData());
		}
	}
}
