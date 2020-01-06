#include "scriptmachine.h"

ScriptMachine::ScriptMachine()
{
    AbortRequest = false;
}

ScriptMachine::~ScriptMachine()
{
    DELARR(uchar, MemData);
    if ((MemMode != 0) && (MemMode != 2))
    {
        DELARR(uchar, MemProg);
    }
}

void ScriptMachine::PrepareMem(int Mode)
{
    MemData = NEWARR(uchar, uchar[65536]);
    MemMode = Mode;
    if (MemMode == 0)
    {
        MemProg = MemData;
    }
    else
    {
        MemProg = NEWARR(uchar, uchar[65536]);
    }
}

void ScriptMachine::LoadProgram(int Mode_, string &ProgCode, int CodeLoc_, int CodeSize_, int DataLoc_, int DataSize_)
{
    CodeLoc = CodeLoc_;
    CodeSize = CodeSize_;
    DataLoc  = DataLoc_;
    DataSize = DataSize_;

    // Preparing memory
    PrepareMem(Mode_);

    // Clearing memory
    int I;
    for (I = 0; I < 65536; I++)
    {
        MemProg[I] = 0;
        MemData[I] = 0;
        MemMapP[I] = 0;
        MemMapR[I] = 0;
        MemMapW[I] = 0;
        MemMapC[I] = 0;
    }

    // Loading compiled code
    int ProgCodeI = 0;
    int ProgCodeL = ProgCode.size();
    for (ProgCodeI = 0; ProgCodeI <= ProgCodeL; ProgCodeI++)
    {
        if (ProgCode[ProgCodeI] == ':')
        {
            // Get binary code line
            int ProgCodeII = ProgCodeI + 1;
            while ((ProgCodeII < ProgCodeL) && (ProgCode[ProgCodeII] != ':'))
            {
                ProgCodeII++;
            }
            string Buf = ProgCode.substr(ProgCodeI, ProgCodeII - ProgCodeI);

            if (Buf.size() > 11)
            {
                if (Buf.substr(7, 2) == "00")
                {
                    // Number of bytes
                    int DataLength = Eden::HexToInt(Buf.substr(1, 2));

                    // First byte address
                    int DataAddr = Eden::HexToInt(Buf.substr(3, 4));

                    // Writing bytes into CODE memory
                    for (I = 0; I < DataLength; I++)
                    {
                        MemMapP[DataAddr + I] = 1;
                        MemProg[DataAddr + I] = Eden::HexToInt(Buf.substr(I * 2 + 9, 2));
                    }
                }
            }
        }
    }


    // Saving memory dump to file
    /*cout << ">>>>" << FileName << endl;
    fstream FXXX(FileName + ".bin", ios::out | ios::binary);
    FXXX.write((char*)MemProg, 65536);
    FXXX.close();*/

    // Resetting
    ProgReset();
}

void ScriptMachine::MemSet(ushort Addr, uchar Val)
{
    MemMapW[Addr] = 1;
    MemData[Addr] = Val;
}

void ScriptMachine::MemSet(ushort AddrH, ushort AddrL, uchar Val)
{
    MemMapW[(AddrH << 8) + AddrL] = 1;
    MemData[(AddrH << 8) + AddrL] = Val;
}

uchar ScriptMachine::MemPrg(ushort Addr)
{
    MemMapC[Addr] = 1;
    return MemProg[Addr];
}

uchar ScriptMachine::MemGet(ushort Addr)
{
    MemMapR[Addr] = 1;
    return MemData[Addr];
}

uchar ScriptMachine::MemGet(ushort AddrH, ushort AddrL)
{
    MemMapR[(AddrH << 8) + AddrL] = 1;
    return MemData[(AddrH << 8) + AddrL];
}

