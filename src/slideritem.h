/* Author Lawrencio Rasamoelison, Demailly Benjamin
 * This class inherits from QSlider
 * We use slider in our application to control those following variables :
 *	- oscillator coupling factor
 *	- oscillator alpha
 * 	- oscillator beta
 * (for each oscillator of course)
 * This class use Qt
 * Instantiated in mainwindow.cpp
 */

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
