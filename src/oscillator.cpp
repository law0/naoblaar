#include "oscillator.h"

Oscillator::Oscillator() : _out1(0.f), _out2(0.f), _alpha(0.07f), _beta(0.15f)
{

}

Oscillator::Oscillator(float alpha, float beta) : _out1(0.f), _out2(0.f), _alpha(alpha), _beta(beta)
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

float Oscillator::operator()(float val)
{
	_val = val;
	float tmp_out1 = _out1 - _beta * _out2 + _alpha + 0.2f * _val;
	_out2  = _out2 >  1.f ? 1.f : _out2;
	_out2  = _out2 <  0.f ? 0.f : _out2;
	_out2 = _out2 + _beta * _out1;
	_out1 = tmp_out1;

	return _out1;
}


