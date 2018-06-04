#include "scriptmachinez180.h"

ScriptMachineZ180::ScriptMachineZ180()
{

}

ScriptMachineZ180::~ScriptMachineZ180()
{

}

void ScriptMachineZ180::DoCommand()
{
    uchar OpCode0;
    uchar OpCode1;
    uchar OpCode2;
    uchar Data1;
    uchar Data2;
    uchar Data3;
    uchar Data4;
    uchar Arg1;
    uchar Arg2;
    int Data1I;
    int Data2I;

    OpCode0 = MemPrg(Reg_PC);


    //cout << Eden::IntToHex16(Reg_PC) << ": " << Eden::IntToHex8(MemPrg(Reg_PC + 0)) << " " << Eden::IntToHex8(MemPrg(Reg_PC + 1)) << " " << Eden::IntToHex8(MemPrg(Reg_PC + 2)) << " " << Eden::IntToHex8(MemPrg(Reg_PC + 3));
    //cout << " " << Eden::IntToHex8(MemPrg(Reg_PC + 4)) << " " << Eden::IntToHex8(MemPrg(Reg_PC + 5)) << " " << Eden::IntToHex8(MemPrg(Reg_PC + 6)) << " " << Eden::IntToHex8(MemPrg(Reg_PC + 7)) << endl;


    Reg_PC++;
    switch (OpCode0)
    {

    // Additional instructions
    case 0x00: // NOP
        break;
    case 0x76: // HALT
        break;
    case 0x27: // DA A
        if ((Reg_A > 0x99) || FlagCGet)
        {
            Data1 = 0x60;
            FlagCSet;
        }
        else
        {
            Data1 = 0x00;
            FlagCClr;
        }
        if (((Reg_A & b00001111) > 0x09) || FlagHGet)
        {
            Data1 += 0x06;
        }
        if (FlagNGet)
        {
            Data2 = Reg_A - Data1;
        }
        else
        {
            Data2 = Reg_A + Data1;
        }
        if ((Reg_A & b00010000) == (Data2 & b00010000))
        {
            FlagHClr;
        }
        else
        {
            FlagHSet;
        }
        Reg_A = Data2;
        SetFlagsSZ(Reg_A);
        SetFlagsP(Reg_A);
        break;

    // Swapping primary and secondary registers
    case 0x08: // EX AF, AF'
        Data1 = Reg_A; Reg_A = Reg_A_; Reg_A_ = Data1;
        Data1 = Reg_F; Reg_F = Reg_F_; Reg_F_ = Data1;
        break;
    case 0xD9: // EXX
        Data1 = Reg_B; Reg_B = Reg_B_; Reg_B_ = Data1;
        Data1 = Reg_C; Reg_C = Reg_C_; Reg_C_ = Data1;
        Data1 = Reg_D; Reg_D = Reg_D_; Reg_D_ = Data1;
        Data1 = Reg_E; Reg_E = Reg_E_; Reg_E_ = Data1;
        Data1 = Reg_H; Reg_H = Reg_H_; Reg_H_ = Data1;
        Data1 = Reg_L; Reg_L = Reg_L_; Reg_L_ = Data1;
        break;
    case 0xE3: // EX (SP), HL
        Reg_SP++;
        Data1 = MemGet(Reg_SP); MemSet(Reg_SP, Reg_H); Reg_H = Data1;
        Reg_SP--;
        Data1 = MemGet(Reg_SP); MemSet(Reg_SP, Reg_L); Reg_L = Data1;
        break;
    case 0xEB: // EX DE, HL
        Data1 = Reg_D; Reg_D = Reg_H; Reg_H = Data1;
        Data1 = Reg_E; Reg_E = Reg_L; Reg_L = Data1;
        break;

    // Writing constant value
    case 0x01: // LD BC, NN
        Reg_C = MemPrg(Reg_PC);
        Reg_PC++;
        Reg_B = MemPrg(Reg_PC);
        Reg_PC++;
        break;
    case 0x11: // LD DE, NN
        Reg_E = MemPrg(Reg_PC);
        Reg_PC++;
        Reg_D = MemPrg(Reg_PC);
        Reg_PC++;
        break;
    case 0x21: // LD HL, NN
        Reg_L = MemPrg(Reg_PC);
        Reg_PC++;
        Reg_H = MemPrg(Reg_PC);
        Reg_PC++;
        break;
    case 0x31: // LD SP, NN
        Data1I = MemPrg(Reg_PC);
        Reg_PC++;
        Data2I = MemPrg(Reg_PC);
        Reg_PC++;
        Reg_SP = (Data2I << 8) + Data1I;
        break;
    case 0x22: // LD (NN), HL
        MemSet(MemPrg(Reg_PC + 1), MemPrg(Reg_PC), Reg_L);
        MemSet(MemPrg(Reg_PC + 1), MemPrg(Reg_PC) + 1, Reg_H);
        Reg_PC++;
        Reg_PC++;
        break;
    case 0x02: // LD (BC), A
        MemSet(Reg_B, Reg_C, Reg_A);
        break;
    case 0x12: // LD (DE), A
        MemSet(Reg_D, Reg_E, Reg_A);
        break;
    case 0x32: // LD (NN), A
        MemSet(MemPrg(Reg_PC + 1), MemPrg(Reg_PC), Reg_A);
        Reg_PC++;
        Reg_PC++;
        break;
    case 0x3A: // LD A, (NN)
        Reg_A = MemGet(MemPrg(Reg_PC + 1), MemPrg(Reg_PC));
        Reg_PC++;
        Reg_PC++;
        break;
    case 0x36: // LD (HL), N
        MemSet(Reg_H, Reg_L, MemPrg(Reg_PC));
        Reg_PC++;
        break;
    case 0x0A: // LD A, (BC)
        Reg_A = MemGet(Reg_B, Reg_C);
        break;
    case 0x1A: // LD A, (DE)
        Reg_A = MemGet(Reg_D, Reg_E);
        break;
    case 0x06: // LD B, N
        Reg_B = MemPrg(Reg_PC);
        Reg_PC++;
        break;
    case 0x0E: // LD C, N
        Reg_C = MemPrg(Reg_PC);
        Reg_PC++;
        break;
    case 0x16: // LD D, N
        Reg_D = MemPrg(Reg_PC);
        Reg_PC++;
        break;
    case 0x1E: // LD E, N
        Reg_E = MemPrg(Reg_PC);
        Reg_PC++;
        break;
    case 0x26: // LD H, N
        Reg_H = MemPrg(Reg_PC);
        Reg_PC++;
        break;
    case 0x2A: // LD HL, (NN)
        Reg_L = MemGet(MemPrg(Reg_PC + 1), MemPrg(Reg_PC));
        Reg_H = MemGet(MemPrg(Reg_PC + 1), MemPrg(Reg_PC) + 1);
        Reg_PC++;
        Reg_PC++;
        break;
    case 0x2E: // LD L, N
        Reg_L = MemPrg(Reg_PC);
        Reg_PC++;
        break;
    case 0x3E: // LD A, N
        Reg_A = MemPrg(Reg_PC);
        Reg_PC++;
        break;
    case 0xF9: // LD SP, HL
        Reg_SP = ((ushort)Reg_H << 8) + (ushort)Reg_L;
        break;

    // Bit shifting and rotating
    case 0x07: // RLC A
        DoRLC(Reg_A, false);
        break;
    case 0x0F: // RRC A
        DoRRC(Reg_A, false);
        break;
    case 0x17: // RL A
        DoRL(Reg_A, false);
        break;
    case 0x1F: // RR A
        DoRR(Reg_A, false);
        break;

    // Value incrementation and decrementation
    case 0x03: // INC BC
        Reg_C++;
        if (Reg_C == 0)
        {
            Reg_B++;
        }
        break;
    case 0x13: // INC DE
        Reg_E++;
        if (Reg_E == 0)
        {
            Reg_D++;
        }
        break;
    case 0x23: // INC HL
        Reg_L++;
        if (Reg_L == 0)
        {
            Reg_H++;
        }
        break;
    case 0x33: // INC SP
        Reg_SP++;
        break;
    case 0x0B: // DEC BC
        Reg_C--;
        if (Reg_C == 255)
        {
            Reg_B--;
        }
        break;
    case 0x1B: // DEC DE
        Reg_E--;
        if (Reg_E == 255)
        {
            Reg_D--;
        }
        break;
    case 0x2B: // DEC HL
        Reg_L--;
        if (Reg_L == 255)
        {
            Reg_H--;
        }
        break;
    case 0x3B: // DEC SP
        Reg_SP--;
        break;
    case 0x04: // INC B
        DoINC(Reg_B);
        break;
    case 0x05: // DEC B
        DoDEC(Reg_B);
        break;
    case 0x0C: // INC C
        DoINC(Reg_C);
        break;
    case 0x0D: // DEC C
        DoDEC(Reg_C);
        break;
    case 0x14: // INC D
        DoINC(Reg_D);
        break;
    case 0x15: // DEC D
        DoDEC(Reg_D);
        break;
    case 0x1C: // INC E
        DoINC(Reg_E);
        break;
    case 0x1D: // DEC E
        DoDEC(Reg_E);
        break;
    case 0x24: // INC H
        DoINC(Reg_H);
        break;
    case 0x25: // DEC H
        DoDEC(Reg_H);
        break;
    case 0x2C: // INC L
        DoINC(Reg_L);
        break;
    case 0x2D: // DEC L
        DoDEC(Reg_L);
        break;
    case 0x34: // INC (HL)
        Data1 = MemGet(Reg_H, Reg_L);
        DoINC(Data1);
        MemSet(Reg_H, Reg_L, Data1);
        break;
    case 0x35: // DEC (HL)
        Data1 = MemGet(Reg_H, Reg_L);
        DoDEC(Data1);
        MemSet(Reg_H, Reg_L, Data1);
        break;
    case 0x3C: // INC A
        DoINC(Reg_A);
        break;
    case 0x3D: // DEC A
        DoDEC(Reg_A);
        break;

    // Adding and subtracting
    case 0x09: // ADD HL, BC
        DoADD2_HL(Reg_B, Reg_C, false);
        break;
    case 0x19: // ADD HL, DE
        DoADD2_HL(Reg_D, Reg_E, false);
        break;
    case 0x29: // ADD HL, HL
        DoADD2_HL(Reg_H, Reg_L, false);
        break;
    case 0x39: // ADD HL, SP
        DoADD2_HL(Reg_SP, false);
        break;
    case 0xC6: // ADD A, N
        DoADD1(MemPrg(Reg_PC), false);
        Reg_PC++;
        break;
    case 0xCE: // ADC A, N
        DoADD1(MemPrg(Reg_PC), true);
        Reg_PC++;
        break;
    case 0xD6: // SUB N
        DoSUB1(MemPrg(Reg_PC), false);
        Reg_PC++;
        break;
    case 0xDE: // SBC A, N
        DoSUB1(MemPrg(Reg_PC), true);
        Reg_PC++;
        break;

    // Logical operations
    case 0x2F: // CPL
        Reg_A = ~Reg_A;
        FlagHSet;
        FlagNSet;
        break;
    case 0x37: // SCF
        FlagCSet;
        FlagHClr;
        FlagNClr;
        break;
    case 0x3F: // CCF
        if (FlagCGet)
        {
            FlagHSet;
            FlagCClr;
        }
        else
        {
            FlagHClr;
            FlagCSet;
        }
        FlagNClr;
        break;
    case 0xE6: // AND N
        DoAND(MemPrg(Reg_PC));
        Reg_PC++;
        break;
    case 0xEE: // XOR N
        DoXOR(MemPrg(Reg_PC));
        Reg_PC++;
        break;
    case 0xF6: // OR N
        DoOR(MemPrg(Reg_PC));
        Reg_PC++;
        break;
    case 0xFE: // CP N
        DoCP(MemPrg(Reg_PC));
        Reg_PC++;
        break;

    // Jumps
    case 0x10: // DJNZ N
        Arg1 = MemPrg(Reg_PC);
        Reg_PC++;
        Reg_B--;
        if (Reg_B != 0)
        {
            DoJumpRel(Arg1, false);
        }
        break;
    case 0x18: // JR N
        DoJR(true);
        break;
    case 0x20: // JR NZ, N
        DoJR(!FlagZGet);
        break;
    case 0x28: // JR Z, N
        DoJR(FlagZGet);
        break;
    case 0x30: // JR NC, N
        DoJR(!FlagCGet);
        break;
    case 0x38: // JR C, N
        DoJR(FlagCGet);
        break;
    case 0xC3: // JP NN
        Arg1 = MemPrg(Reg_PC);
        Reg_PC++;
        Arg2 = MemPrg(Reg_PC);
        Reg_PC++;
        DoJumpAbs(Arg2, Arg1, false);
        break;
    case 0xE9: // JP (HL)
        DoJumpAbs(Reg_H, Reg_L, false);
        break;
    case 0xC2: // JP NZ, NN
        Arg1 = MemPrg(Reg_PC);
        Reg_PC++;
        Arg2 = MemPrg(Reg_PC);
        Reg_PC++;
        if (!FlagZGet)
        {
            DoJumpAbs(Arg2, Arg1, false);
        }
        break;
    case 0xCA: // JP Z, NN
        Arg1 = MemPrg(Reg_PC);
        Reg_PC++;
        Arg2 = MemPrg(Reg_PC);
        Reg_PC++;
        if (FlagZGet)
        {
            DoJumpAbs(Arg2, Arg1, false);
        }
        break;
    case 0xD2: // JP NC, NN
        Arg1 = MemPrg(Reg_PC);
        Reg_PC++;
        Arg2 = MemPrg(Reg_PC);
        Reg_PC++;
        if (!FlagCGet)
        {
            DoJumpAbs(Arg2, Arg1, false);
        }
        break;
    case 0xDA: // JP C, NN
        Arg1 = MemPrg(Reg_PC);
        Reg_PC++;
        Arg2 = MemPrg(Reg_PC);
        Reg_PC++;
        if (FlagCGet)
        {
            DoJumpAbs(Arg2, Arg1, false);
        }
        break;
    case 0xE2: // JP PO, NN
        Arg1 = MemPrg(Reg_PC);
        Reg_PC++;
        Arg2 = MemPrg(Reg_PC);
        Reg_PC++;
        if (!FlagPGet)
        {
            DoJumpAbs(Arg2, Arg1, false);
        }
        break;
    case 0xEA: // JP PE, NN
        Arg1 = MemPrg(Reg_PC);
        Reg_PC++;
        Arg2 = MemPrg(Reg_PC);
        Reg_PC++;
        if (FlagPGet)
        {
            DoJumpAbs(Arg2, Arg1, false);
        }
        break;
    case 0xF2: // JP P, NN
        Arg1 = MemPrg(Reg_PC);
        Reg_PC++;
        Arg2 = MemPrg(Reg_PC);
        Reg_PC++;
        if (!FlagSGet)
        {
            DoJumpAbs(Arg2, Arg1, false);
        }
        break;
    case 0xFA: // JP M, NN
        Arg1 = MemPrg(Reg_PC);
        Reg_PC++;
        Arg2 = MemPrg(Reg_PC);
        Reg_PC++;
        if (FlagSGet)
        {
            DoJumpAbs(Arg2, Arg1, false);
        }
        break;

    // Instructions on standard registers - copying value
    case 0x40: // LD B, B
        Reg_B = Reg_B;
        break;
    case 0x41: // LD B, C
        Reg_B = Reg_C;
        break;
    case 0x42: // LD B, D
        Reg_B = Reg_D;
        break;
    case 0x43: // LD B, E
        Reg_B = Reg_E;
        break;
    case 0x44: // LD B, H
        Reg_B = Reg_H;
        break;
    case 0x45: // LD B, L
        Reg_B = Reg_L;
        break;
    case 0x46: // LD B, (HL)
        Reg_B = MemGet(Reg_H, Reg_L);
        break;
    case 0x47: // LD B, A
        Reg_B = Reg_A;
        break;
    case 0x48: // LD C, B
        Reg_C = Reg_B;
        break;
    case 0x49: // LD C, C
        Reg_C = Reg_C;
        break;
    case 0x4A: // LD C, D
        Reg_C = Reg_D;
        break;
    case 0x4B: // LD C, E
        Reg_C = Reg_E;
        break;
    case 0x4C: // LD C, H
        Reg_C = Reg_H;
        break;
    case 0x4D: // LD C, L
        Reg_C = Reg_L;
        break;
    case 0x4E: // LD C, (HL)
        Reg_C = MemGet(Reg_H, Reg_L);
        break;
    case 0x4F: // LD C, A
        Reg_C = Reg_A;
        break;
    case 0x50: // LD D, B
        Reg_D = Reg_B;
        break;
    case 0x51: // LD D, C
        Reg_D = Reg_C;
        break;
    case 0x52: // LD D, D
        Reg_D = Reg_D;
        break;
    case 0x53: // LD D, E
        Reg_D = Reg_E;
        break;
    case 0x54: // LD D, H
        Reg_D = Reg_H;
        break;
    case 0x55: // LD D, L
        Reg_D = Reg_L;
        break;
    case 0x56: // LD D, (HL)
        Reg_D = MemGet(Reg_H, Reg_L);
        break;
    case 0x57: // LD D, A
        Reg_D = Reg_A;
        break;
    case 0x58: // LD E, B
        Reg_E = Reg_B;
        break;
    case 0x59: // LD E, C
        Reg_E = Reg_C;
        break;
    case 0x5A: // LD E, D
        Reg_E = Reg_D;
        break;
    case 0x5B: // LD E, E
        Reg_E = Reg_E;
        break;
    case 0x5C: // LD E, H
        Reg_E = Reg_H;
        break;
    case 0x5D: // LD E, L
        Reg_E = Reg_L;
        break;
    case 0x5E: // LD E, (HL)
        Reg_E = MemGet(Reg_H, Reg_L);
        break;
    case 0x5F: // LD E, A
        Reg_E = Reg_A;
        break;
    case 0x60: // LD H, B
        Reg_H = Reg_B;
        break;
    case 0x61: // LD H, C
        Reg_H = Reg_C;
        break;
    case 0x62: // LD H, D
        Reg_H = Reg_D;
        break;
    case 0x63: // LD H, E
        Reg_H = Reg_E;
        break;
    case 0x64: // LD H, H
        Reg_H = Reg_H;
        break;
    case 0x65: // LD H, L
        Reg_H = Reg_L;
        break;
    case 0x66: // LD H, (HL)
        Reg_H = MemGet(Reg_H, Reg_L);
        break;
    case 0x67: // LD H, A
        Reg_H = Reg_A;
        break;
    case 0x68: // LD L, B
        Reg_L = Reg_B;
        break;
    case 0x69: // LD L, C
        Reg_L = Reg_C;
        break;
    case 0x6A: // LD L, D
        Reg_L = Reg_D;
        break;
    case 0x6B: // LD L, E
        Reg_L = Reg_E;
        break;
    case 0x6C: // LD L, H
        Reg_L = Reg_H;
        break;
    case 0x6D: // LD L, L
        Reg_L = Reg_L;
        break;
    case 0x6E: // LD L, (HL)
        Reg_L = MemGet(Reg_H, Reg_L);
        break;
    case 0x6F: // LD L, A
        Reg_L = Reg_A;
        break;
    case 0x70: // LD (HL), B
        MemSet(Reg_H, Reg_L, Reg_B);
        break;
    case 0x71: // LD (HL), C
        MemSet(Reg_H, Reg_L, Reg_C);
        break;
    case 0x72: // LD (HL), D
        MemSet(Reg_H, Reg_L, Reg_D);
        break;
    case 0x73: // LD (HL), E
        MemSet(Reg_H, Reg_L, Reg_E);
        break;
    case 0x74: // LD (HL), H
        MemSet(Reg_H, Reg_L, Reg_H);
        break;
    case 0x75: // LD (HL), L
        MemSet(Reg_H, Reg_L, Reg_L);
        break;
    case 0x77: // LD (HL), A
        MemSet(Reg_H, Reg_L, Reg_A);
        break;
    case 0x78: // LD A, B
        Reg_A = Reg_B;
        break;
    case 0x79: // LD A, C
        Reg_A = Reg_C;
        break;
    case 0x7A: // LD A, D
        Reg_A = Reg_D;
        break;
    case 0x7B: // LD A, E
        Reg_A = Reg_E;
        break;
    case 0x7C: // LD A, H
        Reg_A = Reg_H;
        break;
    case 0x7D: // LD A, L
        Reg_A = Reg_L;
        break;
    case 0x7E: // LD A, (HL)
        Reg_A = MemGet(Reg_H, Reg_L);
        break;
    case 0x7F: // LD A, A
        Reg_A = Reg_A;
        break;

    // Instructions on standard registers - mathematical and logical operations
    case 0x80: // ADD A, B
        DoADD1(Reg_B, false);
        break;
    case 0x81: // ADD A, C
        DoADD1(Reg_C, false);
        break;
    case 0x82: // ADD A, D
        DoADD1(Reg_D, false);
        break;
    case 0x83: // ADD A, E
        DoADD1(Reg_E, false);
        break;
    case 0x84: // ADD A, H
        DoADD1(Reg_H, false);
        break;
    case 0x85: // ADD A, L
        DoADD1(Reg_L, false);
        break;
    case 0x86: // ADD A, (HL)
        DoADD1(MemGet(Reg_H, Reg_L), false);
        break;
    case 0x87: // ADD A, A
        DoADD1(Reg_A, false);
        break;
    case 0x88: // ADC A, B
        DoADD1(Reg_B, true);
        break;
    case 0x89: // ADC A, C
        DoADD1(Reg_C, true);
        break;
    case 0x8A: // ADC A, D
        DoADD1(Reg_D, true);
        break;
    case 0x8B: // ADC A, E
        DoADD1(Reg_E, true);
        break;
    case 0x8C: // ADC A, H
        DoADD1(Reg_H, true);
        break;
    case 0x8D: // ADC A, L
        DoADD1(Reg_L, true);
        break;
    case 0x8E: // ADC A, (HL)
        DoADD1(MemGet(Reg_H, Reg_L), true);
        break;
    case 0x8F: // ADC A, A
        DoADD1(Reg_A, true);
        break;
    case 0x90: // SUB B
        DoSUB1(Reg_B, false);
        break;
    case 0x91: // SUB C
        DoSUB1(Reg_C, false);
        break;
    case 0x92: // SUB D
        DoSUB1(Reg_D, false);
        break;
    case 0x93: // SUB E
        DoSUB1(Reg_E, false);
        break;
    case 0x94: // SUB H
        DoSUB1(Reg_H, false);
        break;
    case 0x95: // SUB L
        DoSUB1(Reg_L, false);
        break;
    case 0x96: // SUB (HL)
        DoSUB1(MemGet(Reg_H, Reg_L), false);
        break;
    case 0x97: // SUB A
        DoSUB1(Reg_A, false);
        break;
    case 0x98: // SBC A, B
        DoSUB1(Reg_B, true);
        break;
    case 0x99: // SBC A, C
        DoSUB1(Reg_C, true);
        break;
    case 0x9A: // SBC A, D
        DoSUB1(Reg_D, true);
        break;
    case 0x9B: // SBC A, E
        DoSUB1(Reg_E, true);
        break;
    case 0x9C: // SBC A, H
        DoSUB1(Reg_H, true);
        break;
    case 0x9D: // SBC A, L
        DoSUB1(Reg_L, true);
        break;
    case 0x9E: // SBC A, (HL)
        DoSUB1(MemGet(Reg_H, Reg_L), true);
        break;
    case 0x9F: // SBC A, A
        DoSUB1(Reg_A, true);
        break;
    case 0xA0: // AND B
        DoAND(Reg_B);
        break;
    case 0xA1: // AND C
        DoAND(Reg_C);
        break;
    case 0xA2: // AND D
        DoAND(Reg_D);
        break;
    case 0xA3: // AND E
        DoAND(Reg_E);
        break;
    case 0xA4: // AND H
        DoAND(Reg_H);
        break;
    case 0xA5: // AND L
        DoAND(Reg_L);
        break;
    case 0xA6: // AND (HL)
        DoAND(MemGet(Reg_H, Reg_L));
        break;
    case 0xA7: // AND A
        DoAND(Reg_A);
        break;
    case 0xA8: // XOR B
        DoXOR(Reg_B);
        break;
    case 0xA9: // XOR C
        DoXOR(Reg_C);
        break;
    case 0xAA: // XOR D
        DoXOR(Reg_D);
        break;
    case 0xAB: // XOR E
        DoXOR(Reg_E);
        break;
    case 0xAC: // XOR H
        DoXOR(Reg_H);
        break;
    case 0xAD: // XOR L
        DoXOR(Reg_L);
        break;
    case 0xAE: // XOR (HL)
        DoXOR(MemGet(Reg_H, Reg_L));
        break;
    case 0xAF: // XOR A
        DoXOR(Reg_A);
        break;
    case 0xB0: // OR B
        DoOR(Reg_B);
        break;
    case 0xB1: // OR C
        DoOR(Reg_C);
        break;
    case 0xB2: // OR D
        DoOR(Reg_D);
        break;
    case 0xB3: // OR E
        DoOR(Reg_E);
        break;
    case 0xB4: // OR H
        DoOR(Reg_H);
        break;
    case 0xB5: // OR L
        DoOR(Reg_L);
        break;
    case 0xB6: // OR (HL)
        DoOR(MemGet(Reg_H, Reg_L));
        break;
    case 0xB7: // OR A
        DoOR(Reg_A);
        break;
    case 0xB8: // CP B
        DoCP(Reg_B);
        break;
    case 0xB9: // CP C
        DoCP(Reg_C);
        break;
    case 0xBA: // CP D
        DoCP(Reg_D);
        break;
    case 0xBB: // CP E
        DoCP(Reg_E);
        break;
    case 0xBC: // CP H
        DoCP(Reg_H);
        break;
    case 0xBD: // CP L
        DoCP(Reg_L);
        break;
    case 0xBE: // CP (HL)
        DoCP(MemGet(Reg_H, Reg_L));
        break;
    case 0xBF: // CP A
        DoCP(Reg_A);
        break;


    // Stack instructions
    case 0xC1: // POP BC
        DoPOP(Reg_B, Reg_C);
        break;
    case 0xC5: // PUSH BC
        DoPUSH(Reg_B, Reg_C);
        break;
    case 0xD1: // POP DE
        DoPOP(Reg_D, Reg_E);
        break;
    case 0xD5: // PUSH DE
        DoPUSH(Reg_D, Reg_E);
        break;
    case 0xE1: // POP HL
        DoPOP(Reg_H, Reg_L);
        break;
    case 0xE5: // PUSH HL
        DoPUSH(Reg_H, Reg_L);
        break;
    case 0xF1: // POP AF
        DoPOP(Reg_A, Reg_F);
        break;
    case 0xF5: // PUSH AF
        DoPUSH(Reg_A, Reg_F);
        break;

    // Two-bytes calls
    case 0xCD: // CALL NN
        DoCALL(true);
        break;
    case 0xC4: // CALL NZ NN
        DoCALL(!FlagZGet);
        break;
    case 0xCC: // CALL Z, NN
        DoCALL(FlagZGet);
        break;
    case 0xD4: // CALL NC, NN
        DoCALL(!FlagCGet);
        break;
    case 0xDC: // CALL C, NN
        DoCALL(FlagCGet);
        break;
    case 0xE4: // CALL PO, NN
        DoCALL(!FlagPGet);
        break;
    case 0xEC: // CALL PE, NN
        DoCALL(FlagPGet);
        break;
    case 0xF4: // CALL P, NN
        DoCALL(!FlagSGet);
        break;
    case 0xFC: // CALL M, NN
        DoCALL(FlagSGet);
        break;

    // One-byte calls
    case 0xC7: // RST 00
        DoRST(0x00);
        break;
    case 0xCF: // RST 08
        DoRST(0x08);
        break;
    case 0xD7: // RST 10
        DoRST(0x10);
        break;
    case 0xDF: // RST 18
        DoRST(0x18);
        break;
    case 0xE7: // RST 20
        DoRST(0x20);
        break;
    case 0xEF: // RST 28
        DoRST(0x28);
        break;
    case 0xF7: // RST 30
        DoRST(0x30);
        break;
    case 0xFF: // RST 38
        DoRST(0x38);
        break;

    // Returns from call
    case 0xC9: // RET
        DoRETI();
        break;
    case 0xC0: // RET NZ
        DoRET(!FlagZGet);
        break;
    case 0xC8: // RET Z
        DoRET(FlagZGet);
        break;
    case 0xD0: // RET NC
        DoRET(!FlagCGet);
        break;
    case 0xD8: // RET C
        DoRET(FlagCGet);
        break;
    case 0xE0: // RET PO
        DoRET(!FlagPGet);
        break;
    case 0xE8: // RET PE
        DoRET(FlagPGet);
        break;
    case 0xF0: // RET P
        DoRET(!FlagSGet);
        break;
    case 0xF8: // RET M
        DoRET(FlagSGet);
        break;


    // CB instruction serie - instructions on single register byte pointed by HL
    case 0xCB:
        OpCode1 = MemPrg(Reg_PC);
        Reg_PC++;

        // Reading register
        switch (OpCode1 & b00000111)
        {
            case 0: Data1 = Reg_B; break;
            case 1: Data1 = Reg_C; break;
            case 2: Data1 = Reg_D; break;
            case 3: Data1 = Reg_E; break;
            case 4: Data1 = Reg_H; break;
            case 5: Data1 = Reg_L; break;
            case 6: Data1 = MemGet(Reg_H, Reg_L); break;
            case 7: Data1 = Reg_A; break;
        }

        // Performing operation
        switch (OpCode1 & b11111000)
        {
            case 0x00: // RLC X
                DoRLC(Data1, true);
                break;
            case 0x08: // RRC X
                DoRRC(Data1, true);
                break;
            case 0x10: // RL X
                DoRL(Data1, true);
                break;
            case 0x18: // RR X
                DoRR(Data1, true);
                break;
            case 0x20: // SLA X
                DoSLA(Data1);
                break;
            case 0x28: // SRA X
                DoSRA(Data1);
                break;
            case 0x38: // SRL X
                DoSRL(Data1);
                break;
            case 0x40: // BIT 0, X
                if (Data1 & b00000001) { FlagZClr; } else { FlagZSet; FlagPSet; } FlagHSet; FlagNClr; FlagSClr;
                break;
            case 0x48: // BIT 1, X
                if (Data1 & b00000010) { FlagZClr; } else { FlagZSet; FlagPSet; } FlagHSet; FlagNClr; FlagSClr;
                break;
            case 0x50: // BIT 2, X
                if (Data1 & b00000100) { FlagZClr; } else { FlagZSet; FlagPSet; } FlagHSet; FlagNClr; FlagSClr;
                break;
            case 0x58: // BIT 3, X
                if (Data1 & b00001000) { FlagZClr; } else { FlagZSet; FlagPSet; } FlagHSet; FlagNClr; FlagSClr;
                break;
            case 0x60: // BIT 4, X
                if (Data1 & b00010000) { FlagZClr; } else { FlagZSet; FlagPSet; } FlagHSet; FlagNClr; FlagSClr;
                break;
            case 0x68: // BIT 5, X
                if (Data1 & b00100000) { FlagZClr; } else { FlagZSet; FlagPSet; } FlagHSet; FlagNClr; FlagSClr;
                break;
            case 0x70: // BIT 6, X
                if (Data1 & b01000000) { FlagZClr; } else { FlagZSet; FlagPSet; } FlagHSet; FlagNClr; FlagSClr;
                break;
            case 0x78: // BIT 7, X
                if (Data1 & b10000000) { FlagZClr; } else { FlagZSet; FlagPSet; } FlagHSet; FlagNClr; FlagSClr;
                break;
            case 0x80: // RES 0, X
                Data1 = Data1 & b11111110;
                break;
            case 0x88: // RES 1, X
                Data1 = Data1 & b11111101;
                break;
            case 0x90: // RES 2, X
                Data1 = Data1 & b11111011;
                break;
            case 0x98: // RES 3, X
                Data1 = Data1 & b11110111;
                break;
            case 0xA0: // RES 4, X
                Data1 = Data1 & b11101111;
                break;
            case 0xA8: // RES 5, X
                Data1 = Data1 & b11011111;
                break;
            case 0xB0: // RES 6, X
                Data1 = Data1 & b10111111;
                break;
            case 0xB8: // RES 7, X
                Data1 = Data1 & b01111111;
                break;
            case 0xC0: // SET 0, X
                Data1 = Data1 | b00000001;
                break;
            case 0xC8: // SET 1, X
                Data1 = Data1 | b00000010;
                break;
            case 0xD0: // SET 2, X
                Data1 = Data1 | b00000100;
                break;
            case 0xD8: // SET 3, X
                Data1 = Data1 | b00001000;
                break;
            case 0xE0: // SET 4, X
                Data1 = Data1 | b00010000;
                break;
            case 0xE8: // SET 5, X
                Data1 = Data1 | b00100000;
                break;
            case 0xF0: // SET 6, X
                Data1 = Data1 | b01000000;
                break;
            case 0xF8: // SET 7, X
                Data1 = Data1 | b10000000;
                break;
            default:
                Reg_PC--;
                Reg_PC--;
                MemData[SwapPage] = 255;
                ErrorStatus = "Unknown instruction at " + Eden::IntToHex16(Reg_PC) + ": " + Eden::IntToHex8(OpCode0) + " " + Eden::IntToHex8(OpCode1);
                break;
        }

        // Writing register
        switch (OpCode1 & b00000111)
        {
            case 0: Reg_B = Data1; break;
            case 1: Reg_C = Data1; break;
            case 2: Reg_D = Data1; break;
            case 3: Reg_E = Data1; break;
            case 4: Reg_H = Data1; break;
            case 5: Reg_L = Data1; break;
            case 6: MemSet(Reg_H, Reg_L, Data1); break;
            case 7: Reg_A = Data1; break;
        }

        break;
    case 0xDD: // DD instruction serie - instructions using IX register
    case 0xFD: // ED instruction serie - instructions using IY register
        OpCode1 = MemPrg(Reg_PC);
        Reg_PC++;

        // Reading IX/IY register
        if (OpCode0 == 0xDD)
        {
            Reg_Idx = Reg_IX;
        }
        else
        {
            Reg_Idx = Reg_IY;
        }

        // Performing instruction
        switch (OpCode1)
        {
        case 0x09: // ADD I_, BC
            DoADD2_I_(Reg_B, Reg_C);
            break;
        case 0x19: // ADD I_, DE
            DoADD2_I_(Reg_D, Reg_E);
            break;
        case 0x29: // ADD I_, I_
            DoADD2_I_(Reg_Idx);
            break;
        case 0x39: // ADD I_, SP
            DoADD2_I_(Reg_SP);
            break;
        case 0x23: // INC I_
            Reg_Idx++;
            break;
        case 0x2B: // DEC I_
            Reg_Idx--;
            break;
        case 0x34: // INC (I_ + d)
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            Data1 = MemGet(RegIdxD);
            DoINC(Data1);
            MemSet(RegIdxD, Data1);
            break;
        case 0x35: // DEC (I_ + d)
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            Data1 = MemGet(RegIdxD);
            DoDEC(Data1);
            MemSet(RegIdxD, Data1);
            break;

        case 0x21: // LD I_, NN
            Data1I = MemPrg(Reg_PC);
            Reg_PC++;
            Data2I = MemPrg(Reg_PC);
            Reg_PC++;
            Reg_Idx = (Data2I << 8) + Data1I;
            break;
        case 0x22: // LD (NN), I_
            Data1I = MemPrg(Reg_PC);
            Reg_PC++;
            Data2I = MemPrg(Reg_PC);
            Reg_PC++;
            MemSet((Data2I << 8) + Data1I, Reg_Idx & 255);
            MemSet((Data2I << 8) + Data1I + 1, Reg_Idx >> 8);
            break;
        case 0x2A: // LD I_, (NN)
            Data1I = MemPrg(Reg_PC);
            Reg_PC++;
            Data2I = MemPrg(Reg_PC);
            Reg_PC++;
            Reg_Idx = MemGet(Data2I, Data1I);
            break;
        case 0xF9: // LD SP, I_
            Reg_SP = Reg_Idx;
            break;
        case 0x36: // LD (I_ + d), N
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            Arg2 = MemPrg(Reg_PC);
            Reg_PC++;
            MemSet(RegIdxD, Arg2);
            break;

        case 0x46: // LD B, (I_ + d)
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            Reg_B = MemGet(RegIdxD);
            break;
        case 0x4E: // LD C, (I_ + d)
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            Reg_C = MemGet(RegIdxD);
            break;
        case 0x56: // LD D, (I_ + d)
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            Reg_D = MemGet(RegIdxD);
            break;
        case 0x5E: // LD E, (I_ + d)
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            Reg_E = MemGet(RegIdxD);
            break;
        case 0x66: // LD H, (I_ + d)
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            Reg_H = MemGet(RegIdxD);
            break;
        case 0x6E: // LD L, (I_ + d)
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            Reg_L = MemGet(RegIdxD);
            break;
        case 0x7E: // LD A, (I_ + d)
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            Reg_A = MemGet(RegIdxD);
            break;

        case 0x70: // LD (I_ + d), B
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            MemSet(RegIdxD, Reg_B);
            break;
        case 0x71: // LD (I_ + d), C
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            MemSet(RegIdxD, Reg_C);
            break;
        case 0x72: // LD (I_ + d), D
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            MemSet(RegIdxD, Reg_D);
            break;
        case 0x73: // LD (I_ + d), E
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            MemSet(RegIdxD, Reg_E);
            break;
        case 0x74: // LD (I_ + d), H
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            MemSet(RegIdxD, Reg_H);
            break;
        case 0x75: // LD (I_ + d), L
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            MemSet(RegIdxD, Reg_L);
            break;
        case 0x77: // LD (I_ + d), A
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            MemSet(RegIdxD, Reg_A);
            break;

        case 0x86: // ADD A, (I_ + d)
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            DoADD1(MemGet(RegIdxD), false);
            break;
        case 0x8E: // ADC A, (I_ + d)
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            DoADD1(MemGet(RegIdxD), true);
            break;
        case 0x96: // SUB, (I_ + d)
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            DoSUB1(MemGet(RegIdxD), false);
            break;
        case 0x9E: // SBC A, (I_ + d)
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            DoSUB1(MemGet(RegIdxD), true);
            break;
        case 0xA6: // AND, (I_ + d)
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            DoAND(MemGet(RegIdxD));
            break;
        case 0xAE: // XOR, (I_ + d)
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            DoXOR(MemGet(RegIdxD));
            break;
        case 0xB6: // OR, (I_ + d)
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            DoOR(MemGet(RegIdxD));
            break;
        case 0xBE: // CP, (I_ + d)
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            DoCP(MemGet(RegIdxD));
            break;

        case 0xE3: // EX (SP), I_
            Data1 = (Reg_Idx & 255);
            Data2 = (Reg_Idx >> 8);
            Data3 = MemGet(Reg_SP);
            Data4 = MemGet(Reg_SP + 1);
            MemSet(Reg_SP, Data1);
            MemSet(Reg_SP + 1, Data2);
            Reg_Idx = (((ushort)Data4) << 8) + ((ushort)Data3);
            break;
        case 0xE1: // POP I_
            DoPOP(Data1, Data2);
            Reg_Idx = (((ushort)Data1) << 8) + ((ushort)Data2);
            break;
        case 0xE5: // PUSH I_
            Data1 = (Reg_Idx >> 8);
            Data2 = (Reg_Idx & 255);
            DoPUSH(Data1, Data2);
            break;
        case 0xE9: // JP (I_)
            Reg_PC = Reg_Idx;
            break;

        case 0xCB: // CB instructions
            Arg1 = MemPrg(Reg_PC);
            Reg_PC++;
            OpCode2 = MemPrg(Reg_PC);
            Reg_PC++;
            Data1 = MemGet(RegIdxD);
            switch (OpCode2)
            {
                case 0x06: // RLC (I_ + d)
                    DoRLC(Data1, true);
                    break;
                case 0x0E: // RRC (I_ + d)
                    DoRRC(Data1, true);
                    break;
                case 0x16: // RL (I_ + d)
                    DoRL(Data1, true);
                    break;
                case 0x1E: // RR (I_ + d)
                    DoRR(Data1, true);
                    break;

                case 0x26: // SLA (I_ + d)
                    DoSLA(Data1);
                    break;
                case 0x2E: // SRA (I_ + d)
                    DoSRA(Data1);
                    break;
                case 0x3E: // SRL (I_ + d)
                    DoSRL(Data1);
                    break;

                case 0x46: // BIT 0, (I_ + d)
                    if (Data1 & b00000001) { FlagZClr; } else { FlagZSet; } FlagHSet; FlagNClr;
                    break;
                case 0x4E: // BIT 1, (I_ + d)
                    if (Data1 & b00000010) { FlagZClr; } else { FlagZSet; } FlagHSet; FlagNClr;
                    break;
                case 0x56: // BIT 2, (I_ + d)
                    if (Data1 & b00000100) { FlagZClr; } else { FlagZSet; } FlagHSet; FlagNClr;
                    break;
                case 0x5E: // BIT 3, (I_ + d)
                    if (Data1 & b00001000) { FlagZClr; } else { FlagZSet; } FlagHSet; FlagNClr;
                    break;
                case 0x66: // BIT 4, (I_ + d)
                    if (Data1 & b00010000) { FlagZClr; } else { FlagZSet; } FlagHSet; FlagNClr;
                    break;
                case 0x6E: // BIT 5, (I_ + d)
                    if (Data1 & b00100000) { FlagZClr; } else { FlagZSet; } FlagHSet; FlagNClr;
                    break;
                case 0x76: // BIT 6, (I_ + d)
                    if (Data1 & b01000000) { FlagZClr; } else { FlagZSet; } FlagHSet; FlagNClr;
                    break;
                case 0x7E: // BIT 7, (I_ + d)
                    if (Data1 & b10000000) { FlagZClr; } else { FlagZSet; } FlagHSet; FlagNClr;
                    break;

                case 0x86: // RES 0, (I_ + d)
                    Data1 = Data1 & b11111110;
                    break;
                case 0x8E: // RES 1, (I_ + d)
                    Data1 = Data1 & b11111101;
                    break;
                case 0x96: // RES 2, (I_ + d)
                    Data1 = Data1 & b11111011;
                    break;
                case 0x9E: // RES 3, (I_ + d)
                    Data1 = Data1 & b11110111;
                    break;
                case 0xA6: // RES 4, (I_ + d)
                    Data1 = Data1 & b11101111;
                    break;
                case 0xAE: // RES 5, (I_ + d)
                    Data1 = Data1 & b11011111;
                    break;
                case 0xB6: // RES 6, (I_ + d)
                    Data1 = Data1 & b10111111;
                    break;
                case 0xBE: // RES 7, (I_ + d)
                    Data1 = Data1 & b01111111;
                    break;

                case 0xC6: // SET 0, (I_ + d)
                    Data1 = Data1 | b00000001;
                    break;
                case 0xCE: // SET 1, (I_ + d)
                    Data1 = Data1 | b00000010;
                    break;
                case 0xD6: // SET 2, (I_ + d)
                    Data1 = Data1 | b00000100;
                    break;
                case 0xDE: // SET 3, (I_ + d)
                    Data1 = Data1 | b00001000;
                    break;
                case 0xE6: // SET 4, (I_ + d)
                    Data1 = Data1 | b00010000;
                    break;
                case 0xEE: // SET 5, (I_ + d)
                    Data1 = Data1 | b00100000;
                    break;
                case 0xF6: // SET 6, (I_ + d)
                    Data1 = Data1 | b01000000;
                    break;
                case 0xFE: // SET 7, (I_ + d)
                    Data1 = Data1 | b10000000;
                    break;

                default:
                    Reg_PC--;
                    Reg_PC--;
                    Reg_PC--;
                    Reg_PC--;
                    MemData[SwapPage] = 255;
                    ErrorStatus = "Unknown instruction at " + Eden::IntToHex16(Reg_PC) + ": " + Eden::IntToHex8(OpCode0) + " " + Eden::IntToHex8(OpCode1) + " __ " + Eden::IntToHex8(OpCode2);
                    break;
            }
            MemSet(RegIdxD, Data1);
            break;

        default:
            Reg_PC--;
            Reg_PC--;
            MemData[SwapPage] = 255;
            ErrorStatus = "Unknown instruction at " + Eden::IntToHex16(Reg_PC) + ": " + Eden::IntToHex8(OpCode0) + " " + Eden::IntToHex8(OpCode1);
            break;
        }

        // Writing IX/IY register
        if (OpCode0 == 0xDD)
        {
            Reg_IX = Reg_Idx;
        }
        else
        {
            Reg_IY = Reg_Idx;
        }


        break;
    case 0xED: // ED instructions
        OpCode1 = MemPrg(Reg_PC);
        Reg_PC++;
        switch (OpCode1)
        {
        // Multiplication - Z180
        case 0x4C: // MLT BC
            Data1I = Reg_B;
            Data2I = Reg_C;
            Data1I = Data1I * Data2I;
            Reg_B = Data1I >> 8;
            Reg_C = Data1I & 255;
            break;
        case 0x5C: // MLT DE
            Data1I = Reg_D;
            Data2I = Reg_E;
            Data1I = Data1I * Data2I;
            Reg_D = Data1I >> 8;
            Reg_E = Data1I & 255;
            break;
        case 0x6C: // MLT HL
            Data1I = Reg_H;
            Data2I = Reg_L;
            Data1I = Data1I * Data2I;
            Reg_H = Data1I >> 8;
            Reg_L = Data1I & 255;
            break;
        case 0x7C: // MLT SP
            Data1I = Reg_SP >> 8;
            Data2I = Reg_SP & 255;
            Data1I = Data1I * Data2I;
            Reg_SP = Data1I;
            break;

        // Checking conjunction parity - Z180
        case 0x04: // TST B
            DoTST(Reg_B);
            break;
        case 0x0C: // TST C
            DoTST(Reg_C);
            break;
        case 0x14: // TST D
            DoTST(Reg_D);
            break;
        case 0x1C: // TST E
            DoTST(Reg_E);
            break;
        case 0x24: // TST H
            DoTST(Reg_H);
            break;
        case 0x2C: // TST L
            DoTST(Reg_L);
            break;
        case 0x34: // TST (HL)
            DoTST(MemGet(Reg_H, Reg_L));
            break;
        case 0x3C: // TST A
            DoTST(Reg_A);
            break;
        case 0x64: // TST N
            DoTST(MemPrg(Reg_PC));
            Reg_PC++;
            break;


        // ########################################################


        // Copying 8-bit values
        case 0x47: // LD I, A
            Reg_I = Reg_A;
            break;
        case 0x4F: // LD R, A
            Reg_R = Reg_A;
            break;
        case 0x57: // LD A, I
            Reg_A = Reg_I;
            if (Reg_IFF2)
            {
                FlagPSet;
            }
            else
            {
                FlagPClr;
            }
            SetFlagsSZ(Reg_I);
            FlagHClr;
            FlagNClr;
            break;
        case 0x5F: // LD A, R
            Reg_A = Reg_R;
            if (Reg_IFF2)
            {
                FlagPSet;
            }
            else
            {
                FlagPClr;
            }
            SetFlagsSZ(Reg_R);
            FlagHClr;
            FlagNClr;
            break;

        // Copying 16-bit values
        case 0x43: // LD (NN), BC
            MemSet(MemPrg(Reg_PC + 1), MemPrg(Reg_PC),     Reg_C);
            MemSet(MemPrg(Reg_PC + 1), MemPrg(Reg_PC) + 1, Reg_B);
            Reg_PC++;
            Reg_PC++;
            break;
        case 0x53: // LD (NN), DE
            MemSet(MemPrg(Reg_PC + 1), MemPrg(Reg_PC),     Reg_E);
            MemSet(MemPrg(Reg_PC + 1), MemPrg(Reg_PC) + 1, Reg_D);
            Reg_PC++;
            Reg_PC++;
            break;
        case 0x63: // LD (NN), HL
            MemSet(MemPrg(Reg_PC + 1), MemPrg(Reg_PC),     Reg_L);
            MemSet(MemPrg(Reg_PC + 1), MemPrg(Reg_PC) + 1, Reg_H);
            Reg_PC++;
            Reg_PC++;
            break;
        case 0x73: // LD (NN), SP
            MemSet(MemPrg(Reg_PC + 1), MemPrg(Reg_PC),     Reg_SP & 255);
            MemSet(MemPrg(Reg_PC + 1), MemPrg(Reg_PC) + 1, Reg_SP >> 8);
            Reg_PC++;
            Reg_PC++;
            break;

        case 0x4B: // LD BC, (NN)
            Reg_C = MemGet(MemPrg(Reg_PC + 1), MemPrg(Reg_PC));
            Reg_B = MemGet(MemPrg(Reg_PC + 1), MemPrg(Reg_PC) + 1);
            Reg_PC++;
            Reg_PC++;
            break;
        case 0x5B: // LD DE, (NN)
            Reg_E = MemGet(MemPrg(Reg_PC + 1), MemPrg(Reg_PC));
            Reg_D = MemGet(MemPrg(Reg_PC + 1), MemPrg(Reg_PC) + 1);
            Reg_PC++;
            Reg_PC++;
            break;
        case 0x6B: // LD HL, (NN)
            Reg_L = MemGet(MemPrg(Reg_PC + 1), MemPrg(Reg_PC));
            Reg_H = MemGet(MemPrg(Reg_PC + 1), MemPrg(Reg_PC) + 1);
            Reg_PC++;
            Reg_PC++;
            break;
        case 0x7B: // LD SP, (NN)
            Data1I = MemGet(MemPrg(Reg_PC + 1), MemPrg(Reg_PC));
            Data2I = MemGet(MemPrg(Reg_PC + 1), MemPrg(Reg_PC) + 1);
            Reg_SP = Data1I + (Data2I << 8);
            Reg_PC++;
            Reg_PC++;
            break;

        // Operations on accumulator
        case 0x44: // NEG
            Data1 = Reg_A;
            Reg_A = 0;
            DoSUB1(Data1, false);
            break;
        case 0x67: // RRD
            Data1 = (Reg_A & b11110000);
            Data2 = (Reg_A & b00001111);
            Data3 = (MemGet(Reg_H, Reg_L) & b11110000);
            Data4 = (MemGet(Reg_H, Reg_L) & b00001111);
            Reg_A = Data1 | Data4;
            MemSet(Reg_H, Reg_L, (Data2 << 4) | (Data3 >> 4));
            break;
        case 0x6F: // RLD
            Data1 = (Reg_A & b11110000);
            Data2 = (Reg_A & b00001111);
            Data3 = (MemGet(Reg_H, Reg_L) & b11110000);
            Data4 = (MemGet(Reg_H, Reg_L) & b00001111);
            Reg_A = Data1 | (Data3 >> 4);
            MemSet(Reg_H, Reg_L, (Data4 << 4) | Data2);
            break;

        // Adding on 16-bit values
        case 0x4A: // ADC HL, BC
            DoADD2_HL(Reg_B, Reg_C, true);
            break;
        case 0x5A: // ADC HL, DE
            DoADD2_HL(Reg_D, Reg_E, true);
            break;
        case 0x6A: // ADC HL, HL
            DoADD2_HL(Reg_H, Reg_L, true);
            break;
        case 0x7A: // ADC HL, SP
            DoADD2_HL(Reg_SP, true);
            break;

        // Subtracting on 16-bit values
        case 0x42: // SBC HL, BC
            DoSUB2(Reg_B, Reg_C, true);
            break;
        case 0x52: // SBC HL, DE
            DoSUB2(Reg_D, Reg_E, true);
            break;
        case 0x62: // SBC HL, HL
            DoSUB2(Reg_H, Reg_L, true);
            break;
        case 0x72: // SBC HL, SP
            DoSUB2(Reg_SP, true);
            break;

        // Block data loading
        case 0xA0: // LDI
            DoBlock_LD();
            Reg_E++;
            if (Reg_E == 0)
            {
                Reg_D++;
            }
            DoBlock2I();
            FlagHClr;
            if ((Reg_B) || (Reg_C)) { FlagPSet; } else { FlagPClr; }
            FlagNClr;
            break;
        case 0xA8: // LDD
            DoBlock_LD();
            Reg_E--;
            if (Reg_E == 255)
            {
                Reg_D--;
            }
            DoBlock2D();
            FlagHClr;
            if ((Reg_B) || (Reg_C)) { FlagPSet; } else { FlagPClr; }
            FlagNClr;
            break;
        case 0xB0: // LDIR
            DoBlock_LD();
            Reg_E++;
            if (Reg_E == 0)
            {
                Reg_D++;
            }
            DoBlock2IR(false);
            FlagHClr;
            FlagPClr;
            FlagNClr;
            break;
        case 0xB8: // LDDR
            DoBlock_LD();
            Reg_E--;
            if (Reg_E == 255)
            {
                Reg_D--;
            }
            DoBlock2DR(false);
            FlagHClr;
            FlagPClr;
            FlagNClr;
            break;

        // Block data comparison
        case 0xA1: // CPI
            DoBlock_CP();
            DoBlock2I();
            if ((Reg_B) || (Reg_C)) { FlagPSet; } else { FlagPClr; }
            FlagNSet;
            break;
        case 0xA9: // CPD
            DoBlock_CP();
            DoBlock2D();
            if ((Reg_B) || (Reg_C)) { FlagPSet; } else { FlagPClr; }
            FlagNSet;
            break;
        case 0xB1: // CPIR
            DoBlock_CP();
            DoBlock2IR(true);
            if ((Reg_B) || (Reg_C)) { FlagPSet; } else { FlagPClr; }
            FlagNSet;
            break;
        case 0xB9: // CPDR
            DoBlock_CP();
            DoBlock2DR(true);
            if ((Reg_B) || (Reg_C)) { FlagPSet; } else { FlagPClr; }
            FlagNSet;
            break;

        default:
            Reg_PC--;
            Reg_PC--;
            MemData[SwapPage] = 255;
            ErrorStatus = "Unknown instruction at " + Eden::IntToHex16(Reg_PC) + ": " + Eden::IntToHex8(OpCode0) + " " + Eden::IntToHex8(OpCode1);
            break;
        }

        break;


    default:
        Reg_PC--;
        MemData[SwapPage] = 255;
        ErrorStatus = "Unknown instruction at " + Eden::IntToHex16(Reg_PC) + ": " + Eden::IntToHex8(OpCode0);
    }

    //if ((Reg_PC >= (0x1565)) && (Reg_PC <= (0x159B)))
    /*{
        cout << "PO:    A=" << Eden::IntToHex8(Reg_A) << " B=" << Eden::IntToHex8(Reg_B) << " C=" << Eden::IntToHex8(Reg_C) << " D=" << Eden::IntToHex8(Reg_D) << " E=" << Eden::IntToHex8(Reg_E);
        cout << "   IX=" << Eden::IntToHex16(Reg_IX) << " IY=" << Eden::IntToHex16(Reg_IY) ;
        cout << "   SP=" << Eden::IntToHex16(Reg_SP) << " H=" << Eden::IntToHex8(Reg_H) << " L=" << Eden::IntToHex8(Reg_L) << endl;
    }*/

}

