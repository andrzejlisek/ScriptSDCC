#ifndef QPLAINTEXTEDITKEY_H
#define QPLAINTEXTEDITKEY_H

#include <QObject>
#include <QKeyEvent>
#include <QPlainTextEdit>
#include <QDebug>

class QPlainTextEditKey : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit QPlainTextEditKey(QWidget *parent = 0);

signals:
    void KeyPress(QKeyEvent *event);
    void KeyRelease(QKeyEvent *event);

public slots:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // QPLAINTEXTEDITKEY_H
