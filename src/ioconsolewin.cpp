#include "ioconsolewin.h"
#include "ui_ioconsolewin.h"

IOConsoleWin::IOConsoleWin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IOConsoleWin)
{
    Qt::WindowFlags flags = 0;
    flags |= Qt::WindowMaximizeButtonHint;
    //flags |= Qt::WindowContextHelpButtonHint;
    flags |= Qt::WindowCloseButtonHint;
    //flags |= Qt::WindowStaysOnTopHint;
    flags |= Qt::WindowMinimizeButtonHint;
    setWindowFlags(flags);
    ui->setupUi(this);

    connect(ui->ConsoleScreen, SIGNAL(KeyPress(QKeyEvent*)), this, SLOT(KeyPress(QKeyEvent*)));
    connect(ui->ConsoleScreen, SIGNAL(KeyRelease(QKeyEvent*)), this, SLOT(KeyRelease(QKeyEvent*)));
}

IOConsoleWin::~IOConsoleWin()
{
    disconnect(ui->ConsoleScreen, SIGNAL(KeyPress(QKeyEvent*)), this, SLOT(KeyPress(QKeyEvent*)));
    disconnect(ui->ConsoleScreen, SIGNAL(KeyRelease(QKeyEvent*)), this, SLOT(KeyRelease(QKeyEvent*)));
    delete ui;
}

void IOConsoleWin::on_Show0_clicked()
{
    setWindowTitle("Console 0");
    CurrentIOX = -1;
    CurrentIO = 0;
    Refresh();
}

void IOConsoleWin::on_Show1_clicked()
{
    setWindowTitle("Console 1");
    CurrentIOX = -1;
    CurrentIO = 1;
    Refresh();
}

void IOConsoleWin::on_Show2_clicked()
{
    setWindowTitle("Console 2");
    CurrentIOX = -1;
    CurrentIO = 2;
    Refresh();
}

void IOConsoleWin::on_Show3_clicked()
{
    setWindowTitle("Console 3");
    CurrentIOX = -1;
    CurrentIO = 3;
    Refresh();
}

void IOConsoleWin::SetDispFont()
{
    ui->ConsoleScreen->setFont(QFont(Eden::ToQStr(Core->ConsoleFontName), Core->ConsoleFontSize, 0, false));
}

void IOConsoleWin::Refresh()
{
    if (Core->IOConsole_[CurrentIO]->IsChanged() || (CurrentIOX != CurrentIO))
    {
        string T = Core->IOConsole_[CurrentIO]->GetText();
        CurrentIOX = CurrentIO;
        ui->ConsoleScreen->setPlainText(Eden::ToQStr(T));
        ui->ConsoleScreen->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
        ui->ConsoleScreen->verticalScrollBar()->setValue(ui->ConsoleScreen->verticalScrollBar()->maximum());
    }
}

void IOConsoleWin::KeyPress(QKeyEvent * event)
{
    string T = Eden::ToStr(event->text());
    if (T == "")
    {
        return;
    }
    for (uint I = 0; I < T.size(); I++)
    {
        Core->IOConsole_[CurrentIO]->InputCharPut(T[I]);
    }
}

void IOConsoleWin::KeyRelease(QKeyEvent * event)
{
    event++;
    event--;
}

void IOConsoleWin::on_ConsoleClear_clicked()
{
    Core->IOConsole_[CurrentIO]->Clear();
}

void IOConsoleWin::on_ConsoleCopy_clicked()
{
    QGuiApplication::clipboard()->setText("");
    ui->ConsoleScreen->copy();
}

void IOConsoleWin::on_ConsolePaste_clicked()
{
    string T = Eden::ToStr(QGuiApplication::clipboard()->text());
    if (T == "")
    {
        return;
    }
    for (uint I = 0; I < T.size(); I++)
    {
        Core->IOConsole_[CurrentIO]->InputCharPut(T[I]);
    }
}