void ScriptMachineZ180::DoReset()
{
    Reg_PC = 0;

    Reg_IFF1 = false;
    Reg_IFF2 = false;

    Reg_I = 0;

    Reg_A = 0;
    Reg_A_ = 0;
    Reg_F = 0;
    Reg_F_ = 0;

    Reg_B = 0;
    Reg_B_ = 0;
    Reg_C = 0;
    Reg_C_ = 0;
    Reg_D = 0;
    Reg_D_ = 0;
    Reg_E = 0;
    Reg_E_ = 0;
    Reg_H = 0;
    Reg_H_ = 0;
    Reg_L = 0;
    Reg_L_ = 0;

    Reg_IX = 0;
    Reg_IY = 0;
    Reg_SP = 0;

    Reg_R = 0;
}



void ScriptMachineZ180::DoCALL(bool Condition)
{
    ushort AddrLo = MemPrg(Reg_PC);
    Reg_PC++;
    ushort AddrHi = MemPrg(Reg_PC);
    Reg_PC++;
    if (Condition)
    {
        DoJumpAbs((AddrHi << 8) + AddrLo, true);
    }
}

void ScriptMachineZ180::DoPUSH(uchar &ValH, uchar &ValL)
{
    Reg_SP--;
    MemSet(Reg_SP, ValH);
    Reg_SP--;
    MemSet(Reg_SP, ValL);
}

