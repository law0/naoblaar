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

	//Dockwidget features
	QDockWidget::DockWidgetFeatures DWflag = QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable;

	//plots (graph) -------------------------------
	PlotItem plotitemX(*osc, Oscillator::GetVal);
	PlotItem plotitemY(*osc2, Oscillator::GetVal);
	PlotItem plotitemOSCX(*osc, Oscillator::GetOut);
	PlotItem plotitemOSCY(*osc2, Oscillator::GetOut);

	//dockwdiget of plots
		QDockWidget* DWplotitemX = new QDockWidget("Horizontal flow");
		QDockWidget* DWplotitemY = new QDockWidget("Vertical flow");
		QDockWidget* DWplotitemOSCX = new QDockWidget("Horizontal oscillator");
		QDockWidget* DWplotitemOSCY = new QDockWidget("Vertical oscillator");

		DWplotitemX->setWidget(&plotitemX);
		DWplotitemY->setWidget(&plotitemY);
		DWplotitemOSCX->setWidget(&plotitemOSCX);
		DWplotitemOSCY->setWidget(&plotitemOSCY);

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

		mw.addDockWidget(Qt::RightDockWidgetArea, DWplotitemX);
		mw.addDockWidget(Qt::RightDockWidgetArea, DWplotitemOSCX);
		mw.addDockWidget(Qt::RightDockWidgetArea, DWplotitemY);
		mw.addDockWidget(Qt::RightDockWidgetArea, DWplotitemOSCY);
	//end of plots (graph) --------------------------

	//sliders ---------------------------------------
	SliderItem coupleSlider1(osc, Oscillator::SetCouple);
	SliderItem coupleSlider2(osc2, Oscillator::SetCouple);
	SliderItem alphaSlider1(osc, Oscillator::SetAlpha);
	SliderItem alphaSlider2(osc2, Oscillator::SetAlpha);
	SliderItem betaSlider1(osc, Oscillator::SetBeta);
	SliderItem betaSlider2(osc2, Oscillator::SetBeta);

	coupleSlider1.setOrientation(Qt::Horizontal);
	coupleSlider2.setOrientation(Qt::Horizontal);
	alphaSlider1.setOrientation(Qt::Horizontal);
	alphaSlider2.setOrientation(Qt::Horizontal);
	betaSlider1.setOrientation(Qt::Horizontal);
	betaSlider2.setOrientation(Qt::Horizontal);

	coupleSlider1.setMinimum(0);
	coupleSlider1.setMaximum(300); //makes it between 0 and 0.3 with 0.01 step precision
	coupleSlider1.setValue(200);
	coupleSlider2.setMinimum(0);
	coupleSlider2.setMaximum(300);
	coupleSlider2.setValue(200);

	//coupleSlider1.setTickInterval(10);
	//coupleSlider2.setTickInterval(10);

	alphaSlider1.setMinimum(0);
	alphaSlider1.setMaximum(100); //makes it between 0 and 1 with 0.01 step precision
	alphaSlider1.setValue(7);
	alphaSlider2.setMinimum(0);
	alphaSlider2.setMaximum(100);
	alphaSlider2.setValue(7);

	//alphaSlider1.setTickInterval(10);
	//alphaSlider2.setTickInterval(10);

	betaSlider1.setMinimum(100);
	betaSlider1.setMaximum(2500); //makes it between 0.1 and 2.5 with 0.001 step precision
	betaSlider1.setValue(150);
	betaSlider2.setMinimum(100);
	betaSlider2.setMaximum(2500);
	betaSlider2.setValue(150);

	//betaSlider1.setTickInterval(100);
 	//betaSlider2.setTickInterval(100);

	/*coupleSlider1.setTickPosition(QSlider::TicksAbove);
	coupleSlider2.setTickPosition(QSlider::TicksAbove);
	alphaSlider1.setTickPosition(QSlider::TicksAbove);
	alphaSlider2.setTickPosition(QSlider::TicksAbove);
	betaSlider1.setTickPosition(QSlider::TicksAbove);
 	betaSlider2.setTickPosition(QSlider::TicksAbove);*/


	//sliders dockwidget
		QDockWidget* DWcoupleSlider1 = new QDockWidget("Couple Slider Flux Horizontal");
		QDockWidget* DWcoupleSlider2 = new QDockWidget("Couple Slider Flux Vertical");
		QDockWidget* DWalphaSlider1 = new QDockWidget("Alpha Slider Flux Horizontal");
		QDockWidget* DWalphaSlider2 = new QDockWidget("Alpha Slider Flux Vertical");
		QDockWidget* DWbetaSlider1 = new QDockWidget("Beta Slider Flux Horizontal");
		QDockWidget* DWbetaSlider2 = new QDockWidget("Beta SLider Flux Vertical");

		DWcoupleSlider1->setWidget(&coupleSlider1);
		DWcoupleSlider2->setWidget(&coupleSlider2);
		DWalphaSlider1->setWidget(&alphaSlider1);
		DWalphaSlider2->setWidget(&alphaSlider2);
		DWbetaSlider1->setWidget(&betaSlider1);
		DWbetaSlider2->setWidget(&betaSlider2);

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

		mw.addDockWidget(Qt::BottomDockWidgetArea, DWcoupleSlider1);
		mw.addDockWidget(Qt::BottomDockWidgetArea, DWcoupleSlider2);
		mw.addDockWidget(Qt::BottomDockWidgetArea, DWalphaSlider1);
		mw.addDockWidget(Qt::BottomDockWidgetArea, DWalphaSlider2);
		mw.addDockWidget(Qt::BottomDockWidgetArea, DWbetaSlider1);
		mw.addDockWidget(Qt::BottomDockWidgetArea, DWbetaSlider2);

		mw.splitDockWidget(DWcoupleSlider1, DWcoupleSlider2, Qt::Vertical);
		mw.splitDockWidget(DWalphaSlider1, DWalphaSlider2, Qt::Vertical);
		mw.splitDockWidget(DWbetaSlider1, DWbetaSlider2, Qt::Vertical);
	//end of sliders

	mw.setFixedSize(1300, 620);

	view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        view.setFixedSize(sc->getWidth() + 2, sc->getHeight() + 2);

	mw.show();

	a.exec();

}


