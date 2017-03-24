#include "movie.h"

Movie::Movie(const StreamCatcher* sc) :
    _streamcatcher(sc),
	_loop(NULL),
    running(false),
    _width((const unsigned int)(_streamcatcher->getWidth())),
    _height((const unsigned int)(_streamcatcher->getHeight())),
    _size(_width * _height)
{

}

void Movie::startCapture()
{
	if (!running)
	{
		running = true;
		_loop = new std::thread(Movie::capture, _streamcatcher, &running);
	}
}


void Movie::capture(const StreamCatcher * stream, bool* running)
{
	const unsigned int width = stream->getWidth();
	const unsigned int height = stream->getHeight();
	const unsigned int size = height * width;

    	//running = true;

	const double fps = 60.0;
	const std::chrono::nanoseconds wait_fps(1000000000 / (int)(fps * 1.22)); //despite error correction we have a 22% error for NO REASON
	std::chrono::nanoseconds waiting_time(0);
	std::chrono::nanoseconds error_waiting_time(0);

	VideoWriter video("out.avi",CV_FOURCC('M','J','P','G'), fps, Size(width, height),true);
    	unsigned char out[size * 3];
	std::chrono::high_resolution_clock::time_point finish;
	std::chrono::high_resolution_clock::time_point start;

    	while (*running)
    	{

	       	stream->getBGR(out, size * 3);
		// out is the data and I need to be good in Mat
		//Mat(int heigth, int width, int type (char), out, size_t step
		Mat mat(height, width, CV_8UC3 , out);
		//we need to substract the error from the wanted waiting time
		video.write(mat);

		finish = std::chrono::high_resolution_clock::now();

			waiting_time = wait_fps - std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start);
			std::this_thread::sleep_for(waiting_time - error_waiting_time);

		start = std::chrono::high_resolution_clock::now();

		error_waiting_time = std::chrono::duration_cast<std::chrono::nanoseconds>(start - finish) - waiting_time;
    	}
	video.release();

}

void Movie::stopCapture()
{
	if (running)
	{
		running = false;
	}
    //printf("salut");
}
