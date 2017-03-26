#ifndef SLIDERITEM_H_INCLUDED
#define SLIDERITEM_H_INCLUDED

#include <QSlider>
#include <QObject>
#include <QStyleOptionSlider>
#include <QToolTip>

#include "oscillator.h"

class SliderItem : public QSlider
{

	Q_OBJECT

	public :
		SliderItem(Oscillator* osc, Oscillator::Setter);

	private :
		Oscillator* _osc;
		Oscillator::Setter _choice;

	private slots :
		void updateOscillator();


};


#endif //SLIDERITEM_H_INCLUDED
