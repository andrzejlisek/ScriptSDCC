#include "appcore.h"

AppCore::AppCore()
{
    IOConsole_[0] = NEW(IOConsole, IOConsole());
    IOConsole_[1] = NEW(IOConsole, IOConsole());
    IOConsole_[2] = NEW(IOConsole, IOConsole());
    IOConsole_[3] = NEW(IOConsole, IOConsole());
    IOSpreadsheet_[0] = NEW(IOSpreadsheet, IOSpreadsheet());
    IOSpreadsheet_[1] = NEW(IOSpreadsheet, IOSpreadsheet());
    IOSpreadsheet_[2] = NEW(IOSpreadsheet, IOSpreadsheet());
    IOSpreadsheet_[3] = NEW(IOSpreadsheet, IOSpreadsheet());
    IOGraph_[0] = NEW(IOGraph, IOGraph());
    IOGraph_[1] = NEW(IOGraph, IOGraph());
    IOGraph_[2] = NEW(IOGraph, IOGraph());
    IOGraph_[3] = NEW(IOGraph, IOGraph());
}

AppCore::~AppCore()
{
    if (SM != NULL)
    {
        DEL(ScriptMachine, SM);
    }
    DEL(IOConsole, IOConsole_[0]);
    DEL(IOConsole, IOConsole_[1]);
    DEL(IOConsole, IOConsole_[2]);
    DEL(IOConsole, IOConsole_[3]);
    DEL(IOSpreadsheet, IOSpreadsheet_[0]);
    DEL(IOSpreadsheet, IOSpreadsheet_[1]);
    DEL(IOSpreadsheet, IOSpreadsheet_[2]);
    DEL(IOSpreadsheet, IOSpreadsheet_[3]);
    DEL(IOGraph, IOGraph_[0]);
    DEL(IOGraph, IOGraph_[1]);
    DEL(IOGraph, IOGraph_[2]);
    DEL(IOGraph, IOGraph_[3]);
}

void AppCore::ProgRun()
{
    if (SM != NULL)
    {
        SM->ProgWork();
    }
}


string AppCore::ProgCompile(string LibFiles)
{
    CompileGood = false;

    if (SM != NULL)
    {
        DEL(ScriptMachine, SM);
        SM = NULL;
    }

    // Clearing temporary directory
    ClearTemp();

    // Copying library files
    vector<string> LibFilesX = Eden::StringSplit(LibFiles, '|');
    for (uint I = 0; I < LibFilesX.size(); I++)
    {
        if (!FileCopy(LibDir + LibFilesX[I], TempDir))
        {
            FileCopy(LibFilesX[I], TempDir);
        }
    }

    // Copying source code file
    FileCopy(ProgFileSrc, TempDir);


    // Creating system header file
    fstream F0(TempDir + "_.h", ios::out);
    F0 << "#define mem_swap 0x" << Eden::IntToHex16(SwapPage << 8) << endl;
    switch (Engine)
    {
        case 0: F0 << "#define engine_mcs51 " << endl; break;
        case 1: F0 << "#define engine_z180 " << endl; break;
    }
    switch (MemMode)
    {
        case 0: F0 << "#define mem_common " << endl; break;
        case 1: F0 << "#define mem_separated " << endl; break;
    }
    F0.close();

    // Preparing and running compile script file
    // http://qt.apidoc.info/5.2.0/qtcore/qtglobal.html
    string CompileCommandX = CompileCommand;
    Eden::StringReplace(CompileCommandX, "%CODELOC%", Eden::IntToHex8(CodeLoc));
    Eden::StringReplace(CompileCommandX, "%CODESIZE%", Eden::IntToHex8(CodeSize));
    Eden::StringReplace(CompileCommandX, "%DATALOC%", Eden::IntToHex8(DataLoc));
    Eden::StringReplace(CompileCommandX, "%DATASIZE%", Eden::IntToHex8(DataSize));
    #ifdef Q_OS_WIN
        fstream F1(TempDir + "compile.bat", ios::out);
        F1 << TempDir[0] << ":" << endl;
        F1 << "cd " << TempDir << endl;
        F1 << ProgCmd << " " << CompileCommandX << " \"" << GetFileName(ProgFileSrc) << "\"" << " >Log.msg 2>&1" << endl;
        F1.close();
        SysRun(TempDir + "compile.bat");
    #endif
    #ifdef Q_OS_LINUX
        fstream F1(TempDir + "compile", ios::out);
        F1 << "#!/bin/bash" << endl;
        F1 << "cd " << TempDir << endl;
        F1 << ProgCmd << " " << CompileCommandX << " \"" << GetFileName(ProgFileSrc) << "\"" << " >Log.msg 2>&1" << endl;
        F1.close();
        SysRun("chmod +x " + TempDir + "compile");
        SysRun(TempDir + "compile");
    #endif

    // Loading compiled script
    if (Eden::FileExists(TempDir + ProgFileBin))
    {
        switch (Engine)
        {
            case 0: SM = NEW(ScriptMachine, ScriptMachineMCS51()); break;
            case 1: SM = NEW(ScriptMachine, ScriptMachineZ180()); break;
        }
        SM->IOConsole_[0] = IOConsole_[0];
        SM->IOConsole_[1] = IOConsole_[1];
        SM->IOConsole_[2] = IOConsole_[2];
        SM->IOConsole_[3] = IOConsole_[3];
        SM->IOSpreadsheet_[0] = IOSpreadsheet_[0];
        SM->IOSpreadsheet_[1] = IOSpreadsheet_[1];
        SM->IOSpreadsheet_[2] = IOSpreadsheet_[2];
        SM->IOSpreadsheet_[3] = IOSpreadsheet_[3];
        SM->IOGraph_[0] = IOGraph_[0];
        SM->IOGraph_[1] = IOGraph_[1];
        SM->IOGraph_[2] = IOGraph_[2];
        SM->IOGraph_[3] = IOGraph_[3];

        SM->SwapPage = SwapPage << 8;
        SM->PrepareProgram(MemMode, TempDir + ProgFileBin, Engine, CodeLoc, CodeSize, DataLoc, DataSize);

        CompileGood = true;
    }

    // Loading compiler messages
    string CompileMsg = "";
    stringstream CompileMsgX;
    if (Eden::FileExists(TempDir + "Log.msg"))
    {
        fstream F2(TempDir + "Log.msg", ios::in);
        CompileMsgX << F2.rdbuf();
        CompileMsg = CompileMsgX.str();
        F2.close();
    }

    return CompileMsg;
}

