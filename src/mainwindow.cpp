/* Author Lawrencio Rasamoelison, Demailly Benjamin
 * see mainwindow.h
 */

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
	_save_config_action(new QAction("Save configuration", this)),
	_sensi_spin(NULL),
	runningData(false),
	runningBoth(false)
{
//	setFixedSize(1300, 620);
	QToolBar* downToolBar = new QToolBar(this);
	downToolBar->setAllowedAreas(Qt::BottomToolBarArea);

	QGroupBox *groupSingle = new QGroupBox("Alone", this);
	groupSingle -> setGeometry(530, 455, 120, 95);

	play = new QPushButton("Save Movie", this);
	//play -> setGeometry(540, 485, 100, 30);

	data = new QPushButton("Save Data", this);
	//data -> setGeometry(540, 520, 100, 30);

	QVBoxLayout *vbox_for_data_play = new QVBoxLayout;
	vbox_for_data_play->addWidget(groupSingle);
	vbox_for_data_play->addWidget(play);
	vbox_for_data_play->addWidget(data);
//	vbox_for_data_play->addStretch(1);

	QWidget *inter_data_play = new QWidget;//(downToolBar);
	inter_data_play->setLayout(vbox_for_data_play);

	//downToolBar->addWidget(inter_data_play);

	QGroupBox *groupTogether = new QGroupBox("Both", this);
//	groupTogether -> setGeometry(660, 470, 120, 95);
	both = new QPushButton("Record data and video", this);
//	both -> setGeometry(670, 500, 100, 30);

	QVBoxLayout *vbox_both = new QVBoxLayout;
	vbox_both->addWidget(groupTogether);
	vbox_both->addWidget(both);
//	vbox_both->addStretch(1);

	QWidget *inter_both = new QWidget;//(downToolBar);
	inter_both->setLayout(vbox_both);

	QFormLayout* form_rec = new QFormLayout;
//	QLabel *labelRepository = new QLabel("Emplacement :", this);
//	labelRepository -> setGeometry(10, 485, 100, 30);
	QLineEdit* repository_line = new QLineEdit(this);
	QPushButton* repository_button = new QPushButton("Explore", this);
	_repository = new FileLineButton(repository_line, repository_button, QFileDialog::Directory);
//	_repository -> setGeometry(110, 485, 300, 30);

//	QLabel *labelTitle = new QLabel("Titre :", this);
//	labelTitle -> setGeometry(10, 520, 100, 30);

	_title = new QLineEdit(this);
//	_title -> setGeometry(110, 520, 200, 30);

	form_rec->addRow("Path :", _repository);
	form_rec->addRow("Title :", _title);

	QWidget *inter_file = new QWidget;//(downToolBar);
	inter_file->setLayout(form_rec);

	QHBoxLayout* hbox = new QHBoxLayout;
	hbox->addWidget(inter_file);
	hbox->addSpacing(50);
	hbox->addWidget(inter_data_play);
	hbox->addSpacing(10);
	hbox->addWidget(inter_both);
	hbox->addSpacing(500);

	QWidget* last_inter = new QWidget(downToolBar);
	last_inter->setLayout(hbox);
	downToolBar->addWidget(last_inter);
	this->addToolBar(Qt::BottomToolBarArea, downToolBar);


	connect(play, SIGNAL(pressed()), this, SLOT(movieClick()));
	connect(data, SIGNAL(pressed()), this, SLOT(dataClick()));
	connect(both, SIGNAL(pressed()), this, SLOT(bothClick()));

	_menuBar = this->menuBar(); //create menu bar
	_connection_menu->addAction(_config_menu_action); //add action link to configNaoConnection to connection menu
	_connection_menu->addAction(_save_config_action);
	_connection_menu->addAction(_connect_nao_action); //add action connection to connection Menu
	_menuBar->addMenu(_connection_menu); //add connection menu to menu bar

	connect(_config_menu_action, SIGNAL(triggered()), this, SLOT(configNaoConnection()));
	connect(_connect_nao_action, SIGNAL(triggered()), this, SLOT(connectNao()));
	connect(_save_config_action, SIGNAL(triggered()), this, SLOT(saveConfig()));
	//printf("salut\n");
	//this->getStream();
	play->clearFocus();
	clickButton(data);
	clickButton(play);
	clickButton(both);


}

void MainWindow::movieManagement(string name)
{
	if (runningVideo)
	{
		_movie->stopCapture();
		runningVideo = false;
	}
	else
	{
		_movie->startCapture(name);
		runningVideo = true;
	}
}

void MainWindow::dataManagement(string name)
{
	if (runningData)
	{
		_sd->stopSave();
		runningData = false;
	}
	else
	{
		_sd->startSave(name);
		runningData = true;
	}
}

void MainWindow::dataClick()
{
	if (runningData)
	{		//stop to save
		//data->setDefault(true);
		clickButton(both);
		data->setText("Save Data");
		data->clearFocus();
		dataManagement("");
	}
	else		//start to save
	{
		breakButton(both);
		data->setText("Stop");
		data->clearFocus();
		string name = getName(true);
		dataManagement(name);
	}
}

