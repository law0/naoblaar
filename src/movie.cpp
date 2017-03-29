#include "movie.h"

Movie::Movie(const StreamCatcher* sc) :
    _streamcatcher(sc),
	_loop(NULL),
    _running(false),
    _width((const unsigned int)(_streamcatcher->getWidth())),
    _height((const unsigned int)(_streamcatcher->getHeight())),
    _size(_width * _height)
{

}

void Movie::startCapture()
{
	if (!_running)
	{
		_running = true;
		_loop = new std::thread(Movie::capture, _streamcatcher, &_running);
	}
}


void Movie::capture(const StreamCatcher * stream, bool* _running)
{
	const unsigned int width = stream->getWidth();
	const unsigned int height = stream->getHeight();
	const unsigned int size = height * width;

    	//running = true;

	const double fps = 60.0;
	const std::chrono::nanoseconds wait_fps((int)(1000000000 / fps)); //despite error correction we have a 22% error for NO REASON
	std::chrono::nanoseconds error(0);

	VideoWriter video("out.avi",CV_FOURCC('M','J','P','G'), fps, Size(width, height),true);
    	unsigned char out[size * 3];
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
	error = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start);
	finish = std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(std::chrono::nanoseconds(0));
		start = finish;
	start = std::chrono::high_resolution_clock::now();
	error += std::chrono::duration_cast<std::chrono::nanoseconds>(start - finish); //overtime error of sleep_for, affectation and high_resolution_clock::now() call

    	while (*_running)
    	{

		stream->getBGR(out, size * 3);
		// out is the data and I need to be good in Mat
		//Mat(int heigth, int width, int type (char), out, size_t step
		Mat mat(height, width, CV_8UC3 , out);
		//we need to substract the error from the wanted waiting time

		finish = std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(wait_fps - 0.5 * std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start));
		start = finish;
		video.write(mat);


    	}
	video.release();

}

void Movie::stopCapture()
{
	if (_running)
	{
		_running = false;
		_loop->join();
		delete _loop;
	}
    //printf("salut");
}

Movie::~Movie()
{
	this->stopCapture();
}
