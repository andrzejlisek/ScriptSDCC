#ifndef QMOUSELABEL_H
#define QMOUSELABEL_H

#include <QObject>
#include <QLabel>
#include <QMouseEvent>
#include <iostream>
#include <QPainter>

class QMouseLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QMouseLabel(QWidget *parent = 0);
    QImage * ImgX = NULL;
signals:
    void MouseMove(int Btn, int X, int Y);
    void MousePress(int Btn, int X, int Y);
    void MouseRelease(int Btn, int X, int Y);
public slots:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
};

#endif // QMOUSELABEL_H
