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

	VideoWriter video("out.avi",CV_FOURCC('M','J','P','G'), 190, Size(width, height),true);
    	unsigned char out[size * 3];
    	while (*running)
    	{
       	stream->getBGR(out, size * 3);
		// out is the data and I need to be good in Mat
		//Mat(int heigth, int width, int type (char), out, size_t step
		Mat mat(height, width, CV_8UC3 , out);
		video.write(mat);
		
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