bool ScriptMachine::ProgWorking()
{
    if (MemData[SwapPage] != 0)
    {
        IOBuffer.Raw[0] = MemData[SwapPage + 0x08];
        IOBuffer.Raw[1] = MemData[SwapPage + 0x09];
        IOBuffer.Raw[2] = MemData[SwapPage + 0x0A];
        IOBuffer.Raw[3] = MemData[SwapPage + 0x0B];
        IOBuffer.Raw[4] = MemData[SwapPage + 0x0C];
        IOBuffer.Raw[5] = MemData[SwapPage + 0x0D];
        IOBuffer.Raw[6] = MemData[SwapPage + 0x0E];
        IOBuffer.Raw[7] = MemData[SwapPage + 0x0F];
        if (MemData[SwapPage] < 200)
        {
            int IONum = MemData[SwapPage] & b00000011;
            switch(MemData[SwapPage] & b11111100)
            {
            default:
                ErrorStatus = "Unsupported command " + Eden::ToStr(MemData[SwapPage]);
                MemData[SwapPage] = 255;
                StatusC = 5;
                return false;
            case IOConsoleCmd + 0:
                IOConsole_[IONum]->Clear();
                break;
            case IOConsoleCmd + 4:
                {
                    uchar CmdX = MemData[SwapPage + 1];
                    switch(CmdX)
                    {
                        case 0: IOConsole_[IONum]->Print((int)IOBuffer.BufS1); break;
                        case 1: IOConsole_[IONum]->Print((int)IOBuffer.BufU1); break;
                        case 2: IOConsole_[IONum]->Print(IOBuffer.BufS1); break;
                        case 3: IOConsole_[IONum]->Print(IOBuffer.BufU1); break;
                        case 4: IOConsole_[IONum]->Print(IOBuffer.BufS2); break;
                        case 5: IOConsole_[IONum]->Print(IOBuffer.BufU2); break;
                        case 6: IOConsole_[IONum]->Print(IOBuffer.BufS2); break;
                        case 7: IOConsole_[IONum]->Print(IOBuffer.BufU2); break;
                        case 8: IOConsole_[IONum]->Print(IOBuffer.BufS4); break;
                        case 9: IOConsole_[IONum]->Print(IOBuffer.BufU4); break;
                        case 10: IOConsole_[IONum]->Print(IOBuffer.BufF); break;
                    }
                }
                break;
            case IOConsoleCmd + 8:
                if ((IOBuffer.BufS1 <= 120) && (IOBuffer.BufS1 >= -120))
                {
                    if (IOBuffer.BufS1 >= 0)
                    {
                        IOConsole_[IONum]->SetFloadFixed(IOBuffer.BufS1);
                    }
                    else
                    {
                        IOConsole_[IONum]->SetFloatScientific(0 - IOBuffer.BufS1);
                    }
                }
                break;

            case IOConsoleCmd + 12:
                IOBuffer.BufU1 = IOConsole_[IONum]->InputCharGet();
                break;
            case IOConsoleCmd + 16:
                IOBuffer.BufS1 = IOConsole_[IONum]->InputCharGet();
                break;
            case IOConsoleCmd + 20:
                IOConsole_[IONum]->InputClear();
                break;

            case IOSpreadsheetCmd + 0:
                {
                    uchar CmdX = MemData[SwapPage + 1];
                    string Raw = IOSpreadsheet_[IONum]->CellGet(MemData[SwapPage + 2], MemData[SwapPage + 3]);
                    switch(CmdX)
                    {
                        case 0: IOBuffer.BufS1 = Eden::ToLong(Raw); break;
                        case 1: IOBuffer.BufU1 = Eden::ToLong(Raw); break;
                        case 2: if (Raw.length() > 0) { IOBuffer.BufS1 = Raw[0]; } else { IOBuffer.BufS1 = 0; } break;
                        case 3: if (Raw.length() > 0) { IOBuffer.BufU1 = Raw[0]; } else { IOBuffer.BufU1 = 0; } break;
                        case 4: IOBuffer.BufS2 = Eden::ToLong(Raw); break;
                        case 5: IOBuffer.BufU2 = Eden::ToLong(Raw); break;
                        case 6: IOBuffer.BufS2 = Eden::ToLong(Raw); break;
                        case 7: IOBuffer.BufU2 = Eden::ToLong(Raw); break;
                        case 8: IOBuffer.BufS4 = Eden::ToLong(Raw); break;
                        case 9: IOBuffer.BufU4 = Eden::ToLong(Raw); break;
                        case 10: IOBuffer.BufF = Eden::ToFloat(Raw); break;
                    }
                }
                break;
            case IOSpreadsheetCmd + 4:
                TextBuff.str(IOSpreadsheet_[IONum]->CellGet(MemData[SwapPage + 1], MemData[SwapPage + 2]));
                break;
            case IOSpreadsheetCmd + 8:
                {
                    uchar CmdX = MemData[SwapPage + 1];
                    string Raw = "";
                    switch(CmdX)
                    {
                        case 0: Raw = Eden::ToStr((int)IOBuffer.BufS1); break;
                        case 1: Raw = Eden::ToStr((int)IOBuffer.BufU1); break;
                        case 2: Raw = IOBuffer.BufS1; break;
                        case 3: Raw = IOBuffer.BufU1; break;
                        case 4: Raw = Eden::ToStr(IOBuffer.BufS2); break;
                        case 5: Raw = Eden::ToStr(IOBuffer.BufU2); break;
                        case 6: Raw = Eden::ToStr(IOBuffer.BufS2); break;
                        case 7: Raw = Eden::ToStr(IOBuffer.BufU2); break;
                        case 8: Raw = Eden::ToStr(IOBuffer.BufS4); break;
                        case 9: Raw = Eden::ToStr(IOBuffer.BufU4); break;
                        case 10: Raw = Eden::ToStr(IOBuffer.BufF); break;
                    }
                    IOSpreadsheet_[IONum]->CellSet(MemData[SwapPage + 2], MemData[SwapPage + 3], Raw);
                }
                break;
            case IOSpreadsheetCmd + 12:
                IOSpreadsheet_[IONum]->CellSet(MemData[SwapPage + 1], MemData[SwapPage + 2], TextBuff.str());
                break;
            case IOSpreadsheetCmd + 16:
                {
                    uchar CmdX = MemData[SwapPage + 1];
                    uchar R1 = MemData[SwapPage + 2];
                    uchar C1 = MemData[SwapPage + 3];
                    uchar R2 = MemData[SwapPage + 4];
                    uchar C2 = MemData[SwapPage + 5];
                    if ((R1 <= R2) && (C1 <= C2))
                    {
                        switch(CmdX)
                        {
                            case 0: IOSpreadsheet_[IONum]->SheetClear(R1, C1, R2, C2); break;
                            case 1: IOSpreadsheet_[IONum]->SheetRowAdd(R1, C1, R2, C2); break;
                            case 2: IOSpreadsheet_[IONum]->SheetRowRem(R1, C1, R2, C2); break;
                            case 3: IOSpreadsheet_[IONum]->SheetColAdd(R1, C1, R2, C2); break;
                            case 4: IOSpreadsheet_[IONum]->SheetColRem(R1, C1, R2, C2); break;
                        }
                    }
                }
                break;

            case IOGraphCmd + 0:
                IOGraph_[IONum]->Clear();
                break;
            case IOGraphCmd + 4:
                IOGraph_[IONum]->ThreadSet(MemData[SwapPage + 1]);
                break;
            case IOGraphCmd + 8:
                IOGraph_[IONum]->ThreadReset();
                break;
            case IOGraphCmd + 12:
                {
                    uchar CmdX = MemData[SwapPage + 1];
                    double ValX = 0;
                    double ValY = 0;
                    double ValZ = 0;
                    switch(CmdX)
                    {
                        case 0: ValX = ValGetSC(0x10); ValY = ValGetSC(0x18); ValZ = ValGetSC(0x20); break;
                        case 1: ValX = ValGetUC(0x10); ValY = ValGetUC(0x18); ValZ = ValGetUC(0x20); break;
                        case 4: ValX = ValGetSS(0x10); ValY = ValGetSS(0x18); ValZ = ValGetSS(0x20); break;
                        case 5: ValX = ValGetUS(0x10); ValY = ValGetUS(0x18); ValZ = ValGetUS(0x20); break;
                        case 6: ValX = ValGetSI(0x10); ValY = ValGetSI(0x18); ValZ = ValGetSI(0x20); break;
                        case 7: ValX = ValGetUI(0x10); ValY = ValGetUI(0x18); ValZ = ValGetUI(0x20); break;
                        case 8: ValX = ValGetSL(0x10); ValY = ValGetSL(0x18); ValZ = ValGetSL(0x20); break;
                        case 9: ValX = ValGetUL(0x10); ValY = ValGetUL(0x18); ValZ = ValGetUL(0x20); break;
                        case 10: ValX = ValGetF(0x10); ValY = ValGetF(0x18); ValZ = ValGetF(0x20); break;
                    }
                    IOGraph_[IONum]->Plot(ValX, ValY, ValZ, MemData[SwapPage + 2], MemData[SwapPage + 3], MemData[SwapPage + 4]);
                }
                break;
            case IOGraphCmd + 16:
                if (MemData[SwapPage + 1] == 0)
                {
                    IOGraph_[IONum]->Triangle();
                }
                else
                {
                    IOGraph_[IONum]->Line(MemData[SwapPage + 1]);
                }
                break;

            case IOGraphTextCmd + 0:
                {
                    IOGraph_[IONum]->TextBack = ((MemData[SwapPage + 1] & 2) > 0) ? true : false;
                    IOGraph_[IONum]->TextFore = ((MemData[SwapPage + 1] & 1) > 0) ? true : false;
                    IOGraph_[IONum]->TextColorR0 = MemData[SwapPage + 2];
                    IOGraph_[IONum]->TextColorG0 = MemData[SwapPage + 3];
                    IOGraph_[IONum]->TextColorB0 = MemData[SwapPage + 4];
                    IOGraph_[IONum]->TextColorR = MemData[SwapPage + 5];
                    IOGraph_[IONum]->TextColorG = MemData[SwapPage + 6];
                    IOGraph_[IONum]->TextColorB = MemData[SwapPage + 7];
                }
                break;
            case IOGraphTextCmd + 4:
                {
                    uchar CmdX = MemData[SwapPage + 1];
                    double ValX = 0;
                    double ValY = 0;
                    double ValZ = 0;
                    switch(CmdX)
                    {
                        case 0: ValX = ValGetSC(0x10); ValY = ValGetSC(0x18); ValZ = ValGetSC(0x20); break;
                        case 1: ValX = ValGetUC(0x10); ValY = ValGetUC(0x18); ValZ = ValGetUC(0x20); break;
                        case 4: ValX = ValGetSS(0x10); ValY = ValGetSS(0x18); ValZ = ValGetSS(0x20); break;
                        case 5: ValX = ValGetUS(0x10); ValY = ValGetUS(0x18); ValZ = ValGetUS(0x20); break;
                        case 6: ValX = ValGetSI(0x10); ValY = ValGetSI(0x18); ValZ = ValGetSI(0x20); break;
                        case 7: ValX = ValGetUI(0x10); ValY = ValGetUI(0x18); ValZ = ValGetUI(0x20); break;
                        case 8: ValX = ValGetSL(0x10); ValY = ValGetSL(0x18); ValZ = ValGetSL(0x20); break;
                        case 9: ValX = ValGetUL(0x10); ValY = ValGetUL(0x18); ValZ = ValGetUL(0x20); break;
                        case 10: ValX = ValGetF(0x10); ValY = ValGetF(0x18); ValZ = ValGetF(0x20); break;
                    }
                    int X0 = ValGetSS(0x28);
                    int Y0 = ValGetSS(0x30);
                    IOGraph_[IONum]->TextSet(ValX, ValY, ValZ, X0, Y0);
                }
                break;
            case IOGraphTextCmd + 8:
                {
                    uchar CmdX = MemData[SwapPage + 1];
                    string Raw = "";
                    switch(CmdX)
                    {
                        case 0: Raw = Eden::ToStr((int)IOBuffer.BufS1); break;
                        case 1: Raw = Eden::ToStr((int)IOBuffer.BufU1); break;
                        case 2: Raw = IOBuffer.BufS1; break;
                        case 3: Raw = IOBuffer.BufU1; break;
                        case 4: Raw = Eden::ToStr(IOBuffer.BufS2); break;
                        case 5: Raw = Eden::ToStr(IOBuffer.BufU2); break;
                        case 6: Raw = Eden::ToStr(IOBuffer.BufS2); break;
                        case 7: Raw = Eden::ToStr(IOBuffer.BufU2); break;
                        case 8: Raw = Eden::ToStr(IOBuffer.BufS4); break;
                        case 9: Raw = Eden::ToStr(IOBuffer.BufU4); break;
                        case 10: Raw = Eden::ToStr(IOBuffer.BufF); break;
                    }
                    IOGraph_[IONum]->TextPrint(Raw);
                }
                break;
            case IOGraphTextCmd + 12:
                {
                    IOGraph_[IONum]->TextPrint(TextBuff.str());
                }
                break;
            }
        }
        else
        {
            switch (MemData[SwapPage])
            {
                default:
                    ErrorStatus = "Unsupported command " + Eden::ToStr(MemData[SwapPage]);
                    MemData[SwapPage] = 255;
                    StatusC = 5;
                    return false;

                case 236:
                    {
                        MemData[SwapPage + 1] = FileHandle_->FileOpen(MemData[SwapPage + 1], TextBuff.str(), MemData[SwapPage + 2]);
                    }
                    break;
                case 237:
                    {
                        MemData[SwapPage + 1] = FileHandle_->FileClose(MemData[SwapPage + 1]);
                    }
                    break;
                case 238:
                    {
                        IOBuffer.BufU4 = FileHandle_->FileSize((MemData[SwapPage + 1]));
                    }
                    break;
                case 239:
                    {
                        MemData[SwapPage + 1] = FileHandle_->FileSetPos(MemData[SwapPage + 1], IOBuffer.BufU4);
                    }
                    break;
                case 240:
                    {
                        IOBuffer.BufU4 = FileHandle_->FileGetPos((MemData[SwapPage + 1]));
                    }
                    break;
                case 241:
                    {
                        uint Addr = (((uint)(MemData[SwapPage + 3])) << 8) + ((uint)(MemData[SwapPage + 4]));
                        uint Size = (((uint)(MemData[SwapPage + 5])) << 8) + ((uint)(MemData[SwapPage + 6]));
                        MemBuffer_->PrepareBuf(MemData[SwapPage + 2]);
                        MemData[SwapPage + 1] = FileHandle_->DataGet(MemData[SwapPage + 1], MemBuffer_->BufRaw[MemData[SwapPage + 2]], MemBuffer_->BufOpW[MemData[SwapPage + 2]], Addr, Size);
                    }
                    break;
                case 242:
                    {
                        uint Addr = (((uint)(MemData[SwapPage + 3])) << 8) + ((uint)(MemData[SwapPage + 4]));
                        uint Size = (((uint)(MemData[SwapPage + 5])) << 8) + ((uint)(MemData[SwapPage + 6]));
                        MemBuffer_->PrepareBuf(MemData[SwapPage + 2]);
                        MemData[SwapPage + 1] = FileHandle_->DataPut(MemData[SwapPage + 1], MemBuffer_->BufRaw[MemData[SwapPage + 2]], MemBuffer_->BufOpR[MemData[SwapPage + 2]], Addr, Size);
                    }
                    break;

                case 243:
                    {
                        uchar BufN = MemData[SwapPage + 1];
                        MemBuffer_->Clear(BufN);
                    }
                    break;
                case 244:
                    {
                        uchar BufN = MemData[SwapPage + 1];
                        int BufAddr = ValGetUS(0x10);
                        int ProgAddr = ValGetUS(0x18);
                        int Size = ValGetUS(0x20);
                        if (MemBuffer_->Get(BufN, BufAddr, ProgAddr, Size, MemData, MemMapW))
                        {
                            MemData[SwapPage + 1] = 1;
                        }
                        else
                        {
                            MemData[SwapPage + 1] = 0;
                        }
                    }
                    break;
                case 245:
                    {
                        uchar BufN = MemData[SwapPage + 1];
                        int BufAddr = ValGetUS(0x10);
                        int ProgAddr = ValGetUS(0x18);
                        int Size = ValGetUS(0x20);
                        if (MemBuffer_->Set(BufN, BufAddr, ProgAddr, Size, MemData, MemMapR))
                        {
                            MemData[SwapPage + 1] = 1;
                        }
                        else
                        {
                            MemData[SwapPage + 1] = 0;
                        }
                    }
                    break;

                case 246:
                    {
                        uchar CmdX = MemData[SwapPage + 1];
                        CoreInvokeWait = 255;
                        emit CoreInvoke(BundleIndex, 0, CmdX);
                        while (CoreInvokeWait == 255)
                        {
                        }
                        MemData[SwapPage + 1] = CoreInvokeWait;
                    }
                    break;
                case 247:
                    {
                        uchar CmdX = MemData[SwapPage + 1];
                        CoreInvokeWait = 255;
                        emit CoreInvoke(BundleIndex, 1, CmdX);
                        while (CoreInvokeWait == 255)
                        {
                        }
                        MemData[SwapPage + 1] = CoreInvokeWait;
                    }
                    break;

                case 248:
                    TextBuffS = TextBuff.str();
                    TextBuffI = 0;
                    IOBuffer.BufU2 = Min(TextBuffS.size(), 65535);
                    break;
                case 249:
                    if (TextBuffI < TextBuffS.size())
                    {
                        IOBuffer.BufS1 = TextBuffS[TextBuffI];
                        TextBuffI++;
                    }
                    else
                    {
                        IOBuffer.BufS1 = 0;
                    }
                    break;
                case 250:
                    TextBuff.str(string());
                    break;
                case 251:
                    TextBuff << IOBuffer.BufS1;
                    break;
                case 252:
                    {
                        llong AllM_ = CommandCounter;
                        llong AllP_ = AllM_ - CommandCounterX;
                        CommandCounterX = CommandCounter;
                        uchar Div_ = IOBuffer.BufU1;
                        while (Div_ > 0)
                        {
                            AllP_ = AllP_ / 10;
                            Div_--;
                        }
                        IOBuffer.BufU4 = AllP_;
                    }
                    break;
                case 253:
                    StatusC = 4;
                    return false;
                case 254:
                    StatusC = 3;
                    MemData[SwapPage] = 0;
                    return false;
                case 255:
                    StatusC = 5;
                    return false;
            }
        }
        MemData[SwapPage + 0x08] = IOBuffer.Raw[0];
        MemData[SwapPage + 0x09] = IOBuffer.Raw[1];
        MemData[SwapPage + 0x0A] = IOBuffer.Raw[2];
        MemData[SwapPage + 0x0B] = IOBuffer.Raw[3];
        MemData[SwapPage + 0x0C] = IOBuffer.Raw[4];
        MemData[SwapPage + 0x0D] = IOBuffer.Raw[5];
        MemData[SwapPage + 0x0E] = IOBuffer.Raw[6];
        MemData[SwapPage + 0x0F] = IOBuffer.Raw[7];
        MemData[SwapPage] = 0;
    }
    if (AbortRequest)
    {
        StatusC = 2;
        return false;
    }
    else
    {
        return true;
    }
}

