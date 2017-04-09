/* Author Lawrencio Rasamoelison, Demailly Benjamin
 * this class is a subclass (inherits) of QCustomPlot
 * As such, it is used to trace plots.
 * More precisely, we use this class to trace data of the optical flows
 * and oscillators outputs
 * QCustomPlot is an external library, contained entirely in the
 * two file qcustomplot.h and qcustomplot.cpp.
 * See http://www.qcustomplot.com/
 * Is instantiated in mainwindow.cpp
 * Using QCustomPlot lib
 */

#ifndef PLOTITEM_H_INCLUDED
#define PLOTITEM_H_INCLUDED

#include <mutex>
#include "qcustomplot.h"
#include "oscillator.h"

class PlotItem : public QCustomPlot
{
	Q_OBJECT

	public :
		PlotItem(Oscillator &osc, Oscillator::Getter);

	public slots :
		void force_repaint();

	private :
		Oscillator::Getter _choice;
		QTimer _timer;
		QVector<double> _vec;
		QVector<double> _x;
		Oscillator& _osc;

};

#endif //PLOTITEM_H_INCLUDED
