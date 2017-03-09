#include "videoitem.h"

VideoItem::VideoItem(unsigned char* frameptr) :
	QGraphicsItem(),
	_timer()
{
	_frameptr = frameptr;
	connect(&_timer, SIGNAL(timeout()), this, SLOT(force_repaint()));
	_timer.start(1);
}

void VideoItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	unsigned int size = global_WIDTH * global_HEIGHT;

        unsigned char out[size * 4];

        std::mutex mtx;
        mtx.lock();
        char_bgr_to_rgba(out, (unsigned char *)_frameptr, size * 3);
        mtx.unlock();

        QImage image(out, global_WIDTH, global_HEIGHT, QImage::Format_RGBA8888_Premultiplied);

        painter->drawImage(QRectF(QPoint(0, 0), QSize(global_WIDTH, global_HEIGHT)), image.mirrored(true, false), QRectF(QPoint(0, 0), image.size()));
}

QRectF VideoItem::boundingRect() const
{
	return QRectF(this->pos(), QSize(global_WIDTH, global_HEIGHT));
}

void VideoItem::force_repaint()
{
	this->update();
}
