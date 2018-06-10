#include "iospreadsheetwin.h"
#include "ui_iospreadsheetwin.h"

IOSpreadsheetWin::IOSpreadsheetWin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IOSpreadsheetWin)
{
    Qt::WindowFlags flags = 0;
    flags |= Qt::WindowMaximizeButtonHint;
    //flags |= Qt::WindowContextHelpButtonHint;
    flags |= Qt::WindowCloseButtonHint;
    //flags |= Qt::WindowStaysOnTopHint;
    flags |= Qt::WindowMinimizeButtonHint;
    setWindowFlags(flags);
    ui->setupUi(this);
    ui->SpreadsheetScreen->setRowCount(256);
    ui->SpreadsheetScreen->setColumnCount(256);
    for (int I = 0; I < 256; I++)
    {
        ui->SpreadsheetScreen->setVerticalHeaderItem(I, new QTableWidgetItem(""));
        ui->SpreadsheetScreen->setHorizontalHeaderItem(I, new QTableWidgetItem(""));
        ui->SpreadsheetScreen->verticalHeaderItem(I)->setText(Eden::ToQStr(I));
        ui->SpreadsheetScreen->horizontalHeaderItem(I)->setText(Eden::ToQStr(I));
        for (int II = 0; II < 256; II++)
        {
            ui->SpreadsheetScreen->setItem(I, II, new QTableWidgetItem(""));
        }
    }
    EventEnabled = true;
}

IOSpreadsheetWin::~IOSpreadsheetWin()
{
    delete ui;
}

void IOSpreadsheetWin::on_RowAdd_clicked()
{
    RowAdd();
}

void IOSpreadsheetWin::on_RowRem_clicked()
{
    RowRem();
}

void IOSpreadsheetWin::on_ColAdd_clicked()
{
    ColAdd();
}

void IOSpreadsheetWin::on_ColRem_clicked()
{
    ColRem();
}

void IOSpreadsheetWin::RowAdd()
{
    if (SelectedCells())
    {
        Core->IOSpreadsheet_[CurrentIO]->SheetRowAdd(CurrentRow1(), CurrentCol1(), CurrentRow2(), CurrentCol2());
    }
    Refresh();
}

void IOSpreadsheetWin::RowRem()
{
    if (SelectedCells())
    {
        Core->IOSpreadsheet_[CurrentIO]->SheetRowRem(CurrentRow1(), CurrentCol1(), CurrentRow2(), CurrentCol2());
    }
    Refresh();
}

void IOSpreadsheetWin::ColAdd()
{
    if (SelectedCells())
    {
        Core->IOSpreadsheet_[CurrentIO]->SheetColAdd(CurrentRow1(), CurrentCol1(), CurrentRow2(), CurrentCol2());
    }
    Refresh();
}

void IOSpreadsheetWin::ColRem()
{
    if (SelectedCells())
    {
        Core->IOSpreadsheet_[CurrentIO]->SheetColRem(CurrentRow1(), CurrentCol1(), CurrentRow2(), CurrentCol2());
    }
    Refresh();
}

int IOSpreadsheetWin::CurrentRow1()
{
    if (ui->SpreadsheetScreen->rowCount() > 0)
    {
        if (!ui->SpreadsheetScreen->selectedRanges().empty())
        {
            return ui->SpreadsheetScreen->selectedRanges().front().topRow();
        }
    }
    return -1;
}

int IOSpreadsheetWin::CurrentRow2()
{
    if (ui->SpreadsheetScreen->rowCount() > 0)
    {
        if (!ui->SpreadsheetScreen->selectedRanges().empty())
        {
            return ui->SpreadsheetScreen->selectedRanges().front().bottomRow();
        }
    }
    return -1;
}

int IOSpreadsheetWin::CurrentCol1()
{
    if (ui->SpreadsheetScreen->columnCount() > 0)
    {
        if (!ui->SpreadsheetScreen->selectedRanges().empty())
        {
            return ui->SpreadsheetScreen->selectedRanges().front().leftColumn();
        }
    }
    return -1;
}

int IOSpreadsheetWin::CurrentCol2()
{
    if (ui->SpreadsheetScreen->columnCount() > 0)
    {
        if (!ui->SpreadsheetScreen->selectedRanges().empty())
        {
            return ui->SpreadsheetScreen->selectedRanges().front().rightColumn();
        }
    }
    return -1;
}

bool IOSpreadsheetWin::SelectedCells()
{
    int R1 = CurrentRow1();
    int R2 = CurrentRow2();
    int C1 = CurrentCol1();
    int C2 = CurrentCol2();
    if ((R1 >= 0) && (R2 >= 0) && (R1 <= 255) && (R2 <= 255))
    {
        if ((C1 >= 0) && (C2 >= 0) && (C1 <= 255) && (C2 <= 255))
        {
            if ((R1 <= R2) && (C1 <= C2))
            {
                return true;
            }
        }
    }
    return false;
}

