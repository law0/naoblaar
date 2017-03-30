#ifndef MOVIE_H
#define MOVIE_H

#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <thread>
#include <mutex>
#include <chrono>
#include <string>

#include "streamcatcher.h"

using namespace std;
using namespace cv;

class Movie
{
	public :
        	Movie(const StreamCatcher* streamcatcher);
		~Movie();
		void startCapture(string name);
		static void capture(const StreamCatcher * stream, bool* running, string name);
		void stopCapture();

	private:
        	const StreamCatcher* _streamcatcher;
		std::thread * _loop;
		bool _running;
        	const unsigned int _width;
        	const unsigned int _height;
        	const unsigned int _size;

};

#endif // MOVIE_H
