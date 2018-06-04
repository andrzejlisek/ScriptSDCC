#ifndef IOSPREADSHEETWIN_H
#define IOSPREADSHEETWIN_H

#include <QDialog>
#include "eden.h"
#include "appcore.h"
#include <QGuiApplication>
#include <QClipboard>
#include <sstream>

#define ShowMessage(Message, Title) { QMessageBox Msg; if ((Title) == "") { Msg.setWindowTitle(" "); } else { Msg.setWindowTitle(Eden::ToQStr(Title)); } Msg.setText(Eden::ToQStr(Message)); Msg.exec(); }

namespace Ui {
class IOSpreadsheetWin;
}

class IOSpreadsheetWin : public QDialog
{
    Q_OBJECT

public:
    explicit IOSpreadsheetWin(QWidget *parent = 0);
    ~IOSpreadsheetWin();
    AppCore * Core;
    void Refresh();
    void RefreshVal(int N, int R, int C);

    void RowAdd();
    void RowRem();
    void ColAdd();
    void ColRem();

private slots:
    void on_RowAdd_clicked();

    void on_RowRem_clicked();

    void on_ColAdd_clicked();

    void on_ColRem_clicked();

    void on_Show0_clicked();

    void on_Show1_clicked();

    void on_Show2_clicked();

    void on_Show3_clicked();

    void on_SpreadsheetScreen_cellChanged(int row, int column);

    void on_SheetClear_clicked();

    void on_SheetCopy_clicked();

    void on_SheetPaste_clicked();

private:
    Ui::IOSpreadsheetWin *ui;
    int CurrentIO = 0;
    int CurrentIOX = -1;
    int CurrentRow1();
    int CurrentRow2();
    int CurrentCol1();
    int CurrentCol2();
    bool SelectedCells();
    bool EventEnabled = false;
};

#endif // IOSPREADSHEETWIN_H
