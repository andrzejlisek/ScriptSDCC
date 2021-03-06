#include "iospreadsheet.h"

IOSpreadsheet::IOSpreadsheet()
{
    Data = NEWARR(string, string[65536]);
    Clear();
}

IOSpreadsheet::~IOSpreadsheet()
{
    DELARR(string, Data);
}

void IOSpreadsheet::Clear()
{
    MTX.lock();
    for (int I = 0; I < 65536; I++)
    {
        Data[I] = "";
    }
    MTX.unlock();
}

void IOSpreadsheet::CellSet(int R, int C, string V)
{
    MTX.lock();
    CellSet_(R, C, V);
    MTX.unlock();
}

void IOSpreadsheet::CellSet_(int R, int C, string V)
{
    char X;
    for (int I = (V.length() - 1); I >= 0; I--)
    {
        X = V[I];
        if ((X == '\0') || (X == '\a') || (X == '\b') || (X == '\f') || (X == '\n') || (X == '\r') || (X == '\t') || (X == '\v'))
        {
            V[X] = ' ';
        }
    }
    Data[(R << 8) + C] = V;
    ChangedR.push(R);
    ChangedC.push(C);
}

void IOSpreadsheet::CellSet0(int R, int C, string V)
{
    MTX.lock();
    Data[(R << 8) + C] = V;
    MTX.unlock();
}

void IOSpreadsheet::CellSet(int R, int C, char V)
{
    MTX.lock();
    Data[(R << 8) + C] = V;
    ChangedR.push(R);
    ChangedC.push(C);
    MTX.unlock();
}

void IOSpreadsheet::CellSet(int R, int C, uchar V)
{
    MTX.lock();
    Data[(R << 8) + C] = V;
    ChangedR.push(R);
    ChangedC.push(C);
    MTX.unlock();
}

string IOSpreadsheet::CellGet_(int R, int C)
{
    string S = Data[(R << 8) + C];
    return S;
}

string IOSpreadsheet::CellGet(int R, int C)
{
    MTX.lock();
    string S = Data[(R << 8) + C];
    MTX.unlock();
    return S;
}

bool IOSpreadsheet::CellChanged(int &R, int &C)
{
    MTX.lock();
    if ((ChangedR.size() > 0) && (ChangedC.size() > 0))
    {
        R = ChangedR.front();
        C = ChangedC.front();
        ChangedR.pop();
        ChangedC.pop();
        MTX.unlock();
        return true;
    }
    else
    {
        MTX.unlock();
        return false;
    }
}

void IOSpreadsheet::SheetClear(int R1, int C1, int R2, int C2)
{
    MTX.lock();
    for (int R = R1; R <= R2; R++)
    {
        for (int C = C1; C <= C2; C++)
        {
            CellSet_(R, C, "");
        }
    }
    MTX.unlock();
}

void IOSpreadsheet::SheetRowAdd(int R1, int C1, int R2, int C2)
{
    MTX.lock();
    int X = R1;
    int X0 = R2;
    int X1 = C1;
    int X2 = C2;
    if ((X < 0) || (X1 < 0) || (X2 < X1))
    {
        MTX.unlock();
        return;
    }
    string V;
    for (int C = X1; C <= X2; C++)
    {
        for (int R = (X0 - 1); R >= X; R--)
        {
            V = CellGet_(R, C);
            CellSet_(R + 1, C, V);
        }
        CellSet_(X, C, "");
    }
    MTX.unlock();
}

void IOSpreadsheet::SheetRowRem(int R1, int C1, int R2, int C2)
{
    MTX.lock();
    int X = R1;
    int X0 = R2;
    int X1 = C1;
    int X2 = C2;
    if ((X < 0) || (X1 < 0) || (X2 < X1))
    {
        MTX.unlock();
        return;
    }
    string V;
    for (int C = X1; C <= X2; C++)
    {
        for (int R = X; R <= (X0 - 1); R++)
        {
            V = CellGet_(R + 1, C);
            CellSet_(R, C, V);
        }
        CellSet_(X0, C, "");
    }
    MTX.unlock();
}

void IOSpreadsheet::SheetColAdd(int R1, int C1, int R2, int C2)
{
    MTX.lock();
    int X = C1;
    int X0 = C2;
    int X1 = R1;
    int X2 = R2;
    if ((X < 0) || (X1 < 0) || (X2 < X1))
    {
        MTX.unlock();
        return;
    }
    string V;
    for (int R = X1; R <= X2; R++)
    {
        for (int C = (X0 - 1); C >= X; C--)
        {
            V = CellGet_(R, C);
            CellSet_(R, C + 1, V);
        }
        CellSet_(R, X, "");
    }
    MTX.unlock();
}

void IOSpreadsheet::SheetColRem(int R1, int C1, int R2, int C2)
{
    MTX.lock();
    int X = C1;
    int X0 = C2;
    int X1 = R1;
    int X2 = R2;
    if ((X < 0) || (X1 < 0) || (X2 < X1))
    {
        MTX.unlock();
        return;
    }
    string V;
    for (int R = X1; R <= X2; R++)
    {
        for (int C = X; C <= (X0 - 1); C++)
        {
            V = CellGet_(R, C + 1);
            CellSet_(R, C, V);
        }
        CellSet_(R, X0, "");
    }
    MTX.unlock();
}
