#ifndef STREAMCATCHER_H_INCLUDED
#define STREAMCATCHER_H_INCLUDED

#include <unistd.h>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <thread>
#include <mutex>
#include <vector>
#include "utilities.h"

class StreamCatcher
{

	public :
		static StreamCatcher* getInstance(int device = CV_CAP_ANY);
		static void killInstance();
		static bool isInstantiated();
		static std::vector<int> getListCameras();
		std::thread * getThread();
		char* getFramePtr() const;
		CvCapture * getCapture();
		unsigned int getWidth() const;
		unsigned int getHeight() const;
		unsigned char* getBGR(unsigned char* img, unsigned int size) const; //thread safe and always return fast as it is double buffered
		unsigned char* getRGBA(unsigned char* img, unsigned int size) const; //thread safe and always return fast as it is double buffered
		unsigned char* getGRAY(unsigned char* img, unsigned int size) const; //thread safe and always return fast as it is double buffered

	private:
		StreamCatcher(int device = CV_CAP_ANY);
		static void loop(StreamCatcher *);
		char get_buffer_selector() const;
		void set_buffer_selector(char bs);
		unsigned char* get_bgr_buffer(char bs) const;

		static StreamCatcher * _instance;
		CvCapture * _capture;
		std::thread * _loop_thread;
		char* _current_frame;
		static bool _loop_bool;
		unsigned int _width;
		unsigned int _height;
		unsigned int _size;
		char _buffer_selector;
		unsigned char* _bgr_buffer[2];



};

#endif //STREAMCATCHER_H_INCLUDED
