#ifndef STREAMCATCHER_H_INCLUDED
#define STREAMCATCHER_H_INCLUDED

#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <thread>
#include <mutex>

class StreamCatcher
{

	public :
		static StreamCatcher* getInstance();
		static void killInstance();
		std::thread * getThread();
		char* getFramePtr() const;
		CvCapture * getCapture();
		unsigned int getWidth() const;
		unsigned int getHeight() const;

	private:
		StreamCatcher();
		static void loop(StreamCatcher *);

		static StreamCatcher * _instance;
		CvCapture * _capture;
		std::thread * _loop_thread;
		char* _current_frame;
		static bool _loop_bool;
		unsigned int _width;
		unsigned int _height;


};

#endif //STREAMCATCHER_H_INCLUDED