void ScriptMachineZ180::DoPOP(uchar &ValH, uchar &ValL)
{
    ValL = MemGet(Reg_SP);
    Reg_SP++;
    ValH = MemGet(Reg_SP);
    Reg_SP++;
}

void ScriptMachineZ180::DoRET(bool Condition)
{
    if (Condition)
    {
        uchar PCH;
        uchar PCL;
        DoPOP(PCH, PCL);
        Reg_PC = (((ushort)PCH) << 8) + ((ushort)PCL);
    }
}

void ScriptMachineZ180::DoRETI()
{
    uchar PCH;
    uchar PCL;
    DoPOP(PCH, PCL);
    Reg_PC = (((ushort)PCH) << 8) + ((ushort)PCL);
}

void ScriptMachineZ180::DoRST(int Addr)
{
    DoJumpAbs(Addr, true);
}

void ScriptMachineZ180::DoJR(bool Condition)
{
    uchar TempData = MemPrg(Reg_PC);
    Reg_PC++;
    if (Condition)
    {
        DoJumpRel(TempData, false);
    }
}

void ScriptMachineZ180::DoADD1(uchar Val, bool Carry)
{
    int C = Carry ? (FlagCGet ? 1 : 0) : 0;
    int A = Reg_A;
    int Res = A + Val + C;
    int ResX = ((int)((char)Reg_A)) + ((int)((char)Val)) + C;

    if ((ResX > 127) || (ResX < -128))
    {
        FlagPSet;
    }
    else
    {
        FlagPClr;
    }

    if (((A & 15) + (Val & 15) + C) > 15)
    {
        FlagHSet;
    }
    else
    {
        FlagHClr;
    }
    if (Res > 255)
    {
        FlagCSet;
        Res = Res - 256;
    }
    else
    {
        FlagCClr;
    }

    Reg_A = Res & 255;
    SetFlagsSZ(Reg_A);
    FlagNClr;
}

