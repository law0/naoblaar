#ifndef SAVEDATA_H
#define SAVEDATA_H

#include <iostream>
#include <stdio.h>
#include <thread>
#include <fstream>
#include <unistd.h>
#include <string> 

#include "oscillator.h"

using namespace std;

class Savedata
{
	public :
        	Savedata(const Oscillator* osc1, const Oscillator* osc2);
		~Savedata();
		void startSave();
		static void save(const Oscillator* oscH, const Oscillator* oscV, bool* _running);
		void stopSave();

	private:
        	const Oscillator* _oscH;
		const Oscillator* _oscV;
		std::thread * _loop;
		bool _running;
		//String _title;
};

#endif // SAVEDATA_H
