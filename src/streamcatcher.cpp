#include "streamcatcher.h"

StreamCatcher* StreamCatcher::_instance = NULL;
bool StreamCatcher::_loop_bool = false;

StreamCatcher::StreamCatcher() :
	_capture(cvCaptureFromCAM( CV_CAP_ANY )),
	_loop_thread(NULL),
	_current_frame(NULL)
{


	if( _capture == NULL )
	{
	        fprintf( stderr, "ERROR: capture is NULL \n" );
        	getchar();
	}


	IplImage* frame_for_size = cvQueryFrame( _capture );

	_width = frame_for_size->width;
	_height = frame_for_size->height;

	printf("%d\n", frame_for_size->widthStep);

	_current_frame = (char*)malloc(frame_for_size->imageSize);

	_loop_bool = true;

	_loop_thread = new std::thread(StreamCatcher::loop, this);
}

void StreamCatcher::killInstance()
{
	_loop_bool = false;
	std::thread* t = _instance->getThread();
	t->join();
	delete t;

	char* current_frame = _instance->getFramePtr();
	free(current_frame);

	delete _instance;
}

StreamCatcher* StreamCatcher::getInstance()
{
	if(_instance == NULL)
	{
		_instance = new StreamCatcher();
	}
	return _instance;
}

std::thread * StreamCatcher::getThread()
{
	return _loop_thread;
}


void StreamCatcher::loop(StreamCatcher * sc)
{

	char* current_frame = sc->getFramePtr();
	CvCapture* capture = sc->getCapture();
	std::mutex mtx;
	while ( _loop_bool )
	{
		IplImage* frame = cvQueryFrame( capture );
		if ( frame == NULL )
		{
			fprintf( stderr, "ERROR: frame is null...\n" );
			getchar();
			break;
		}

		char* vec = frame->imageData;

		mtx.lock();
		memcpy(current_frame, vec, frame->imageSize);
		mtx.unlock();
	}

}


char * StreamCatcher::getFramePtr() const
{
	return _current_frame;
}


CvCapture * StreamCatcher::getCapture()
{
	return _capture;
}

unsigned int StreamCatcher::getWidth() const
{
	return _width;
}

unsigned int StreamCatcher::getHeight() const
{
	return _height;
}