void ScriptMachineZ180::DoADD2_HL(int Val, bool Carry)
{
    int C = Carry ? (FlagCGet ? 1 : 0) : 0;

    int A = Reg_H;
    A = A << 8;
    A = A + Reg_L;

    int Res = A + Val + C;
    int ResX = ((int)((short)A)) + ((int)((short)Val)) + C;

    if ((ResX > 32767) || (ResX < -32768))
    {
        FlagPSet;
    }
    else
    {
        FlagPClr;
    }

    if (((A & 4095) + (Val & 4095) + C) > 4095)
    {
        FlagHSet;
    }
    else
    {
        FlagHClr;
    }
    if (Res > 65535)
    {
        FlagCSet;
        Res = Res - 65536;
    }
    else
    {
        FlagCClr;
    }

    A = Res & 65535;
    Reg_L = A & 255;
    A = A >> 8;
    Reg_H = A & 255;
    if (Carry)
    {
        SetFlagsSZ_(Reg_H, Reg_L);
    }
    FlagNClr;
}

void ScriptMachineZ180::DoADD2_HL(int ValH, int ValL, bool Carry)
{
    DoADD2_HL((ValH << 8) + ValL, Carry);
}

void ScriptMachineZ180::DoADD2_I_(int Val)
{
    int A = Reg_Idx;

    int Res = A + Val;

    if (((A & 4095) + (Val & 4095)) > 4095)
    {
        FlagHSet;
    }
    else
    {
        FlagHClr;
    }
    if (Res > 65535)
    {
        FlagCSet;
        Res = Res - 65536;
    }
    else
    {
        FlagCClr;
    }

    A = Res & 65535;
    Reg_Idx = A;
    FlagNClr;
}

