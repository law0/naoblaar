#ifndef SCRIPTLAUNCHER_H_INCLUDED
#define SCRIPTLAUNCHER_H_INCLUDED

#include <string>
#include "oscillator.h"

class ScriptLauncher
{

	public :
		static ScriptLauncher* getInstance();
		int connect();
		void disconnect();
		int getStatus();

	private :
		ScriptLauncher();
		Oscillator* osc;
		std::string _ip;
		int _port;
		int _pid;
		int _joint;
		bool _status;
		SharedMemory* _sm; //avoid instantiating this one immediately
		static ScriptLauncher * _instance;



};

#endif //SCRIPTLAUNCHER_H_INCLUDED

