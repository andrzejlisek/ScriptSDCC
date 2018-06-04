#include "ioconsole.h"

IOConsole::IOConsole()
{
    Clear();
}

IOConsole::~IOConsole()
{

}

void IOConsole::Clear()
{
    SS.str(std::string());
    Changed = true;
}

void IOConsole::SetFloadFixed(int N)
{
    SS << fixed << setprecision(N);
}

void IOConsole::SetFloatScientific(int N)
{
    SS << scientific << setprecision(N);
}

void IOConsole::Print(uchar X)  { SS << X; Changed = true; }
void IOConsole::Print(char X)   { SS << X; Changed = true; }
void IOConsole::Print(ushort X) { SS << X; Changed = true; }
void IOConsole::Print(short X)  { SS << X; Changed = true; }
void IOConsole::Print(uint X)   { SS << X; Changed = true; }
void IOConsole::Print(int X)    { SS << X; Changed = true; }
void IOConsole::Print(ulong X)  { SS << X; Changed = true; }
void IOConsole::Print(long X)   { SS << X; Changed = true; }
void IOConsole::Print(float X)  { SS << X; Changed = true; }

bool IOConsole::IsChanged()
{
    return Changed;
}

string IOConsole::GetText()
{
    return SS.str();
}

void IOConsole::ResetChanged()
{
    Changed = false;
}

void IOConsole::InputCharPut(ConChar X)
{
    if (X != '\0')
    {
        InputCharQueue.push(X);
    }
}

ConChar IOConsole::InputCharGet()
{
    if (InputCharQueue.size() > 0)
    {
        char X = InputCharQueue.front();
        InputCharQueue.pop();
        return X;
    }
    else
    {
        return 0;
    }
}

void IOConsole::InputClear()
{
    while (InputCharQueue.size() > 0)
    {
        InputCharQueue.pop();
    }
}