void ScriptMachineZ180::DoADD2_I_(int ValH, int ValL)
{
    DoADD2_I_((ValH << 8) + ValL);
}

void ScriptMachineZ180::DoSUB1(uchar Val, bool Carry)
{
    int C = Carry ? (FlagCGet ? 1 : 0) : 0;
    int A = Reg_A;

    int Res = A - (Val + C);
    int ResX = ((int)((char)Reg_A)) - ((int)((char)Val)) - C;

    if ((ResX > 127) || (ResX < -128))
    {
        FlagPSet;
    }
    else
    {
        FlagPClr;
    }

    if (Res < 0)
    {
        FlagCSet;
        Res = Res + 256;
    }
    else
    {
        FlagCClr;
    }
    if (((Val + 1) & 15) > (A & 15))
    {
        FlagHSet;
    }
    else
    {
        FlagHClr;
    }

    Reg_A = Res & 255;
    SetFlagsSZ(Reg_A);
    FlagNSet;
}

void ScriptMachineZ180::DoSUB2(int Val, bool Carry)
{
    int C = Carry ? (FlagCGet ? 1 : 0) : 0;

    int A = Reg_H;
    A = A << 8;
    A = A + Reg_L;

    int Res = A - (Val + C);
    int ResX = ((int)((short)A)) - ((int)((short)Val)) - C;

    if ((ResX > 32767) || (ResX < -32768))
    {
        FlagPSet;
    }
    else
    {
        FlagPClr;
    }

    if (Res < 0)
    {
        FlagCSet;
        Res = Res + 65536;
    }
    else
    {
        FlagCClr;
    }
    if (((Val + 1) & 4095) > (A & 4095))
    {
        FlagHSet;
    }
    else
    {
        FlagHClr;
    }

    A = Res & 65535;
    Reg_L = A & 255;
    A = A >> 8;
    Reg_H = A & 255;
    SetFlagsSZ_(Reg_H, Reg_L);
    FlagNSet;
}

