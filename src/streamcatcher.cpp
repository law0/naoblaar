#include "streamcatcher.h"

StreamCatcher* StreamCatcher::_instance = NULL;
bool StreamCatcher::_loop_bool = false;

StreamCatcher::StreamCatcher() :
	_capture(cvCaptureFromCAM( CV_CAP_ANY )),
	_loop_thread(NULL),
	_current_frame(NULL),
	_buffer_selector(0)
{


	if( _capture == NULL )
	{
	        fprintf( stderr, "ERROR: capture is NULL \n" );
        	getchar();
	}


	IplImage* frame_for_size = cvQueryFrame( _capture );

	_width = frame_for_size->width;
	_height = frame_for_size->height;
	_size =  _width * _height;

	printf("CV: widthStep = %d\n", frame_for_size->widthStep);

	_current_frame = (char*)malloc(frame_for_size->imageSize);

	_bgr_buffer[0] = (unsigned char*)malloc(frame_for_size->imageSize);
	memset(_bgr_buffer[0], 0, frame_for_size->imageSize);
	_bgr_buffer[1] = (unsigned char*)malloc(frame_for_size->imageSize);
	memset(_bgr_buffer[1], 0, frame_for_size->imageSize);

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
		unsigned int frameSize = frame->imageSize;

		mtx.lock();

		memcpy(current_frame, vec, frameSize);
		unsigned char reverse_buffer_selector = (sc->get_buffer_selector() + 1) & 0x01; //when _buffer_selector is 1 _reverse_buffer_selector is 0 and reverse
		memcpy(sc->get_bgr_buffer(reverse_buffer_selector), current_frame, frameSize);
		sc->set_buffer_selector(reverse_buffer_selector);

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

unsigned char* StreamCatcher::getBGR(unsigned char* img, unsigned int size) const
{
	unsigned int img_size = _size * 3;
	if(size != img_size)
	{
		printf("CAUTION StreamCatcher::getBGR() : SIZE DIFFER\n");
	}
	memcpy(img, _bgr_buffer[(int)_buffer_selector], size);
	return img;
}

unsigned char* StreamCatcher::getRGBA(unsigned char* img, unsigned int size) const
{
	unsigned int img_size = _size * 4;
	unsigned int buffer_size = _size * 3;
	if(size != img_size)
	{
		printf("CAUTION StreamCatcher::getRGBA() : SIZE DIFFER\n");
	}

	unsigned char* buffer = _bgr_buffer[(int)_buffer_selector];
        unsigned int i, c;
        for(i = 0, c = 0; i < buffer_size; i += 3, c+=4)
        {
                img[c] = buffer[i+2];
                img[c+1] = buffer[i+1];
                img[c+2] = buffer[i];
                img[c+3] = 255;
        }

	return img;
}

unsigned char* StreamCatcher::getGRAY(unsigned char* img, unsigned int size) const
{
	unsigned int buffer_size = _size * 3;

	if(size != _size)
	{
		printf("CAUTION StreamCatcher::getGRAY() : SIZE DIFFER\n");
	}

	unsigned char* buffer = _bgr_buffer[(int)_buffer_selector];
        unsigned int i, c,  m;
        for(i = 0, c = 0; i < buffer_size; i += 3, ++c)
        {
		m = buffer[i] + buffer[i+1] + buffer[i+2];
		m /= 3;
		img[c] = (unsigned char)m;
        }

	return img;
}

char StreamCatcher::get_buffer_selector() const
{
	return _buffer_selector;
}

void StreamCatcher::set_buffer_selector(char bs)
{
	_buffer_selector = bs;
}

unsigned char* StreamCatcher::get_bgr_buffer(char bs) const
{
	return _bgr_buffer[(int)bs];
}