void MainWindow::movieClick()
{
	if (runningVideo)
	{		//stop to save
		clickButton(both);
		play->setText("Save Movie");
		play->clearFocus();
		movieManagement("");
	}
	else		//start to save
	{
		breakButton(both);
		play->setText("Stop");
		play->clearFocus();
		string name = getName(false);
		movieManagement(name);
	}

}

void MainWindow::bothClick()
{
	if (runningBoth)
	{		//stop to save
		clickButton(play);
		clickButton(data);
		both->setText("Record data and video");
		both->clearFocus();
		runningBoth = false;
		dataManagement("");
		movieManagement("");
	}
	else		//start to save
	{
		breakButton(play);
		breakButton(data);
		both->setText("Stop");
		both->clearFocus();
		runningBoth = true;
		string name = getName(true);
		dataManagement(name);
		string nameM = getName(false);
		movieManagement(nameM);
	}

}

void MainWindow::clickButton(QPushButton * button)
{
	button->blockSignals(false);
	button->setEnabled(true);
	button->setStyleSheet("background-color : white; border-radius : 5px;");
	button->clearFocus();
}

void MainWindow::breakButton(QPushButton * button)
{
	button->blockSignals(true);
	button->clearFocus();
	button->setEnabled(false);
	button->setStyleSheet("background-color : gray; border-radius : 5px;");
}

string MainWindow::getName(bool isData)
{
	string place = _repository->text().toStdString();
	string name = _title->text().toStdString();
	if (name == "")
	{
		name = "test";
	}
	string endD(".csv");
	string endM(".avi");
	string titleD = place + name + endD;
	string titleM = place + name + endM;
	int i = 1;
	FILE * data;
	FILE * movie;
	bool exist = true;
	while (exist)
	{
		exist = false;
		if ((data = fopen(titleD.c_str(), "r")))
		{
			exist = true;
			fclose(data);
		} 
		
		if ((movie = fopen(titleM.c_str(), "r")))
		{
			fclose(movie);
			exist = true;
		}
		
		if (exist)
		{
			titleD = place + name + to_string(i) + endD;
			titleM = place + name + to_string(i) + endM;
			i++;
		}
	}


	if (isData)
	{
		return titleD;
	}
	else
	{
		return titleM;
	}
	return "test";	
	
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

		QPushButton* naoqi_button = new QPushButton("Explore");
		QPushButton* mainscript_button = new QPushButton("Explore");
		QPushButton* python_button = new QPushButton("Explore");
		QPushButton* shm_button = new QPushButton("Explore");

		FileLineButton* naoqi_filelinebutton = new FileLineButton(naoqi_line, naoqi_button, QFileDialog::Directory);
		FileLineButton* mainscript_filelinebutton = new FileLineButton(mainscript_line, mainscript_button);
		FileLineButton* python_filelinebutton = new FileLineButton(python_line, python_button);
		FileLineButton* shm_filelinebutton = new FileLineButton(shm_line, shm_button);

		naoqi_filelinebutton->setFixedWidth(600);
		mainscript_filelinebutton->setFixedWidth(600);
		python_filelinebutton->setFixedWidth(600);
		shm_filelinebutton->setFixedWidth(600);

		formLayout->addRow("Oscillator to connect to", oscillator_line);
		formLayout->addRow("Joint (0 to 9)", joint_line);
		formLayout->addRow("IP", ip_line);
		formLayout->addRow("PORT", port_line);
		formLayout->addRow("(Advanced) path to naoqi", naoqi_filelinebutton);
		formLayout->addRow("(Advanced) path to main.py", mainscript_filelinebutton);
		formLayout->addRow("(Advanced) path to python", python_filelinebutton);
		formLayout->addRow("(Advanced) Shared memory file", shm_filelinebutton);

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

void MainWindow::saveConfig()
{
	_sl->saveConfigToFile("config.benlaw");
}

void MainWindow::setSensibilityPtr(float* sensibility)
{
	_sensibility = sensibility;
	if(_sensi_spin == NULL)
	{
		_sensi_spin = new QDoubleSpinBox(this);
		_sensi_spin->setGeometry(660, 300, 120, 25);
		QLabel* label = new QLabel("Optical flow sensibility", this);
		label->setGeometry(660, 280, 300, 25);
	}
	_sensi_spin->setMinimum(0.000001);
	_sensi_spin->setMaximum(0.000050);
	_sensi_spin->setDecimals(8);
	_sensi_spin->setSingleStep(0.0000001);
	_sensi_spin->setValue((double)(*_sensibility));
	connect(_sensi_spin, SIGNAL(valueChanged(double)), this, SLOT(sensiChanged(double)));
}

void MainWindow::sensiChanged(double value)
{
	std::mutex mtx;
	mtx.lock();
	*_sensibility = (float)(value);
	mtx.unlock();
}
