#ifndef OSCILLATOR_H_INCLUDED
#define OSCILLATOR_H_INCLUDED

#include <math.h>

class Oscillator
{
	public :
		Oscillator();
		Oscillator(float alpha, float beta);
		float operator()(float val); //fonctor so it can be used and passed as a fonction

	private :
		float _out1;
		float _out2;
		float _alpha;
		float _beta;
};

#endif //OSCILLATOR_H_INCLUDED
