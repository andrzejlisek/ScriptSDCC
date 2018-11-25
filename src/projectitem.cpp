#include "projectitem.h"

ProjectItem::ProjectItem()
{
    SM = NULL;
    Init();
}

void ProjectItem::Init()
{
    BundleFile = "";
    BundleDesc = "";
    Descr = "";

    if (SM != NULL)
    {
        DEL(ScriptMachine, SM);
    }
    SM = NULL;
    Engine = 0;
    MemMode = 0;
    CompileCommand = "";

    LibFiles = "";
    ProgFileSrc = "";
    ProgFileBin = "";
    ViewCmd = "";

    BtnCompile = true;
    BtnReset = false;
    BtnRun = false;
    BtnAbort = false;

    SwapPage = 0;
    CodeLoc_ = 0;
    CodeSize = 0;
    DataLoc_ = 0;
    DataSize = 0;

    CompileMsg = "";
}

ProjectItem::~ProjectItem()
{
    if (SM != NULL)
    {
        DEL(ScriptMachine, SM);
    }
}

void ProjectItem::ProgRun()
{
    if (SM != NULL)
    {
        SM->ProgWork();
    }
}

void ProjectItem::ProgReset()
{
    if (SM != NULL)
    {
        SM->ProgReset();
    }
}

void ProjectItem::ProgAbort()
{
    if (SM != NULL)
    {
        SM->ProgAbort();
    }
}

void ProjectItem::ProgCompile()
{
    CompileMsg = "";
    CompileGood = false;

    if (SM != NULL)
    {
        DEL(ScriptMachine, SM);
        SM = NULL;
    }

    // Creating source code directory
    string ProgDir = GetFilePath(ProgFileSrc);
    string ProjDir = GetFilePath(BundleFile);

    // Clearing temporary directory
    ClearTemp();

    // Copying source and library files
    vector<string> LibFilesX = Eden::StringSplit(LibFiles, '|');
    LibFilesX.push_back(ProgFileSrc);
    for (uint I = 0; I < LibFilesX.size(); I++)
    {
        bool CopyOK = false;

        string FilePath = GetFilePath(LibFilesX[I]);

        if (FilePath != "")
        {
            CopyOK = FileCopy(LibFilesX[I], TempDir);
        }
        else
        {
            if ((!CopyOK) && (ProjDir != ""))
            {
                CopyOK = FileCopy(ProjDir + LibFilesX[I], TempDir);
            }

            if ((!CopyOK) && (ProgDir != ""))
            {
                CopyOK = FileCopy(ProgDir + LibFilesX[I], TempDir);
            }

            if ((!CopyOK) && (LibDir != ""))
            {
                CopyOK = FileCopy(LibDir + LibFilesX[I], TempDir);
            }
        }

        if (!CopyOK)
        {
            CompileMsg = CompileMsg + "File \"" + LibFilesX[I] + "\" not found" + Eden::EOL();
        }
    }

    if (CompileMsg != "")
    {
        return;
    }

    // Creating system header file
    fstream F0(TempDir + "_.h", ios::out);
    F0 << "#define mem_swap 0x" << Eden::IntToHex16(SwapPage << 8) << endl;
    switch (Engine)
    {
        case 0: F0 << "#define engine_mcs51" << endl; break;
        case 1: F0 << "#define engine_z180" << endl; break;
    }
    switch (MemMode)
    {
        case 0: F0 << "#define mem_common" << endl; break;
        case 1: F0 << "#define mem_separated" << endl; break;
    }
    F0.close();

    // Preparing and running compile script file
    // http://qt.apidoc.info/5.2.0/qtcore/qtglobal.html
    string CompileCommandX = CompileCommand;
    Eden::StringReplace(CompileCommandX, "%CODELOC%", Eden::IntToHex8(CodeLoc_));
    Eden::StringReplace(CompileCommandX, "%CODESIZE%", Eden::IntToHex8(CodeSize));
    Eden::StringReplace(CompileCommandX, "%DATALOC%", Eden::IntToHex8(DataLoc_));
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
        CompileGood = true;
    }

    // Loading compiler messages
    CompileMsg = "";
    stringstream CompileMsgX;
    if (Eden::FileExists(TempDir + "Log.msg"))
    {
        fstream F2(TempDir + "Log.msg", ios::in);
        CompileMsgX << F2.rdbuf();
        CompileMsg = CompileMsgX.str();
        F2.close();
    }
}

void ProjectItem::ProgCompileAfter()
{
    if (CompileGood)
    {
        SM->SwapPage = SwapPage << 8;
        SM->PrepareProgram(MemMode, TempDir + ProgFileBin, Engine, CodeLoc_, CodeSize, DataLoc_, DataSize);
    }
}

void ProjectItem::SysRun(string Cmd)
{
    system(Cmd.c_str());
}

void ProjectItem::ClearTemp()
{
    #ifdef Q_OS_WIN
        SysRun("del /q " + TempDir + "*.*");
    #endif
    #ifdef Q_OS_LINUX
        SysRun("rm -f " + TempDir + "*");
    #endif
}

string ProjectItem::GetFilePath(string FileName)
{
    string F = "";
    int T = -1;
    for (int I = FileName.length() - 1; I > 0; I--)
    {
        if ((FileName[I] == '/') || (FileName[I] == '\\'))
        {
            T = 0;
            while (T <= I)
            {
                F = F + FileName[T];
                T++;
            }
            return F;
        }
    }
    return "";
}

string ProjectItem::GetFileName(string FileName)
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
            return F;
        }
    }
    return FileName;
}

bool ProjectItem::FileCopy(string SrcFile, string DstDir)
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
