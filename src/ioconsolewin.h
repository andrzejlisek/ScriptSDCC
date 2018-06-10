#ifndef IOCONSOLEWIN_H
#define IOCONSOLEWIN_H

#include <QDialog>
#include "eden.h"
#include "appcore.h"
#include <QScrollBar>
#include <QGuiApplication>
#include <QClipboard>

namespace Ui {
class IOConsoleWin;
}

class IOConsoleWin : public QDialog
{
    Q_OBJECT

public:
    explicit IOConsoleWin(QWidget *parent = 0);
    ~IOConsoleWin();
    AppCore * Core;
    void SetDispFont();
    void Refresh();

private slots:
    void on_Show0_clicked();

    void on_Show1_clicked();

    void on_Show2_clicked();

    void on_Show3_clicked();

    void KeyPress(QKeyEvent * event);

    void KeyRelease(QKeyEvent * event);

    void on_ConsoleClear_clicked();

    void on_ConsoleCopy_clicked();

    void on_ConsolePaste_clicked();

private:
    Ui::IOConsoleWin *ui;
    int CurrentIO = 0;
    int CurrentIOX = -1;
};

#endif // IOCONSOLEWIN_H
