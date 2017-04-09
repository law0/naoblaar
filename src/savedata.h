/* Author Lawrencio Rasamoelison, Demailly Benjamin
 * This class is used to record data.
 * It records, in a csv file, the following data (in order in 4 columns):
 * Horizontal optical flow, Horizontal oscillator output, Vertical
 * optical flow, Vertical oscillator output
 * Instanciated in launchview.cpp
 */


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
		void startSave(string name);
		static void save(const Oscillator* oscH, const Oscillator* oscV, bool* _running, string name);
		void stopSave();

	private:
        	const Oscillator* _oscH;
		const Oscillator* _oscV;
		std::thread * _loop;
		bool _running;
		//String _title;
};

#endif // SAVEDATA_H