void ScriptMachineZ180::DoSUB2(int ValH, int ValL, bool Carry)
{
    DoSUB2((ValH << 8) + ValL, Carry);
}

void ScriptMachineZ180::DoCP(uchar Val)
{
    uchar TempA = Reg_A;
    DoSUB1(Val, false);
    Reg_A = TempA;
}

void ScriptMachineZ180::DoTST(uchar Val)
{
    uchar TempA = Reg_A & Val;
    SetFlagsSZ(TempA);
    SetFlagsP(TempA);
    FlagHSet;
    FlagNClr;
    FlagCClr;
}

void ScriptMachineZ180::DoAND(uchar Val)
{
    Reg_A = Reg_A & Val;
    SetFlagsSZ(Reg_A);
    SetFlagsP(Reg_A);
    FlagHSet;
    FlagNClr;
    FlagCClr;
}

void ScriptMachineZ180::DoXOR(uchar Val)
{
    Reg_A = Reg_A ^ Val;
    SetFlagsSZ(Reg_A);
    SetFlagsP(Reg_A);
    FlagHClr;
    FlagNClr;
    FlagCClr;
}

void ScriptMachineZ180::DoOR(uchar Val)
{
    Reg_A = Reg_A | Val;
    SetFlagsSZ(Reg_A);
    SetFlagsP(Reg_A);
    FlagHClr;
    FlagNClr;
    FlagCClr;
}

