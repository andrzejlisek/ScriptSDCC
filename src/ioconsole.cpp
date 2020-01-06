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
    MTX.lock();
    SS.str(std::string());
    Changed = true;
    MTX.unlock();
}

void IOConsole::SetFloadFixed(int N)
{
    MTX.lock();
    SS << fixed << setprecision(N);
    MTX.unlock();
}

void IOConsole::SetFloatScientific(int N)
{
    MTX.lock();
    SS << scientific << setprecision(N);
    MTX.unlock();
}

void IOConsole::Print(uchar X)  { MTX.lock(); SS << X; Changed = true; MTX.unlock(); }
void IOConsole::Print(char X)   { MTX.lock(); SS << X; Changed = true; MTX.unlock(); }
void IOConsole::Print(ushort X) { MTX.lock(); SS << X; Changed = true; MTX.unlock(); }
void IOConsole::Print(short X)  { MTX.lock(); SS << X; Changed = true; MTX.unlock(); }
void IOConsole::Print(uint X)   { MTX.lock(); SS << X; Changed = true; MTX.unlock(); }
void IOConsole::Print(int X)    { MTX.lock(); SS << X; Changed = true; MTX.unlock(); }
void IOConsole::Print(ulong X)  { MTX.lock(); SS << X; Changed = true; MTX.unlock(); }
void IOConsole::Print(long X)   { MTX.lock(); SS << X; Changed = true; MTX.unlock(); }
void IOConsole::Print(float X)  { MTX.lock(); SS << X; Changed = true; MTX.unlock(); }

bool IOConsole::IsChanged()
{
    MTX.lock();
    bool C = Changed;
    MTX.unlock();
    return C;
}

string IOConsole::GetText()
{
    MTX.lock();
    string SS_ = SS.str();
    MTX.unlock();
    return SS_;
}

void IOConsole::ResetChanged()
{
    MTX.lock();
    Changed = false;
    MTX.unlock();
}

void IOConsole::InputCharPut(ConChar X)
{
    if (X != '\0')
    {
        MTX.lock();
        InputCharQueue.push(X);
        MTX.unlock();
    }
}

ConChar IOConsole::InputCharGet()
{
    MTX.lock();
    if (InputCharQueue.size() > 0)
    {
        char X = InputCharQueue.front();
        InputCharQueue.pop();
        MTX.unlock();
        return X;
    }
    else
    {
        MTX.unlock();
        return 0;
    }
}

void IOConsole::InputClear()
{
    MTX.lock();
    while (InputCharQueue.size() > 0)
    {
        InputCharQueue.pop();
    }
    MTX.unlock();
}
