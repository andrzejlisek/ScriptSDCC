#ifndef SCRIPTMACHINEZ180_H
#define SCRIPTMACHINEZ180_H

#include "scriptmachine.h"

#define FlagCSet Reg_F = Reg_F | b00000001
#define FlagNSet Reg_F = Reg_F | b00000010
#define FlagPSet Reg_F = Reg_F | b00000100
#define FlagHSet Reg_F = Reg_F | b00010000
#define FlagZSet Reg_F = Reg_F | b01000000
#define FlagSSet Reg_F = Reg_F | b10000000

#define FlagCClr Reg_F = Reg_F & b11111110
#define FlagNClr Reg_F = Reg_F & b11111101
#define FlagPClr Reg_F = Reg_F & b11111011
#define FlagHClr Reg_F = Reg_F & b11101111
#define FlagZClr Reg_F = Reg_F & b10111111
#define FlagSClr Reg_F = Reg_F & b01111111

#define FlagCGet ((Reg_F & b00000001) > 0)
#define FlagNGet ((Reg_F & b00000010) > 0)
#define FlagPGet ((Reg_F & b00000100) > 0)
#define FlagHGet ((Reg_F & b00010000) > 0)
#define FlagZGet ((Reg_F & b01000000) > 0)
#define FlagSGet ((Reg_F & b10000000) > 0)

#define SetFlagsSZ(Val) if (Val & b10000000) { FlagSSet; } else { FlagSClr; } if (Val) { FlagZClr; } else { FlagZSet; }
#define SetFlagsSZ_(ValH, ValL) if (ValH & b10000000) { FlagSSet; } else { FlagSClr; } if (ValH || ValL) { FlagZClr; } else { FlagZSet; }
#define SetFlagsP(Val) { bool P = 1; if (Val & b10000000) { P = !P; } if (Val & b01000000) { P = !P; } if (Val & b00100000) { P = !P; } if (Val & b00010000) { P = !P; } if (Val & b00001000) { P = !P; } if (Val & b00000100) { P = !P; } if (Val & b00000010) { P = !P; } if (Val & b00000001) { P = !P; } if (P) { FlagPSet; } else { FlagPClr; } }

#define RegIdxD ((Arg1 < 128) ? (Reg_Idx + ((ushort)Arg1)) : (Reg_Idx + ((ushort)Arg1) - 256))

class ScriptMachineZ180 : public ScriptMachine
{
public:
    ScriptMachineZ180();
    ~ScriptMachineZ180();

    void DoCommand();
    void DoReset();
private:
    uchar Reg_A, Reg_A_;
    uchar Reg_F, Reg_F_;

    uchar Reg_B, Reg_B_;
    uchar Reg_C, Reg_C_;
    uchar Reg_D, Reg_D_;
    uchar Reg_E, Reg_E_;
    uchar Reg_H, Reg_H_;
    uchar Reg_L, Reg_L_;

    ushort Reg_IX;
    ushort Reg_IY;
    ushort Reg_SP;
    ushort Reg_Idx;

    uchar Reg_I;
    uchar Reg_R;

    ushort Reg_PC;

    bool Reg_IFF1;
    bool Reg_IFF2;

    void DoCALL(bool Condition);
    void DoPUSH(uchar &ValH, uchar &ValL);
    void DoPOP(uchar &ValH, uchar &ValL);
    void DoRET(bool Condition);
    void DoRETI();
    void DoRST(int Addr);
    void DoJR(bool Condition);
    void DoADD1(uchar Val, bool Carry);
    void DoADD2_HL(int Val, bool Carry);
    void DoADD2_HL(int ValH, int ValL, bool Carry);
    void DoADD2_I_(int Val);
    void DoADD2_I_(int ValH, int ValL);
    void DoSUB1(uchar Val, bool Carry);
    void DoSUB2(int Val, bool Carry);
    void DoSUB2(int ValH, int ValL, bool Carry);
    void DoCP(uchar Val);
    void DoTST(uchar Val);
    void DoAND(uchar Val);
    void DoXOR(uchar Val);
    void DoOR(uchar Val);
    void DoINC(uchar &Reg);
    void DoDEC(uchar &Reg);
    void DoRL(uchar &Reg, bool NoAcc);
    void DoRR(uchar &Reg, bool NoAcc);
    void DoRLC(uchar &Reg, bool NoAcc);
    void DoRRC(uchar &Reg, bool NoAcc);
    void DoSLA(uchar &Reg);
    void DoSRA(uchar &Reg);
    void DoSRL(uchar &Reg);
    void DoJumpAbs(int Addr, bool Call);
    void DoJumpAbs(int AddrH, int AddrL, bool Call);
    void DoJumpRel(int Addr, bool Call);
    void DoBlock1I();
    void DoBlock1D();
    void DoBlock1IR();
    void DoBlock1DR();
    void DoBlock2I();
    void DoBlock2D();
    void DoBlock2IR(bool CompareA);
    void DoBlock2DR(bool CompareA);
    void DoBlock_CP();
    void DoBlock_LD();
};

#endif // SCRIPTMACHINEZ180_H
