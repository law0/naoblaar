/* Author Lawrencio Rasamoelison, Demailly Benjamin
 * This class inherits QObject and QGraphicsItem
 * this class is used to display the video, live.
 * This class is instanciated in launchview
 * This class use Qt
 */


#ifndef VIDEOITEM_H_INCLUDED
#define VIDEOITEM_H_INCLUDED

#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <mutex>

#include <QtWidgets>
#include <QObject>
#include <QGraphicsItem>

#include "streamcatcher.h"
#include "utilities.h"

class VideoItem : public QObject, public QGraphicsItem
{
	Q_OBJECT

	public :
		VideoItem(const StreamCatcher* streamcatcher);
		void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
		QRectF boundingRect() const;

	private:
		QTimer _timer;
		const StreamCatcher* _streamcatcher;
//		unsigned char* const _frameptr;
		const unsigned int _width;
		const unsigned int _height;
		const unsigned int _size;


	public slots:
		void force_repaint();

};

#endif // VIDEOITEM_H_INCLUDED
