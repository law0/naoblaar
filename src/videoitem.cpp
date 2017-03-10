#include "videoitem.h"

VideoItem::VideoItem(const StreamCatcher* streamcatcher) :
	QGraphicsItem(),
	_timer(),
	_frameptr((unsigned char* const)(streamcatcher->getFramePtr())),
	_width((const unsigned int)(streamcatcher->getWidth())),
	_height((const unsigned int)(streamcatcher->getHeight())),
	_size(_width * _height)
{
	connect(&_timer, SIGNAL(timeout()), this, SLOT(force_repaint()));
	_timer.start(1);
}

void VideoItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
        unsigned char out[_size * 4];

        std::mutex mtx;
        mtx.lock();
        char_bgr_to_rgba(out,_frameptr, _size * 3);
        mtx.unlock();

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
