#include "launchView.h"

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
    	mw.addMovie(&movie); //add Movie object for the button "record"

	scene.addItem(&vi);

	mw.addOscillators(osc, osc2); //add oscillators has most widget contained in mw needs the oscillators

	mw.setFixedSize(1300, 620);

	view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        view.setFixedSize(sc->getWidth() + 2, sc->getHeight() + 2);

	mw.show();

	a.exec();

}


