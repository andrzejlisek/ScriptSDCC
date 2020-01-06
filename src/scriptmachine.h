#ifndef SCRIPTMACHINE_H
#define SCRIPTMACHINE_H

#include <string>
#include <iostream>
#include "eden.h"
#include "ioconsole.h"
#include "iospreadsheet.h"
#include "iograph.h"
#include "binary.h"
#include "membuffer.h"
#include "filehandle.h"

#define IOConsoleCmd 4
#define IOSpreadsheetCmd 32
#define IOGraphCmd 60
#define IOGraphTextCmd 80


class ScriptMachine : public QObject
{
    Q_OBJECT
public:
    ScriptMachine();
    virtual ~ScriptMachine();

    MemBuffer * MemBuffer_;

    uchar BundleIndex;

    uchar * MemData;
    uchar * MemProg;
    uchar MemMapR[65536];
    uchar MemMapW[65536];
    uchar MemMapP[65536];
    uchar MemMapC[65536];

    FileHandle * FileHandle_;

    void MemSet(ushort Addr, uchar Val);
    void MemSet(ushort AddrH, ushort AddrL, uchar Val);
    uchar MemPrg(ushort Addr);
    uchar MemGet(ushort Addr);
    uchar MemGet(ushort AddrH, ushort AddrL);
    int MemMode;
    int CodeLoc;
    int CodeSize;
    int DataLoc;
    int DataSize;
    void PrepareMem(int Mode);

    void LoadProgram(int Mode_, string &ProgCode, int CodeLoc_, int CodeSize_, int DataLoc_, int DataSize_);


    stringstream TextBuff;
    string TextBuffS;
    uint TextBuffI;

    union IOBufferUnion
    {
        uchar Raw[8];
        char BufS1;
        uchar BufU1;
        short BufS2;
        ushort BufU2;
        int BufS4;
        uint BufU4;
        float BufF;
    } IOBuffer;

    void InputValue(string Val);


    virtual void DoCommand() = 0;
    virtual void DoReset() = 0;

    bool ProgWorking();
    void ProgWork();
    void ProgReset();
    void ProgAbort();

    bool AbortRequest;

    int SwapPage = 0;

    llong CommandCounter = 0;
    llong CommandCounterX = 0;


    IOConsole * IOConsole_[4];
    IOSpreadsheet * IOSpreadsheet_[4];
    IOGraph * IOGraph_[4];

    string GetStatus();
    uchar StatusC = 0;

    volatile uchar CoreInvokeWait;
private:
    char ValGetSC(int Addr);
    uchar ValGetUC(int Addr);
    short ValGetSS(int Addr);
    ushort ValGetUS(int Addr);
    int ValGetSI(int Addr);
    uint ValGetUI(int Addr);
    long ValGetSL(int Addr);
    ulong ValGetUL(int Addr);
    double ValGetF(int Addr);
    void ValPut(int Addr, double Val);

protected:
    string ErrorStatus = "";
signals:
    void CoreInvoke(uchar Idx, uchar Param1, uchar Param2);
};

#endif // SCRIPTMACHINE_H
