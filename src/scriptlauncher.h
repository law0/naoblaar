#ifndef SCRIPTLAUNCHER_H_INCLUDED
#define SCRIPTLAUNCHER_H_INCLUDED

#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include "oscillator.h"
#include "sharedmemory.h"

class ScriptLauncher
{

	public :
		static ScriptLauncher* getInstance();
		static void killInstance();
		int connect();
		int disconnect();
		int getStatus() const;
		void setNaoqiPath(std::string);
		void setMainScriptPath(std::string);
		void setPythonPath(std::string);
		void setJoint(int);
		void setOscillator(Oscillator* osc);
		int getPid() const;
		std::string getError() const;

	private :
		ScriptLauncher();
		Oscillator* _osc;
		std::string _ip;
		int _port;
		int _pid;
		int _joint;
		int _status;
		std::string _naoqipath;
		std::string _mainscriptpath;
		std::string _pythonpath;
		std::string _error;
		int _p[2];
		SharedMemory* _sm; //avoid instantiating this one immediately
		static ScriptLauncher * _instance;



};

#endif //SCRIPTLAUNCHER_H_INCLUDED

