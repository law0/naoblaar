#include "slideritem.h"

SliderItem::SliderItem(Oscillator * osc, Oscillator::Setter choice) : _osc(osc), _choice(choice)
{
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(updateOscillator()));
}

void SliderItem::updateOscillator()
{
	float min = minimum();
	float range = (float)(maximum() - min);
	float val = ((float)(this->value()) - min)/ range;

	QStyleOptionSlider opt;
	initStyleOption(&opt);

	QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
	QPoint bottomRightCorner = sr.bottomLeft();
	switch(_choice)
	{
		case Oscillator::SetAlpha :
			_osc->setAlpha(val); //alpha between 0.0 and 1.0
//			printf("a%f\n", val);
		        QToolTip::showText(mapToGlobal( QPoint( bottomRightCorner.x(), bottomRightCorner.y() ) ), QString::number((double)val), this);
			break;

		case Oscillator::SetBeta :
			val = val * 2.4f + 0.1f;
//			printf("b%f\n", val);
			_osc->setBeta(val); //beta between 0.1 and 2.5
		        QToolTip::showText(mapToGlobal( QPoint( bottomRightCorner.x(), bottomRightCorner.y() ) ), QString::number((double)val), this);
			break;

		case Oscillator::SetCouple :
			val *= 0.3f;
			_osc->setCouple(val); //couple between 0.0 and 0.3
//			printf("c%f\n", val);
		        QToolTip::showText(mapToGlobal( QPoint( bottomRightCorner.x(), bottomRightCorner.y() ) ), QString::number((double)val), this);
			break;

		default:
			break;
	}
}
