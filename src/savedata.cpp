#include "savedata.h"

Savedata::Savedata(const Oscillator* osc1, const Oscillator* osc2) :
    	_oscH(osc1),
	_oscV(osc2),
	_loop(NULL),
    	_running(false)
{

}

void Savedata::startSave(string place, string title)
{
	place.append("/");
	if (!_running)
	{
		_running = true;
		_loop = new std::thread(Savedata::save, _oscH, _oscV, &_running, place, title);
	}
}


void Savedata::save(const Oscillator* oscH, const Oscillator* oscV, bool* _running, string place, string name)
{
	//string name("test");
	if (name == "")
	{
		name = "test";
	}
	string endD(".csv");
	string endM(".avi");
	string titleD = place + name + endD;
	string titleM = place + name + endM;
	int i = 1;
	FILE * data = fopen(titleD.c_str(), "r");
	FILE * movie = fopen(titleM.c_str(), "r");
	while ((data = fopen(titleD.c_str(), "r")) || (movie = fopen(titleM.c_str(), "r")))
	{
		if (data != NULL)
		{
			fclose(data);
		}
		if (movie != NULL)
		{
			fclose(movie);
		}

		titleD = place + name + to_string(i) + endD;
		titleM = place + name + to_string(i) + endM;
		i++;
	}

	if (data != NULL)
	{
		fclose(data);
	}
	if (movie != NULL)
	{
		fclose(movie);
	}


	ofstream myfile(titleD, ios::app);
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
		_loop->join();
		delete _loop;
	}
    //printf("salut");
}

Savedata::~Savedata()
{
	this->stopSave();
}
