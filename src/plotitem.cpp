#include "plotitem.h"

PlotItem::PlotItem(Oscillator &osc) : _timer(), _vec(201), _osc(osc)
{
	setFixedSize(300, 300);
        // create graph and assign data to it:
	QVector<double> x(501);
	for(int i = 0; i < 501; ++i)
	{
		x[i] = (double)i;
	}

	for(int i = 0; i <201; ++i)
	{
		_vec[i] = 0.0;
	}

        addGraph();
        graph(0)->setData(x, _vec);
        // give the axes some labels:
        xAxis->setLabel("x");
        yAxis->setLabel("y");
        // set axes ranges, so we see all data:
        xAxis->setRange(0, 5);
        yAxis->setRange(-1, 1);
//	yAxis->setRangeReversed();
        this->replot();

	 connect(&_timer, SIGNAL(timeout()), this, SLOT(force_repaint()));
        _timer.start(10);
}

void PlotItem::force_repaint()
{
	_vec.removeFirst();
	_vec.append(_osc.getVal());
	this->replot();
}
