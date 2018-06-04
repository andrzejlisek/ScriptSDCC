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
    for (int I = 0; I < 65536; I++)
    {
        Data[I] = "";
    }
}

void IOSpreadsheet::CellSet(int R, int C, string V)
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
    Data[(R << 8) + C] = V;
}

void IOSpreadsheet::CellSet(int R, int C, char V)
{
    Data[(R << 8) + C] = V;
    ChangedR.push(R);
    ChangedC.push(C);
}

void IOSpreadsheet::CellSet(int R, int C, uchar V)
{
    Data[(R << 8) + C] = V;
    ChangedR.push(R);
    ChangedC.push(C);
}

string IOSpreadsheet::CellGet(int R, int C)
{
    return Data[(R << 8) + C];
}

bool IOSpreadsheet::CellChanged(int &R, int &C)
{
    if ((ChangedR.size() > 0) && (ChangedC.size() > 0))
    {
        R = ChangedR.front();
        C = ChangedC.front();
        ChangedR.pop();
        ChangedC.pop();
        return true;
    }
    else
    {
        return false;
    }
}

void IOSpreadsheet::SheetClear(int R1, int C1, int R2, int C2)
{
    for (int R = R1; R <= R2; R++)
    {
        for (int C = C1; C <= C2; C++)
        {
            CellSet(R, C, "");
        }
    }
}

void IOSpreadsheet::SheetRowAdd(int R1, int C1, int R2, int C2)
{
    int X = R1;
    int X0 = R2;
    int X1 = C1;
    int X2 = C2;
    if ((X < 0) || (X1 < 0) || (X2 < X1))
    {
        return;
    }
    string V;
    for (int C = X1; C <= X2; C++)
    {
        for (int R = (X0 - 1); R >= X; R--)
        {
            V = CellGet(R, C);
            CellSet(R + 1, C, V);
        }
        CellSet(X, C, "");
    }
}

void IOSpreadsheet::SheetRowRem(int R1, int C1, int R2, int C2)
{
    int X = R1;
    int X0 = R2;
    int X1 = C1;
    int X2 = C2;
    if ((X < 0) || (X1 < 0) || (X2 < X1))
    {
        return;
    }
    string V;
    for (int C = X1; C <= X2; C++)
    {
        for (int R = X; R <= (X0 - 1); R++)
        {
            V = CellGet(R + 1, C);
            CellSet(R, C, V);
        }
        CellSet(X0, C, "");
    }
}

void IOSpreadsheet::SheetColAdd(int R1, int C1, int R2, int C2)
{
    int X = C1;
    int X0 = C2;
    int X1 = R1;
    int X2 = R2;
    if ((X < 0) || (X1 < 0) || (X2 < X1))
    {
        return;
    }
    string V;
    for (int R = X1; R <= X2; R++)
    {
        for (int C = (X0 - 1); C >= X; C--)
        {
            V = CellGet(R, C);
            CellSet(R, C + 1, V);
        }
        CellSet(R, X, "");
    }
}

void IOSpreadsheet::SheetColRem(int R1, int C1, int R2, int C2)
{
    int X = C1;
    int X0 = C2;
    int X1 = R1;
    int X2 = R2;
    if ((X < 0) || (X1 < 0) || (X2 < X1))
    {
        return;
    }
    string V;
    for (int R = X1; R <= X2; R++)
    {
        for (int C = X; C <= (X0 - 1); C++)
        {
            V = CellGet(R, C + 1);
            CellSet(R, C, V);
        }
        CellSet(R, X0, "");
    }
}
