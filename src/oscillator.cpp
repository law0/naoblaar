#include "oscillator.h"

Oscillator::Oscillator() : _out1(0.f), _out2(0.f), _alpha(0.07f), _beta(0.15f), _couple(0.2f)
{

}

Oscillator::Oscillator(float alpha, float beta) : _out1(0.f), _out2(0.f), _alpha(alpha), _beta(beta), _couple(0.2f)
{

}

float Oscillator::getOut() const
{
	return _out1;
}

float Oscillator::getVal() const
{
	return _val;
}

float Oscillator::getAlpha() const
{
	return _alpha;
}

float Oscillator::getBeta() const
{
	return _beta;
}

float Oscillator::getCouple() const
{
	return _couple;
}

float Oscillator::setVal(float val)
{
	_val = val;
	float tmp_out1 = _out1 - _beta * _out2 + _alpha + _couple * _val;
	_out2  = _out2 >  1.f ? 1.f : _out2;
	_out2  = _out2 <  0.f ? 0.f : _out2;
	_out2 = _out2 + _beta * _out1;
	_out1 = tmp_out1;

	return _out1;
}

float Oscillator::operator()(float val)
{
	return this->setVal(val);
}

void Oscillator::setAlpha(float a)
{
	_alpha = a;
}

void Oscillator::setBeta(float b)
{
	_beta = b;
}

void Oscillator::setCouple(float c)
{
	_couple = c;
}


