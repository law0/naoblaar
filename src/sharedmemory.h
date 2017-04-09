/* Author Lawrencio Rasamoelison, Demailly Benjamin
 * This class is used by the scriptlauncher class
 * The external script use a shared memory file from
 * which it takes the variables to make NAO moves.
 * the shared memory file is created via open fallocate and mmap
 * the script reads 10 float values from the shared memory file,
 * corresponding to the 10 joint (from 0 to 9) that is possible to move
 * from the script.
 * So the shared memory file must be at least of 40 bytes of length.
 * In our application we fallocate a 80 bytes long shared memory file.
 * Instantiated in scriptlauncher.cpp
 */


#ifndef SHAREDMEMORY_H_INCLUDED
#define SHAREDMEMORY_H_INCLUDED

#include <iostream>
#include <stdio.h>
#include <thread>
#include <mutex>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
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
		static void share(const Oscillator* osc, int* fd, float* data, unsigned int joint, bool* running);
		unsigned int _joint;
		std::string _path;
		const unsigned int _size;
		float* _data;
		const Oscillator* _osc;
		std::thread * _loop;
		bool running;
		int* _fd;

};

#endif // SHAREDMEMORY_H_INCLUDED
