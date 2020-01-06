#ifndef IOSPREADSHEET_H
#define IOSPREADSHEET_H

#include <iostream>
#include <QMessageBox>
#include "eden.h"
#include <queue>
#include <mutex>

using namespace std;

class IOSpreadsheet
{
public:
    IOSpreadsheet();
    ~IOSpreadsheet();
    bool Changed = false;
    void Clear();
    void CellSet(int R, int C, string V);
    void CellSet_(int R, int C, string V);
    void CellSet0(int R, int C, string V);
    void CellSet(int R, int C, char V);
    void CellSet(int R, int C, uchar V);
    string CellGet(int R, int C);
    string CellGet_(int R, int C);
    bool CellChanged(int &R, int &C);

    void SheetClear(int R1, int C1, int R2, int C2);
    void SheetRowAdd(int R1, int C1, int R2, int C2);
    void SheetRowRem(int R1, int C1, int R2, int C2);
    void SheetColAdd(int R1, int C1, int R2, int C2);
    void SheetColRem(int R1, int C1, int R2, int C2);
private:
    mutex MTX;
    queue<uchar> ChangedR;
    queue<uchar> ChangedC;
    string * Data;
};

#endif // IOSPREADSHEET_H
