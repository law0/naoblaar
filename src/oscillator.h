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
