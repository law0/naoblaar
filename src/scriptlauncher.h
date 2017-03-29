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

		void setIp(std::string ip);
		void setPort(int port);
		void setNaoqiPath(std::string);
		void setMainScriptPath(std::string);
		void setPythonPath(std::string);
		void setJoint(int);
		void setOscillators(Oscillator* oscH, Oscillator* oscV);
		void chooseOscillator(int o);
		void setSharedMemoryPath(std::string);

		int getStatus();
		int getPid() const;
		std::string getIp() const;
		int getPort() const;
		std::string getError() const;
		std::string getNaoqiPath() const;
		std::string getMainScriptPath() const;
		std::string getPythonPath() const;
		std::string getChoosenOscillator() const;
		std::string getSharedMemoryPath() const;
		int getJoint() const;

		void loadConfigFromFile(std::string);
		void saveConfigToFile(std::string);

	private :
		ScriptLauncher();
		Oscillator* _osc0;
		Oscillator* _osc1;
		Oscillator* _choosen_osc;
		std::string _ip;
		int _port;
		int _pid;
		int _joint;
		int _status;
		std::string _naoqipath;
		std::string _mainscriptpath;
		std::string _pythonpath;
		std::string _sharedmemorypath;
		std::string _error;
		int _p[2];
		SharedMemory* _sm; //avoid instantiating this one immediately
		static ScriptLauncher * _instance;



};

#endif //SCRIPTLAUNCHER_H_INCLUDED

