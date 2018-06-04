#ifndef IOCONSOLE_H
#define IOCONSOLE_H

#include <iostream>
#include <sstream>
#include "eden.h"
#include <iomanip>
#include <queue>

using namespace std;

#define ConChar uchar

class IOConsole
{
public:
    IOConsole();
    ~IOConsole();
    void Clear();
    void SetFloadFixed(int N);
    void SetFloatScientific(int N);
    void Print(uchar X);
    void Print(char X);
    void Print(ushort X);
    void Print(short X);
    void Print(uint X);
    void Print(int X);
    void Print(ulong X);
    void Print(long X);
    void Print(float X);
    bool IsChanged();
    string GetText();
    void ResetChanged();
    void InputCharPut(ConChar X);
    ConChar InputCharGet();
    void InputClear();
private:
    bool Changed;
    stringstream SS;
    queue<ConChar> InputCharQueue;
};

#endif // IOCONSOLE_H
