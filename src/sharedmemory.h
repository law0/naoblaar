#ifndef SHAREDMEMORY_H_INCLUDED
#define SHAREDMEMORY_H_INCLUDED

#include <iostream>
#include <stdio.h>
#include <thread>
#include <mutex>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>

#include "oscillator.h"

class SharedMemory
{
	public :
        	SharedMemory(const Oscillator* osc, unsigned int joint_choice, std::string path = "/run/shm/nao.motors-tf");
		~SharedMemory();
		void startShare();
		void stopShare();

	private:
		static void share(const Oscillator* osc, float* data, unsigned int joint, bool* running);
		unsigned int _joint;
		const unsigned int _size;
		float* _data;
		const Oscillator* _osc;
		std::thread * _loop;
		bool running;

};

#endif // SHAREDMEMORY_H_INCLUDED