string AppCore::GetStatus(llong &CommandCounter)
{
    if (SM != NULL)
    {
        CommandCounter = SM->CommandCounter;
        return SM->GetStatus();
    }
    else
    {
        return "Unknown";
    }
}

void AppCore::ProgReset()
{
    if (SM != NULL)
    {
        SM->ProgReset();
    }
}

void AppCore::ProgAbort()
{
    if (SM != NULL)
    {
        SM->ProgAbort();
    }
}

void AppCore::SysRun(string Cmd)
{
    system(Cmd.c_str());
}

void AppCore::ClearTemp()
{
    #ifdef Q_OS_WIN
        SysRun("del /q " + TempDir + "*.*");
    #endif
    #ifdef Q_OS_LINUX
        SysRun("rm -f " + TempDir + "*");
    #endif
}

string AppCore::GetFileName(string FileName)
{
    string F = "";
    int T = -1;
    for (int I = FileName.length() - 1; I > 0; I--)
    {
        if ((FileName[I] == '/') || (FileName[I] == '\\'))
        {
            T = I + 1;
            while (T < (int)FileName.length())
            {
                F = F + FileName[T];
                T++;
            }
            break;
        }
    }
    return F;
}

bool AppCore::FileCopy(string SrcFile, string DstDir)
{
    if (Eden::FileExists(SrcFile))
    {
        DstDir = Eden::CorrectDir(DstDir);
        string DstFile = GetFileName(SrcFile);
        if (DstFile != "")
        {
            DstFile = DstDir + DstFile;
            ifstream src(SrcFile, ios::binary);
            ofstream dst(DstFile, ios::binary);
            dst << src.rdbuf();
            src.close();
            dst.close();
        }
        return true;
    }
    else
    {
        return false;
    }
}

