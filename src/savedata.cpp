#include "savedata.h"

Savedata::Savedata(const Oscillator* osc1, const Oscillator* osc2) :
    	_oscH(osc1),
	_oscV(osc2),
	_loop(NULL),
    	_running(false)
{

}

void Savedata::startSave()
{
	if (!_running)
	{
		_running = true;
		_loop = new std::thread(Savedata::save, _oscH, _oscV, &_running);
	}
}


void Savedata::save(const Oscillator* oscH, const Oscillator* oscV, bool* _running)
{
	string name("test");
	string end(".csv");
	string title = name + end;
	int i = 1;
	while (FILE *file = fopen(title.c_str(), "r"))
	{
		fclose(file);
		title = name + to_string(i) + end;
		i++;
	}
	ofstream myfile(title, ios::app);
  	//myfile.open() ;

	if (!myfile.is_open())
	{
		printf("Problème pour enregistrer les données...\n");
		*_running = false;
	}

	while (*_running)
    	{
		usleep(1000);
		float outH = oscH->getOut();
		float valH = oscH->getVal();

		float outV = oscV->getOut();
		float valV = oscV->getVal();

		myfile << outH << ", " << valH << ", " << outV << ", " << valV << ",\n";
    	}

	myfile.close();
}

void Savedata::stopSave()
{
	if (_running)
	{
		_running = false;
	}
    //printf("salut");
}
