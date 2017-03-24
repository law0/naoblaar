#include "slideritem.h"

SliderItem::SliderItem(Oscillator * osc, Oscillator::Setter choice) : _osc(osc), _choice(choice)
{
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(updateOscillator()));
}

void SliderItem::updateOscillator()
{
	float min = minimum();
	float range = (float)(maximum() - min);
	switch(_choice)
	{
		case Oscillator::SetAlpha :
			_osc->setAlpha( ((float)(this->value()) - min)/ range ); //alpha between 0.0 and 1.0
			break;

		case Oscillator::SetBeta :
			_osc->setBeta( ((float)(this->value()) - min)/ range * 2.4f + 0.1f); //beta between 0.1 and 2.5
			break;

		case Oscillator::SetCouple :
			_osc->setCouple( ((float)(this->value()) - min)/ range * 0.3f); //couple between 0.0 and 0.3

		default:
			break;
	}
}
