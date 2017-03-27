#include "scriptlauncher.h"

ScriptLauncher* ScriptLauncher::_instance = NULL;

ScriptLauncher::ScriptLauncher():
	_ip("127.0.0.1"),
	_port(0),
	_pid(0),
	_joint(2),
	_status(false),
	_sm(NULL)
{

}

int ScriptLauncher::connect()
{
	
}