void ScriptMachine::ProgWork()
{
    StatusC = 1;
    AbortRequest = false;
    while (ProgWorking())
    {
        DoCommand();
        CommandCounter++;
    }
}

void ScriptMachine::ProgAbort()
{
    AbortRequest = true;
}

void ScriptMachine::ProgReset()
{
    StatusC = 0;
    CommandCounter = 0;
    CommandCounterX = 0;
    MemData[SwapPage +  0] = 0;
    MemData[SwapPage +  1] = 0;
    MemData[SwapPage +  2] = 0;
    MemData[SwapPage +  3] = 0;
    MemData[SwapPage +  4] = 0;
    MemData[SwapPage +  5] = 0;
    MemData[SwapPage +  6] = 0;
    MemData[SwapPage +  7] = 0;
    MemData[SwapPage +  8] = 0;
    MemData[SwapPage +  9] = 0;
    MemData[SwapPage + 10] = 0;
    MemData[SwapPage + 11] = 0;
    MemData[SwapPage + 12] = 0;
    MemData[SwapPage + 13] = 0;
    MemData[SwapPage + 14] = 0;
    MemData[SwapPage + 15] = 0;
    DoReset();
}

char ScriptMachine::ValGetSC(int Addr)
{
    IOBuffer.Raw[0] = MemData[SwapPage + Addr + 0];
    return IOBuffer.BufS1;
}

