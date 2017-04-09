/* Author Lawrencio Rasamoelison, Demailly Benjamin
 * see videoitem.h
 */

#include "videoitem.h"

VideoItem::VideoItem(const StreamCatcher* streamcatcher) :
	QGraphicsItem(),
	_timer(),
	_streamcatcher(streamcatcher),
	_width((const unsigned int)(_streamcatcher->getWidth())),
	_height((const unsigned int)(_streamcatcher->getHeight())),
	_size(_width * _height)
{
	connect(&_timer, SIGNAL(timeout()), this, SLOT(force_repaint()));
	_timer.start(1);
}

void VideoItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
        unsigned char out[_size * 4];

	_streamcatcher->getRGBA(out, _size * 4);

        QImage image(out, _width, _height, QImage::Format_RGBA8888_Premultiplied);

        painter->drawImage(QRectF(QPoint(0, 0), QSize(_width, _height)), image.mirrored(true, false), QRectF(QPoint(0, 0), image.size()));
}

QRectF VideoItem::boundingRect() const
{
	return QRectF(this->pos(), QSize(_width, _height));
}

void VideoItem::force_repaint()
{
	this->update();
}
