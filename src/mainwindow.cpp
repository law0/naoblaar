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
	_connection_menu(new QMenu("Connection")),
	_config_menu_action(new QAction("Configuration", this)),
	_connect_nao_action(new QAction("Connect", this)),
	runningData(false),
	runningBoth(false)
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

	QGroupBox *groupSingle = new QGroupBox("Seul", this);
	groupSingle -> setGeometry(530, 470, 120, 95);

	play = new QPushButton("Enregistrer", this);
	play -> setGeometry(540, 485, 100, 30);

	//QPushButton *pause = new QPushButton("stop", this);
	//pause -> setGeometry(345, 505, 80, 30);

	data = new QPushButton("Data", this);
	data -> setGeometry(540, 520, 100, 30);

	QGroupBox *groupTogether = new QGroupBox("Ensemble", this);
	groupTogether -> setGeometry(660, 470, 120, 95);

	both = new QPushButton("Enregistrer", this);
	both -> setGeometry(670, 480, 100, 30);

	QLabel *labelRepository = new QLabel("Emplacement :", this);
	labelRepository -> setGeometry(10, 485, 100, 30);

	_repository = new QLineEdit(this);
	_repository -> setGeometry(110, 485, 300, 30);

	QLabel *labelTitle = new QLabel("Titre :", this);
	labelTitle -> setGeometry(10, 520, 100, 30);
	
	_title = new QLineEdit(this);
	_title -> setGeometry(110, 520, 200, 30);

	connect(play, SIGNAL(pressed()), this, SLOT(movieClick()));
	connect(data, SIGNAL(pressed()), this, SLOT(dataClick()));
	connect(both, SIGNAL(pressed()), this, SLOT(bothClick()));

	_menuBar = this->menuBar(); //create menu bar
	_connection_menu->addAction(_config_menu_action); //add action link to configNaoConnection to connection menu
	_connection_menu->addAction(_connect_nao_action); //add action connection to connection Menu
	_menuBar->addMenu(_connection_menu); //add connection menu to menu bar

	connect(_config_menu_action, SIGNAL(triggered()), this, SLOT(configNaoConnection()));
	connect(_connect_nao_action, SIGNAL(triggered()), this, SLOT(connectNao()));
	//printf("salut\n");
	//this->getStream();

}

void MainWindow::movieManagement()
{
	if (runningVideo)
	{
		_movie->stopCapture();
		runningVideo = false;
	}
	else
	{
		string place;
		if (!_repository->text().isEmpty())
		{
			place = "";
		}
		else
		{
			place = _repository->text().toStdString();
		}
		_movie->startCapture();
		runningVideo = true;
	}
}

void MainWindow::dataManagement()
{
	if (runningData)
	{
		_sd->stopSave();
		runningData = false;
	}
	else
	{
		string place;
		if (!_repository->text().isEmpty())
		{
			place = "";
		}
		else
		{
			place = _repository->text().toStdString();
		}
		_sd->startSave(place, _title->text().toStdString());
		runningData = true;
	}
}

void MainWindow::dataClick()
{
	if (runningData)
	{		//stop to save
		//data->setDefault(true);
		both->blockSignals(false);
		data->setText("Enregistrer");
	}
	else		//start to save
	{
		both->blockSignals(true);
		data->setText("Pause");
	}
	dataManagement();
}

void MainWindow::movieClick()
{
	if (runningData)
	{		//stop to save
		both->blockSignals(false);
		play->setText("Enregistrer");
	}
	else		//start to save
	{
		both->blockSignals(true);
		play->setText("Pause");
	}
	movieManagement();
}

void MainWindow::bothClick()
{
	if (runningBoth)
	{		//stop to save
		play->blockSignals(false);
		data->blockSignals(false);
		both->setDefault(false);
		both->setText("Enregistrer");
		runningBoth = false;
	}
	else		//start to save
	{
		play->blockSignals(true);
		data->blockSignals(true);
		both->setText("Pause");
		runningBoth = true;
	}
	dataManagement();
	movieManagement();
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

/*	_oscH = osc1;
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

void MainWindow::addSaveData(Savedata * sd)
{
	_sd = sd;
}

void MainWindow::addScriptLauncher(ScriptLauncher* sl)
{
	_sl = sl;
}

void MainWindow::connectNao() // this is a slot
{
	if(_connect_nao_action->text() == "Connect")
	{
		int r = this->connectToNao();
		if(r != 0)
		{
			QMessageBox errorBox;
			errorBox.setText(_last_error);
			errorBox.exec();
		}
		else
		{
			_last_error = "";
			_connect_nao_action->setText("Disconnect");
		}
	}
	else if(_connect_nao_action->text() == "Disconnect")
	{
		this->disconnectToNao();
		_connect_nao_action->setText("Connect");
	}
}

int MainWindow::connectToNao() //return 0 if no error
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
			if(s != 1) //if acceptable parameter but exited immediately...
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

void MainWindow::disconnectToNao()
{
	_sl->disconnect();
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
		joint_line->setInputMask("9");
		QLineEdit* ip_line = new QLineEdit(QString::fromStdString(_sl->getIp()));
		ip_line->setInputMask("000.000.000.000;_");
		QLineEdit* port_line = new QLineEdit(QString::number(_sl->getPort()));
		port_line->setInputMask("D0000;_");
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