uchar ScriptMachine::ValGetUC(int Addr)
{
    IOBuffer.Raw[0] = MemData[SwapPage + Addr + 0];
    return IOBuffer.BufU1;
}

short ScriptMachine::ValGetSS(int Addr)
{
    IOBuffer.Raw[0] = MemData[SwapPage + Addr + 0];
    IOBuffer.Raw[1] = MemData[SwapPage + Addr + 1];
    return IOBuffer.BufS2;
}

ushort ScriptMachine::ValGetUS(int Addr)
{
    IOBuffer.Raw[0] = MemData[SwapPage + Addr + 0];
    IOBuffer.Raw[1] = MemData[SwapPage + Addr + 1];
    return IOBuffer.BufU2;
}

int ScriptMachine::ValGetSI(int Addr)
{
    IOBuffer.Raw[0] = MemData[SwapPage + Addr + 0];
    IOBuffer.Raw[1] = MemData[SwapPage + Addr + 1];
    //IOBuffer.Raw[2] = MemData[SwapPage + Addr + 2];
    //IOBuffer.Raw[3] = MemData[SwapPage + Addr + 3];
    return IOBuffer.BufS2;
}

uint ScriptMachine::ValGetUI(int Addr)
{
    IOBuffer.Raw[0] = MemData[SwapPage + Addr + 0];
    IOBuffer.Raw[1] = MemData[SwapPage + Addr + 1];
    //IOBuffer.Raw[2] = MemData[SwapPage + Addr + 2];
    //IOBuffer.Raw[3] = MemData[SwapPage + Addr + 3];
    return IOBuffer.BufU2;
}

