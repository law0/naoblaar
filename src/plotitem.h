#ifndef PLOTITEM_H_INCLUDED
#define PLOTITEM_H_INCLUDED

#include <mutex>
#include "qcustomplot.h"
#include "oscillator.h"

class PlotItem : public QCustomPlot
{
	Q_OBJECT

	public :
		PlotItem(Oscillator &osc);

	public slots :
		void force_repaint();

	private :
		QTimer _timer;
		QVector<double> _vec;
		QVector<double> _x;
		Oscillator& _osc;

};

#endif //PLOTITEM_H_INCLUDED
