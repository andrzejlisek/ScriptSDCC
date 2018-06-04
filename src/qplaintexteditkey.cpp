#include "qplaintexteditkey.h"

QPlainTextEditKey::QPlainTextEditKey(QWidget *parent) : QPlainTextEdit(parent)
{

}

void QPlainTextEditKey::keyPressEvent(QKeyEvent * event)
{
    QPlainTextEdit::keyPressEvent(event);
    emit KeyPress(event);
}

void QPlainTextEditKey::keyReleaseEvent(QKeyEvent * event)
{
    QPlainTextEdit::keyReleaseEvent(event);
    emit KeyRelease(event);
}
