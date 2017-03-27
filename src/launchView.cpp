#include "launchView.h"

void launchView(int argc, char **argv, StreamCatcher* sc, Oscillator* osc, Oscillator* osc2)
{
	QApplication a(argc, argv);

	QFile file("slider.qss");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
	     QTextStream stream(&file);
	     a.setStyleSheet(stream.readAll());
	     file.close();
	}

	MainWindow mw;

	QGraphicsScene scene;

	QGraphicsView view(&scene);

	mw.setCentralWidget(&view);

	//video on screen
	VideoItem vi(sc);

	//video recorder
	Movie movie(sc);
    	mw.addMovie(&movie);

	scene.addItem(&vi);

	mw.addOscillators(osc, osc2);

	mw.setFixedSize(1300, 620);

	view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        view.setFixedSize(sc->getWidth() + 2, sc->getHeight() + 2);

	mw.show();

	a.exec();

}


