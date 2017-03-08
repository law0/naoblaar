#ifndef VIDEOITEM_H_INCLUDED
#define VIDEOITEM_H_INCLUDED

#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <mutex>

#include <QtWidgets>
#include <QObject>
#include <QGraphicsItem>

#include "utilities.h"

class VideoItem : public QObject, public QGraphicsItem
{
	Q_OBJECT

	public :
		VideoItem(unsigned char* frameptr);
		void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
		QRectF boundingRect() const;

	private:
		unsigned char* _frameptr;
		QTimer _timer;


	public slots:
		void force_repaint();

};

#endif // VIDEOITEM_H_INCLUDED