QImage * AppCore::MemMapRepaint()
{
    int MemBmpW = MemMapH * (256 - MemMapHOffset);
    int MemBmpH = MemMapV * (256 - MemMapVOffset);
    if (MemMap == NULL)
    {
        MemMap = NEW(QImage, QImage(MemBmpW, MemBmpH, QImage::Format_RGB32));
    }
    else
    {
        if ((MemMap->width() != MemBmpW) || (MemMap->height() != MemBmpH))
        {
            MemMap = NEW(QImage, QImage(MemBmpW, MemBmpH, QImage::Format_RGB32));
        }
    }
    uchar * MemMapRaw = MemMap->bits();
    int P = (MemMapVOffset << 8) + MemMapHOffset;
    int PX = MemMapVOffset;
    int P4 = 0;
    int VisW = 256 - MemMapHOffset;
    int VisH = 256 - MemMapVOffset;
    if (SM != NULL)
    {
        bool InCodeRange;
        bool InDataRange;
        if (SM->MemMode == 0)
        {
            for (int Y = 0; Y < VisH; Y++)
            {
                InCodeRange = (Y >= (SM->CodeLoc - MemMapVOffset)) && (Y < (SM->CodeLoc + SM->CodeSize - MemMapVOffset));
                InDataRange = (Y >= (SM->DataLoc - MemMapVOffset)) && (Y < (SM->DataLoc + SM->DataSize - MemMapVOffset));
                for (int YY = 0; YY < MemMapV; YY++)
                {
                    for (int X = 0; X < VisW; X++)
                    {
                        for (int XX = 0; XX < MemMapH; XX++)
                        {
                            if (MemMapData)
                            {
                                if (PX == SwapPage)
                                {
                                    MemMapRaw[P4 + 0] = 128;
                                    MemMapRaw[P4 + 1] = SM->MemMapR[P] ? 255 : 128;
                                    MemMapRaw[P4 + 2] = SM->MemMapW[P] ? 255 : 128;
                                }
                                else
                                {
                                    MemMapRaw[P4 + 0] = SM->MemMapP[P] ? 255 : (InCodeRange ? 64 : 0);
                                    MemMapRaw[P4 + 1] = SM->MemMapR[P] ? 255 : (InDataRange ? 64 : 0);
                                    MemMapRaw[P4 + 2] = SM->MemMapW[P] ? 255 : (InDataRange ? 64 : 0);
                                }
                            }
                            else
                            {
                                if (PX == SwapPage)
                                {
                                    MemMapRaw[P4 + 0] = SM->MemMapP[P] ? 255 : 128;
                                    MemMapRaw[P4 + 1] = SM->MemMapC[P] ? 255 : 128;
                                    MemMapRaw[P4 + 2] = 128;
                                }
                                else
                                {
                                    MemMapRaw[P4 + 0] = SM->MemMapP[P] ? 255 : (InCodeRange ? 64 : 0);
                                    MemMapRaw[P4 + 1] = SM->MemMapC[P] ? 255 : (InDataRange ? 64 : 0);
                                    MemMapRaw[P4 + 2] = (InDataRange ? 64 : 0);
                                }
                            }
                            MemMapRaw[P4 + 3] = 0;
                            P4 += 4;
                        }
                        P++;
                    }
                    P -= VisW;
                }
                P += 256;
                PX++;
            }
        }
        else
        {
            for (int Y = 0; Y < VisH; Y++)
            {
                InCodeRange = (Y >= (SM->CodeLoc - MemMapVOffset)) && (Y < (SM->CodeLoc + SM->CodeSize - MemMapVOffset));
                InDataRange = (Y >= (SM->DataLoc - MemMapVOffset)) && (Y < (SM->DataLoc + SM->DataSize - MemMapVOffset));
                for (int YY = 0; YY < MemMapV; YY++)
                {
                    for (int X = 0; X < VisW; X++)
                    {
                        for (int XX = 0; XX < MemMapH; XX++)
                        {
                            if (MemMapData)
                            {
                                if (PX == SwapPage)
                                {
                                    MemMapRaw[P4 + 0] = 128;
                                    MemMapRaw[P4 + 1] = SM->MemMapR[P] ? 255 : 128;
                                    MemMapRaw[P4 + 2] = SM->MemMapW[P] ? 255 : 128;
                                }
                                else
                                {
                                    MemMapRaw[P4 + 0] = 0;
                                    MemMapRaw[P4 + 1] = SM->MemMapR[P] ? 255 : (InDataRange ? 64 : 0);
                                    MemMapRaw[P4 + 2] = SM->MemMapW[P] ? 255 : (InDataRange ? 64 : 0);
                                }
                            }
                            else
                            {
                                MemMapRaw[P4 + 0] = SM->MemMapP[P] ? 255 : (InCodeRange ? 64 : 0);
                                MemMapRaw[P4 + 1] = SM->MemMapC[P] ? 255 : 0;
                                MemMapRaw[P4 + 2] = 0;
                            }
                            MemMapRaw[P4 + 3] = 0;
                            P4 += 4;
                        }
                        P++;
                    }
                    P -= VisW;
                }
                P += 256;
                PX++;
            }
        }
    }
    else
    {
        for (int I = ((MemBmpW * MemBmpH) - 1); I >= 0; I--)
        {
            MemMapRaw[P4 + 0] = 0;
            MemMapRaw[P4 + 1] = 0;
            MemMapRaw[P4 + 2] = 0;
            MemMapRaw[P4 + 3] = 0;
            P4 += 4;
        }
    }
    return MemMap;
}
