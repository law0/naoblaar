#include "launchView.h"

#define VIDEO_WIDTH 640
#define VIDEO_HEIGHT 480

void launchView(int argc, char **argv, bool* isClosed, StreamCatcher* sc, ScriptLauncher* sl, Oscillator* osc, Oscillator* osc2)
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