long ScriptMachine::ValGetSL(int Addr)
{
    IOBuffer.Raw[0] = MemData[SwapPage + Addr + 0];
    IOBuffer.Raw[1] = MemData[SwapPage + Addr + 1];
    IOBuffer.Raw[2] = MemData[SwapPage + Addr + 2];
    IOBuffer.Raw[3] = MemData[SwapPage + Addr + 3];
    IOBuffer.Raw[4] = MemData[SwapPage + Addr + 4];
    IOBuffer.Raw[5] = MemData[SwapPage + Addr + 5];
    IOBuffer.Raw[6] = MemData[SwapPage + Addr + 6];
    IOBuffer.Raw[7] = MemData[SwapPage + Addr + 7];
    return IOBuffer.BufS4;
}

ulong ScriptMachine::ValGetUL(int Addr)
{
    IOBuffer.Raw[0] = MemData[SwapPage + Addr + 0];
    IOBuffer.Raw[1] = MemData[SwapPage + Addr + 1];
    IOBuffer.Raw[2] = MemData[SwapPage + Addr + 2];
    IOBuffer.Raw[3] = MemData[SwapPage + Addr + 3];
    IOBuffer.Raw[4] = MemData[SwapPage + Addr + 4];
    IOBuffer.Raw[5] = MemData[SwapPage + Addr + 5];
    IOBuffer.Raw[6] = MemData[SwapPage + Addr + 6];
    IOBuffer.Raw[7] = MemData[SwapPage + Addr + 7];
    return IOBuffer.BufU4;
}

