/* Author Lawrencio Rasamoelison, Demailly Benjamin
 * see sharedmemory.h
 */

#include "sharedmemory.h"

SharedMemory::SharedMemory(const Oscillator* osc, unsigned int joint_choice, std::string path) :
	_joint(joint_choice),
	_path(path),
	_size(20),
	_osc(osc),
	_loop(NULL),
	running(false),
	_fd((int*)malloc(sizeof(int)))
{
	int fd;

	if ((fd = open(_path.c_str(), O_RDWR | O_TRUNC | O_CREAT, (mode_t)0600)) == -1)
	{
        	perror("open");
	        exit(1);
	}

/*	for(unsigned int i = 0; i < _size * sizeof(float); ++i)
	{
		write(fd, 0, 1);
	}*/

	int pf;
	if(( pf = posix_fallocate(fd, 0, _size * sizeof(float))) != 0)
	{
		printf("error %d :", pf);
		perror("posix fallocate");
		exit(1);
	}


    	_data = (float*)mmap((caddr_t)0, _size * sizeof(float), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if ((char*)_data == (caddr_t)(-1))
	{
        	perror("mmap");
	        exit(1);
	}

	*_fd = fd;

}

SharedMemory::~SharedMemory()
{
	this->stopShare();
	munmap(_data, _size * sizeof(float));
	close(*_fd);
	free(_fd);
}

void SharedMemory::startShare()
{
	if (!running)
	{
		running = true;
		_loop = new std::thread(SharedMemory::share, _osc, _fd, _data, _joint, &running);
	}
}


void SharedMemory::share(const Oscillator* osc, int* fd, float* data, const unsigned int joint, bool* running)
{
	std::mutex mtx;
   	while (*running)
    	{
		if(joint < 10)
		{
			mtx.lock();
			flock(*fd, LOCK_EX);
			data[joint] = osc->getOut();
			flock(*fd, LOCK_UN);
			mtx.unlock();
		}
    	}
}

void SharedMemory::stopShare()
{
	running = false;
	if(_loop != NULL)
	{
		_loop->join();
		delete _loop;
		_loop = NULL;
	}
}
