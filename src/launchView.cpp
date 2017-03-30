#include "launchView.h"

#define VIDEO_WIDTH 600
#define VIDEO_HEIGHT 400

void launchView(int argc, char **argv, bool* isClosed, ScriptLauncher* sl, Oscillator* osc, Oscillator* osc2)
{
	QApplication a(argc, argv);

	QFile file("slider.qss");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
	     QTextStream stream(&file);
	     a.setStyleSheet(stream.readAll());
	     file.close();
	}

	MainWindow mw(isClosed);

	StreamCatcher* sc;

	int choice = 0;

	if( ! StreamCatcher::isInstantiated())
	{
		//calling StreamCatcher::getInstance(choice) is safe
		//meaning calling it twice won't instantiate twice
		//but here we want to know if it has already been instantiated
		//as that would mean a camera have already been choosen
		//and thus we don't need to pop a dialog to ask the user

		choice = chooseCamera(true);

		printf("Instantiating cam in gui with choice %d\n", choice);

	}

	sc = StreamCatcher::getInstance(choice); //if camera has already been choosen, this will only return the singleton already instantiated


	QGraphicsScene scene;

	QGraphicsView view(&scene);

	mw.setCentralWidget(&view);

	//scriptLauncher (join with menu to be launch via menu)
	mw.addScriptLauncher(sl);

	//video on screen
	VideoItem vi(sc);

	//video recorder
	Movie movie(sc);

	//save data
	Savedata sd(osc, osc2);
    	mw.addMovie(&movie); //add Movie object for the button "record"
	mw.addSaveData(&sd);


	scene.addItem(&vi);

	mw.addOscillators(osc, osc2); //add oscillators has most widget contained in mw needs the oscillators

	mw.setFixedSize(1300, 670);

	view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


        view.scale((float)VIDEO_WIDTH / (float)sc->getWidth(), (float)VIDEO_HEIGHT / (float)sc->getHeight());
	view.setFixedSize(VIDEO_WIDTH, VIDEO_HEIGHT);

	mw.show();

	a.exec();

}

int chooseCamera(bool gui)
{
	int ret = 0;
	std::vector<int> listCams;
	listCams = StreamCatcher::getListCameras();
	int vsize = listCams.size();

//	if(size > 1)
//	{
		if(gui == true)
		{
			std::string str = "";
			int i;
			int max = 0;
			for (i = 0; i < vsize; i++)
			{
				str.append(std::to_string(listCams[i]) + "; ");
				max = max < listCams[i] ? listCams[i] : max;
			}
			bool ok;
			int choice = -1;
			choice = QInputDialog::getInt(NULL, "Choose camera input",
	                      "Choose camera : " + QString::fromStdString(str), 0, 0, max, 1, &ok);

			if (ok && choice != -1)
			{
				ret = choice;
			}
		}
		else if(gui == false)
		{
			printf("choose camera. Enter one the number list below\n");
			printf("List of cameras : \n");
			for (int i = 0; i < vsize; i++)
			{
				printf("Camera %d\n", listCams[i]);
			}
			printf("end of list\n\n\n");
			ret = getchar() - 48;
		}
//	}
	return ret;

}


