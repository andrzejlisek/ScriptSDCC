#include "qmouselabel.h"

QMouseLabel::QMouseLabel(QWidget *parent) : QLabel(parent)
{
    setMouseTracking(false);
}

void QMouseLabel::mouseMoveEvent(QMouseEvent *event)
{
    emit MouseMove(event->buttons(), event->x(), event->y());
}

void QMouseLabel::mousePressEvent(QMouseEvent *event)
{
    emit MousePress(event->buttons(), event->x(), event->y());
}

void QMouseLabel::mouseReleaseEvent(QMouseEvent *event)
{
    emit MouseRelease(event->buttons(), event->x(), event->y());
}

void QMouseLabel::paintEvent(QPaintEvent *event)
{
    if (ImgX != NULL)
    {
        QPainter painter(this);
        painter.drawImage(0, 0, *ImgX);
    }
    else
    {
        QLabel::paintEvent(event);
    }
}
