#include "scriptlauncher.h"

ScriptLauncher* ScriptLauncher::_instance = NULL;

ScriptLauncher::ScriptLauncher():
	_osc0(NULL),
	_osc1(NULL),
	_choosen_osc(NULL),
	_ip("127.0.0.1"),
	_port(9559),
	_pid(0),
	_joint(2),
	_status(0),
	_naoqipath("/home/law/Documents/EISTI_stuff/NAO/pynaoqi-python2.7-2.1.4.13-linux64"),
	_mainscriptpath("main.py"),
	_pythonpath("/usr/bin/python"),
	_sharedmemorypath("/run/shm/nao.motors-tf"),
	_error("none"),
	_sm(NULL)
{

}

ScriptLauncher* ScriptLauncher::getInstance()
{
	if(_instance == NULL)
	{
		_instance = new ScriptLauncher();
	}
	return _instance;
}

void ScriptLauncher::killInstance()
{
	_instance->disconnect();
	delete _instance;
}

int ScriptLauncher::connect()
{

	if(_choosen_osc == NULL)
	{
		_error = "no oscillator choosen";
		return -1;
	}

	struct sockaddr_in sa;
	int result = inet_pton(AF_INET, (char*)(_ip.c_str()), &(sa.sin_addr));
	if(result != 1)
	{
		_error = "invalid ip address. Note: must be like 127.0.0.1";
		return 1;
	}
	else if(_port <= 0 || _port > 65535)
	{
		_error = "port is either below 1 or above 65535";
		return 2;
	}
	else if(_joint > 9)
	{
		_error = "you must choose a joint from 0 to 9";
		return 3;
	}
	else if(access(_naoqipath.c_str(), F_OK ) != 0)
	{
		_error = _naoqipath + " unfound";
		return 4;
	}
	else if(access(_mainscriptpath.c_str(), F_OK ) != 0)
	{
		_error = _mainscriptpath + " unfound";
		return 5;
	}
	else if(access(_pythonpath.c_str(), F_OK ) != 0)
	{
		_error = _pythonpath + " unfound";
		return 6;
	}


	pipe(_p);
	_pid = fork();

	switch(_pid)
	{
		case -1 :
			_error = "cannot fork";
			return 7;
			break; //useless but meh
		case 0 :
		{ //<-- avoid cross initialization error. google it, it's interesting
			dup2(_p[1], STDERR_FILENO); //duplicate error
			close(_p[0]);
			close(_p[1]);
			setenv("PYTHONPATH", _naoqipath.c_str(), 1);
			char* pargs[] = {"python", "main.py", "--qi-url", (char*)((_ip + ":" + std::to_string(_port)).c_str()), NULL};
			if(execv(_pythonpath.c_str(), pargs) == -1)
			{
				_error = "can't launch script...";
				return 8;
			}
			break;
		}
		default :
			char buffer[4096];
			memset(buffer, '\0', 4096);
			close(_p[1]);
			sleep(1);
			int bytes = read(_p[0], buffer, 4096);
			if(bytes > 0) //if an error has been return immediately
			{
				_error.assign((const char*)buffer);
			}

			int s = this->getStatus();

			if(WIFEXITED(s) == false) //if the child (the script) has not terminated immediately
			{
				_sm = new SharedMemory(_choosen_osc, _joint, _sharedmemorypath);
				_sm->startShare();
			}
			else
			{
				return 9;
			}
			break;
	}
	return 0;
}

int ScriptLauncher::disconnect()
{
	return kill(_pid, SIGKILL);
}

void ScriptLauncher::setIp(std::string ip)
{
	_ip = ip;
}

std::string ScriptLauncher::getIp() const
{
	return _ip;
}

void ScriptLauncher::setPort(int port)
{
	_port = port;
}

int ScriptLauncher::getPort() const
{
	return _port;
}

void ScriptLauncher::setOscillators(Oscillator* osc0, Oscillator* osc1)
{
	_osc0 = osc0;
	_osc1 = osc1;
}

void ScriptLauncher::chooseOscillator(int i)
{
	switch(i)
	{
		case 0:
			_choosen_osc = _osc0;
			break;
		case 1:
			_choosen_osc = _osc1;
			break;
		default:
			break;
	}
}

std::string ScriptLauncher::getChoosenOscillator() const
{
	std::string choice = "UNKNOWN";
	if(_choosen_osc == NULL)
	{
		choice = "NONE";
	}
	else if(_choosen_osc == _osc0)
	{
		choice = "Horizontal";
	}
	else if(_choosen_osc == _osc1)
	{
		choice = "Vertical";
	}

	return choice;
}

void ScriptLauncher::setJoint(int joint)
{
	_joint = joint;
}

int ScriptLauncher::getJoint() const
{
	return _joint;
}

void ScriptLauncher::setNaoqiPath(std::string path)
{
	_naoqipath = path;
}

std::string ScriptLauncher::getNaoqiPath() const
{
	return _naoqipath;
}

void ScriptLauncher::setMainScriptPath(std::string path)
{
	_mainscriptpath = path;
}

std::string ScriptLauncher::getMainScriptPath() const
{
	return _mainscriptpath;
}

void ScriptLauncher::setPythonPath(std::string path)
{
	_pythonpath = path;
}

std::string ScriptLauncher::getPythonPath() const
{
	return _pythonpath;
}

void ScriptLauncher::setSharedMemoryPath(std::string path)
{
	_sharedmemorypath = path;
}

std::string ScriptLauncher::getSharedMemoryPath() const
{
	return _sharedmemorypath;
}

std::string ScriptLauncher::getError() const
{
	return _error;
}

int ScriptLauncher::getPid() const
{
	return _pid;
}

int ScriptLauncher::getStatus() const
{
	waitpid(_pid, (int*)&_status, WNOHANG);
	return _status;
}


