void ScriptMachineZ180::DoINC(uchar &Reg)
{
    uchar Val1 = Reg + 1;
    if ((!(Val1 & b00001000)) && (Reg & b00001000))
    {
        FlagHSet;
    }
    else
    {
        FlagHClr;
    }
    if (Reg == 0x7F)
    {
        FlagPSet;
    }
    else
    {
        FlagPClr;
    }
    FlagNClr;
    Reg = Val1;
    SetFlagsSZ(Reg);
}

void ScriptMachineZ180::DoDEC(uchar &Reg)
{
    uchar Val1 = Reg - 1;
    if ((!(Reg & b00001000)) && (Val1 & b00001000))
    {
        FlagHSet;
    }
    else
    {
        FlagHClr;
    }
    if (Reg == 0x80)
    {
        FlagPSet;
    }
    else
    {
        FlagPClr;
    }
    FlagNSet;
    Reg = Val1;
    SetFlagsSZ(Reg);
}

void ScriptMachineZ180::DoRL(uchar &Reg, bool NoAcc)
{
    bool Carry = Reg & b10000000;
    Reg = Reg << 1;
    if (FlagCGet)
    {
        Reg++;
    }
    if (Carry)
    {
        FlagCSet;
    }
    else
    {
        FlagCClr;
    }
    FlagNClr;
    FlagHClr;

    if (NoAcc)
    {
        SetFlagsP(Reg);
        SetFlagsSZ(Reg);
    }
}