double ScriptMachine::ValGetF(int Addr)
{
    IOBuffer.Raw[0] = MemData[SwapPage + Addr + 0];
    IOBuffer.Raw[1] = MemData[SwapPage + Addr + 1];
    IOBuffer.Raw[2] = MemData[SwapPage + Addr + 2];
    IOBuffer.Raw[3] = MemData[SwapPage + Addr + 3];
    IOBuffer.Raw[4] = MemData[SwapPage + Addr + 4];
    IOBuffer.Raw[5] = MemData[SwapPage + Addr + 5];
    IOBuffer.Raw[6] = MemData[SwapPage + Addr + 6];
    IOBuffer.Raw[7] = MemData[SwapPage + Addr + 7];
    return IOBuffer.BufF;
}


void ScriptMachine::ValPut(int Addr, double Val)
{
    IOBuffer.BufF = Val;
    MemData[SwapPage + Addr + 0] = IOBuffer.Raw[0];
    MemData[SwapPage + Addr + 1] = IOBuffer.Raw[1];
    MemData[SwapPage + Addr + 2] = IOBuffer.Raw[2];
    MemData[SwapPage + Addr + 3] = IOBuffer.Raw[3];
    MemData[SwapPage + Addr + 4] = IOBuffer.Raw[4];
    MemData[SwapPage + Addr + 5] = IOBuffer.Raw[5];
    MemData[SwapPage + Addr + 6] = IOBuffer.Raw[6];
    MemData[SwapPage + Addr + 7] = IOBuffer.Raw[7];
}

string ScriptMachine::GetStatus()
{
    switch (StatusC)
    {
        case 0: return "Reset";
        case 1: return "Running";
        case 2: return "Stopped by user";
        case 3: return "Stopped by command";
        case 4: return "End";
        case 5: return "Halted: " + ErrorStatus;
        default: return "Unknown or not created";
    }
}
