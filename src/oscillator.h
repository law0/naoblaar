/* Author Lawrencio Rasamoelison, Demailly Benjamin
 * This class represent the oscillator such as described in
 * the "rapport final" of our end of studies project.
 * For convenience this class is also a functor,
 * thus, it can be used as a simple function
 * This class is also used as an interface for a several number of
 * widgets, because it contains most of the data that needs to
 * shown on screen and controled.
 * (Some objects using this one are designed in such manner that
 * they can "choose" which getter or setter they will control.
 * For such aim, we provided two enumeration Oscillator::Getter
 * and Oscillator::Setter, to make things more clear.)
 * Instantiated in main.cpp
 */


#ifndef OSCILLATOR_H_INCLUDED
#define OSCILLATOR_H_INCLUDED

#include <math.h>

class Oscillator
{



	public :
		Oscillator();
		Oscillator(float alpha, float beta);
		float operator()(float val); //fonctor so it can be used and passed as a fonction
		float getOut() const;
		float getVal() const;
		float getAlpha() const;
		float getBeta() const;
		float getCouple() const;
		float setVal(float val);
		void setAlpha(float);
		void setBeta(float);
		void setCouple(float);

		enum Getter {GetOut, GetVal, GetAlpha, GetBeta, GetCouple};
		enum Setter {SetVal, SetAlpha, SetBeta, SetCouple};

	private :
		float _out1;
		float _out2;
		float _alpha;
		float _beta;
		float _val;
		float _couple;
};

#endif //OSCILLATOR_H_INCLUDED