void ScriptMachineZ180::DoRR(uchar &Reg, bool NoAcc)
{
    bool Carry = Reg & b00000001;
    Reg = Reg >> 1;
    if (FlagCGet)
    {
        Reg += 128;
    }
    if (Carry)
    {
        FlagCSet;
    }
    else
    {
        FlagCClr;
    }
    FlagNClr;
    FlagHClr;

    if (NoAcc)
    {
        SetFlagsP(Reg);
        SetFlagsSZ(Reg);
    }
}

void ScriptMachineZ180::DoRLC(uchar &Reg, bool NoAcc)
{
    bool Carry = Reg & b10000000;
    Reg = Reg << 1;
    if (Carry)
    {
        FlagCSet;
        Reg++;
    }
    else
    {
        FlagCClr;
    }
    FlagNClr;
    FlagHClr;

    if (NoAcc)
    {
        SetFlagsP(Reg);
        SetFlagsSZ(Reg);
    }
}

void ScriptMachineZ180::DoRRC(uchar &Reg, bool NoAcc)
{
    bool Carry = Reg & b00000001;
    Reg = Reg >> 1;
    if (Carry)
    {
        FlagCSet;
        Reg += 128;
    }
    else
    {
        FlagCClr;
    }
    FlagNClr;
    FlagHClr;

    if (NoAcc)
    {
        SetFlagsP(Reg);
        SetFlagsSZ(Reg);
    }
}

void ScriptMachineZ180::DoSLA(uchar &Reg)
{
    if (Reg & b10000000)
    {
        FlagCSet;
    }
    else
    {
        FlagCClr;
    }
    Reg = Reg << 1;

    FlagNClr;
    FlagHClr;

    SetFlagsP(Reg);
    SetFlagsSZ(Reg);
}

void ScriptMachineZ180::DoSRA(uchar &Reg)
{
    uchar Carry = Reg & b10000000;
    if (Reg & b00000001)
    {
        FlagCSet;
    }
    else
    {
        FlagCClr;
    }
    Reg = Reg >> 1;
    Reg = Reg | Carry;

    FlagNClr;
    FlagHClr;

    SetFlagsP(Reg);
    SetFlagsSZ(Reg);
}

void ScriptMachineZ180::DoSRL(uchar &Reg)
{
    if (Reg & b00000001)
    {
        FlagCSet;
    }
    else
    {
        FlagCClr;
    }
    Reg = Reg >> 1;

    FlagNClr;
    FlagHClr;

    SetFlagsP(Reg);
    SetFlagsSZ(Reg);
}

void ScriptMachineZ180::DoJumpAbs(int Addr, bool Call)
{
    if (Call)
    {
        uchar PCH = Reg_PC >> 8;
        uchar PCL = Reg_PC & 255;
        DoPUSH(PCH, PCL);
    }
    Reg_PC = Addr;
}

void ScriptMachineZ180::DoJumpAbs(int AddrH, int AddrL, bool Call)
{
    if (Call)
    {
        uchar PCH = Reg_PC >> 8;
        uchar PCL = Reg_PC & 255;
        DoPUSH(PCH, PCL);
    }
    Reg_PC = (AddrH << 8) + AddrL;
}

void ScriptMachineZ180::DoJumpRel(int Addr, bool Call)
{
    if (Call)
    {
        uchar PCH = Reg_PC >> 8;
        uchar PCL = Reg_PC & 255;
        DoPUSH(PCH, PCL);
    }
    Reg_PC = Reg_PC + Addr;
    if (Addr > 127)
    {
        Reg_PC -= 256;
    }
}

void ScriptMachineZ180::DoBlock1I()
{
    Reg_L++;
    if (Reg_L == 0) { Reg_H++; }
    if (Reg_B == 0)
    {
        FlagZSet;
    }
    else
    {
        FlagZClr;
    }
    FlagNSet;
}

void ScriptMachineZ180::DoBlock1D()
{
    Reg_L--;
    if (Reg_L == 255) { Reg_H--; }
    if (Reg_B == 0)
    {
        FlagZSet;
    }
    else
    {
        FlagZClr;
    }
    FlagNSet;
}

void ScriptMachineZ180::DoBlock1IR()
{
    Reg_L++;
    if (Reg_L == 0) { Reg_H++; }
    if (Reg_B != 0)
    {
        Reg_PC--;
        Reg_PC--;
    }
    FlagZSet;
    FlagNSet;
}

void ScriptMachineZ180::DoBlock1DR()
{
    Reg_L--;
    if (Reg_L == 255) { Reg_H--; }
    if (Reg_B != 0)
    {
        Reg_PC--;
        Reg_PC--;
    }
    FlagZSet;
    FlagNSet;
}

void ScriptMachineZ180::DoBlock2I()
{
    if (Reg_A == MemGet(Reg_H, Reg_L))
    {
        FlagZSet;
    }
    else
    {
        FlagZClr;
    }
    Reg_L++;
    if (Reg_L == 0)
    {
        Reg_H++;
    }
    Reg_C--;
    if (Reg_C == 255)
    {
        Reg_B--;
    }
    if ((Reg_B == 0) && (Reg_C == 0))
    {
        FlagPClr;
    }
    else
    {
        FlagPSet;
    }
}

void ScriptMachineZ180::DoBlock2D()
{
    if (Reg_A == MemGet(Reg_H, Reg_L))
    {
        FlagZSet;
    }
    else
    {
        FlagZClr;
    }
    Reg_L--;
    if (Reg_L == 255)
    {
        Reg_H--;
    }
    Reg_C--;
    if (Reg_C == 255)
    {
        Reg_B--;
    }
    if ((Reg_B == 0) && (Reg_C == 0))
    {
        FlagPClr;
    }
    else
    {
        FlagPSet;
    }
}

void ScriptMachineZ180::DoBlock2IR(bool CompareA)
{
    Reg_L++;
    if (Reg_L == 0)
    {
        Reg_H++;
    }
    Reg_C--;
    if (Reg_C == 255)
    {
        Reg_B--;
    }

    if (!(((Reg_B == 0) && (Reg_C == 0)) || (CompareA && (FlagZGet))))
    {
        Reg_PC--;
        Reg_PC--;
    }
}

void ScriptMachineZ180::DoBlock2DR(bool CompareA)
{
    Reg_L--;
    if (Reg_L == 255)
    {
        Reg_H--;
    }
    Reg_C--;
    if (Reg_C == 255)
    {
        Reg_B--;
    }

    if (!(((Reg_B == 0) && (Reg_C == 0)) || (CompareA && (FlagZGet))))
    {
        Reg_PC--;
        Reg_PC--;
    }
}

void ScriptMachineZ180::DoBlock_CP()
{
    bool C = FlagCGet;
    DoCP(MemGet(Reg_H, Reg_L));
    if (C)
    {
        FlagCSet;
    }
    else
    {
        FlagCClr;
    }
}

void ScriptMachineZ180::DoBlock_LD()
{
    MemSet(Reg_D, Reg_E, MemGet(Reg_H, Reg_L));
}