void IOSpreadsheetWin::on_Show0_clicked()
{
    setWindowTitle("Spreadsheet 0");
    CurrentIOX = -1;
    CurrentIO = 0;
    Refresh();
}

void IOSpreadsheetWin::on_Show1_clicked()
{
    setWindowTitle("Spreadsheet 1");
    CurrentIOX = -1;
    CurrentIO = 1;
    Refresh();
}

void IOSpreadsheetWin::on_Show2_clicked()
{
    setWindowTitle("Spreadsheet 2");
    CurrentIOX = -1;
    CurrentIO = 2;
    Refresh();
}

void IOSpreadsheetWin::on_Show3_clicked()
{
    setWindowTitle("Spreadsheet 3");
    CurrentIOX = -1;
    CurrentIO = 3;
    Refresh();
}

void IOSpreadsheetWin::SetDispFont()
{
    EventEnabled = false;
    QFont F = QFont(Eden::ToQStr(Core->SpreadsheetFontName), Core->SpreadsheetFontSize, 0, false);
    ui->SpreadsheetScreen->setFont(F);
    for (int I = 0; I < 256; I++)
    {
        ui->SpreadsheetScreen->verticalHeaderItem(I)->setFont(F);
        ui->SpreadsheetScreen->horizontalHeaderItem(I)->setFont(F);
        for (int II = 0; II < 256; II++)
        {
            ui->SpreadsheetScreen->item(I, II)->setFont(F);
        }
        ui->SpreadsheetScreen->resizeRowToContents(I);
    }
    EventEnabled = true;
}

void IOSpreadsheetWin::Refresh()
{
    EventEnabled = false;
    for (int R = 0; R < 256; R++)
    {
        for (int C = 0; C < 256; C++)
        {
            ui->SpreadsheetScreen->item(R, C)->setText(Eden::ToQStr(Core->IOSpreadsheet_[CurrentIO]->CellGet(R, C)));
        }
    }
    EventEnabled = true;
}

void IOSpreadsheetWin::RefreshVal(int N, int R, int C)
{
    EventEnabled = false;
    if (CurrentIO == N)
    {
        ui->SpreadsheetScreen->item(R, C)->setText(Eden::ToQStr(Core->IOSpreadsheet_[CurrentIO]->CellGet(R, C)));
    }
    EventEnabled = true;
}

void IOSpreadsheetWin::on_SpreadsheetScreen_cellChanged(int row, int column)
{
    if (EventEnabled)
    {
        Core->IOSpreadsheet_[CurrentIO]->CellSet(row, column, Eden::ToStr(ui->SpreadsheetScreen->item(row, column)->text()));
    }
}

void IOSpreadsheetWin::on_SheetClear_clicked()
{
    if (SelectedCells())
    {
        Core->IOSpreadsheet_[CurrentIO]->SheetClear(CurrentRow1(), CurrentCol1(), CurrentRow2(), CurrentCol2());
    }
    Refresh();
}

void IOSpreadsheetWin::on_SheetCopy_clicked()
{
    int R1 = CurrentRow1();
    int R2 = CurrentRow2();
    int C1 = CurrentCol1();
    int C2 = CurrentCol2();
    if (SelectedCells())
    {
        stringstream SS;
        for (int R = R1; R <= R2; R++)
        {
            for (int C = C1; C <= C2; C++)
            {
                SS << Core->IOSpreadsheet_[CurrentIO]->CellGet(R, C);
                if (C < C2)
                {
                    SS << "\t";
                }
            }
            SS << endl;
        }
        QGuiApplication::clipboard()->setText(Eden::ToQStr(SS.str()));
    }
}

void IOSpreadsheetWin::on_SheetPaste_clicked()
{
    int R1 = CurrentRow1();
    int R2 = CurrentRow2();
    int C1 = CurrentCol1();
    int C2 = CurrentCol2();
    if (SelectedCells())
    {
        int R = R1;
        int C = C1;
        string S = Eden::ToStr(QGuiApplication::clipboard()->text());
        int L = S.length();
        char X;

        string Buf = "";
        for (int I = 0; I < L; I++)
        {
            X = S[I];
            if ((X == '\t') || (X == '\r') || (X == '\n'))
            {
                if ((R <= R2) && (C <= C2))
                {
                    Core->IOSpreadsheet_[CurrentIO]->CellSet(R, C, Buf);
                }
                Buf = "";
                if (X == '\t')
                {
                    C++;
                }
                else
                {
                    if (X == '\r')
                    {
                        R++;
                        C = C1;
                        if ((I + 1) < L)
                        {
                            if (S[I + 1] == '\n')
                            {
                                I++;
                            }
                        }
                    }
                    else
                    {
                        if (X == '\n')
                        {
                            R++;
                            C = C1;
                        }
                    }
                }
            }
            else
            {
                Buf = Buf + X;
            }
        }
        if (Buf != "")
        {
            if ((R <= R2) && (C <= C2))
            {
                Core->IOSpreadsheet_[CurrentIO]->CellSet(R, C, Buf);
            }
        }
    }
    Refresh();
}
