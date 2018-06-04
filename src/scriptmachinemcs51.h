#ifndef SCRIPTMACHINEMCS51_H
#define SCRIPTMACHINEMCS51_H

#include "scriptmachine.h"

#define BoolToInt(X) ((X) ? 1 : 0)
#define IRAMRead(X) IRAM[X]
#define IRAMWrite(X, Y) IRAM[X] = (uchar)(Y)

#define PSW7Clr BitWrite(0xD7, false)
#define PSW6Clr BitWrite(0xD6, false)
#define PSW5Clr BitWrite(0xD5, false)
#define PSW4Clr BitWrite(0xD4, false)
#define PSW3Clr BitWrite(0xD3, false)
#define PSW2Clr BitWrite(0xD2, false)
#define PSW1Clr BitWrite(0xD1, false)
#define PSW0Clr BitWrite(0xD0, false)

#define PSW7Set BitWrite(0xD7, true)
#define PSW6Set BitWrite(0xD6, true)
#define PSW5Set BitWrite(0xD5, true)
#define PSW4Set BitWrite(0xD4, true)
#define PSW3Set BitWrite(0xD3, true)
#define PSW2Set BitWrite(0xD2, true)
#define PSW1Set BitWrite(0xD1, true)
#define PSW0Set BitWrite(0xD0, true)

#define PSW7Get BitRead(0xD7)
#define PSW6Get BitRead(0xD6)
#define PSW5Get BitRead(0xD5)
#define PSW4Get BitRead(0xD4)
#define PSW3Get BitRead(0xD3)
#define PSW2Get BitRead(0xD2)
#define PSW1Get BitRead(0xD1)
#define PSW0Get BitRead(0xD0)

#define BitAddress1(Addr) (((Addr) >> 3) + 32)
#define BitAddress2(Addr) (((Addr) >> 3) - 16)

#define AccumulatorRead RegBitsX[0x0C]
#define AccumulatorWrite(Data) RegBitsX[0x0C] = (Data); CheckParity()
#define BitRegRead(Addr) RegBitsX[BitAddress2((int)(Addr))]
#define BitRegWrite(Addr, Data) RegBitsX[BitAddress2((int)(Addr))] = (Data); if (BitAddress2((int)(Addr)) == 0x0D) { CheckParity(); }

class ScriptMachineMCS51 : public ScriptMachine
{
public:
    ScriptMachineMCS51();
    ~ScriptMachineMCS51();

    void DoCommand();
    void DoReset();
private:
    uchar IRAM[256];

    uchar RegSP;
    uchar RegDPH;
    uchar RegDPL;
    ushort PC;

    inline uchar PrgRead(int X)
    {
        MemMapC[X] = 1;
        return MemProg[X];
    }
    inline uchar XBYTERead(int X)
    {
        MemMapR[X] = 1;
        return MemData[X];
    }
    inline void XBYTEWrite(int X, uchar Y)
    {
        MemMapW[X] = 1;
        MemData[X] = Y;
    }

    void DoAJMP(int Addr, int Offset);
    void DoACALL(int Addr, int Offset);
    void DoCJNE(int Data1, int Data2, int Addr);
    void DoDJNZ(int R, int Addr);
    void DoADD(int Arg);
    void DoADDC(int Arg);
    void DoSUBB(int Arg);
    void DecSP();
    void IncSP();
    int CalcRel(int Data);
    int RRead(int N);
    void RWrite(int N, int Data);
    int IBYTERead(int Addr);
    void IBYTEWrite(int Addr, int Data);
    void IntToBin(int N0, bool &B7, bool &B6, bool &B5, bool &B4, bool &B3, bool &B2, bool &B1, bool &B0);
    int BinToInt(bool B7, bool B6, bool B5, bool B4, bool B3, bool B2, bool B1, bool B0);
    bool BitRead(int Addr);
    void BitWrite(int Addr, bool Val);
    void CheckParity();

    int RegBitsX[16];
};

#endif // SCRIPTMACHINEMCS51_H
