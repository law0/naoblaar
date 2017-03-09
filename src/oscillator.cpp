#include "oscillator.h"

Oscillator::Oscillator() : _out1(1.f), _out2(1.f), _alpha(0.07f), _beta(0.15f)
{

}

Oscillator::Oscillator(float alpha, float beta) : _out1(1.f), _out2(1.f), _alpha(alpha), _beta(beta)
{

}

float Oscillator::operator()(float _val)
{
	_out1 = _out1 - _beta * _out2 + _alpha + _val;
	_out2 = _out1 + _beta * _out2 + _alpha;
	return _out1;
}


