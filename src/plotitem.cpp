#include "plotitem.h"

PlotItem::PlotItem(Oscillator &osc, int choice) : _choice(choice), _timer(), _vec(201, 0.0), _x(201, 0.0), _osc(osc)
{
	setFixedSize(400, 125);
        // create graph and assign data to it:
        addGraph();
	for(int i = 0; i < 201; ++i)
	{
		_x[i] = i/35.0 - 1;
	}
        graph(0)->setData(_x, _vec);
        // give the axes some labels:
        xAxis->setLabel("x");
        yAxis->setLabel("y");
        // set axes ranges, so we see all data:
        xAxis->setRange(0, 5);
        yAxis->setRange(-1, 1);
//	yAxis->setRangeReversed();
        this->replot();

	 connect(&_timer, SIGNAL(timeout()), this, SLOT(force_repaint()));
        _timer.start(100);
}

void PlotItem::force_repaint()
{
	std::mutex mtx;
	_vec.removeFirst();
	switch(_choice)
	{
		case 1 :
			mtx.lock();
			_vec.append(_osc.getOut());
			mtx.unlock();
			break;
		case 0 :
		default :
			mtx.lock();
			_vec.append(_osc.getVal());
			mtx.unlock();
			break;
	}
	this->graph(0)->setData(_x, _vec);
	this->replot();
}
