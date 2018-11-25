#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include <string>
#include "scriptmachine.h"
#include "scriptmachinemcs51.h"
#include "scriptmachinez180.h"

using namespace std;

class ProjectItem
{
public:
    ProjectItem * Bundle[256];

    ProjectItem();
    ~ProjectItem();
    string BundleFile;
    string BundleDesc;
    string Descr;
    ScriptMachine * SM;
    void ProgRun();
    void ProgAbort();
    void ProgReset();
    bool CompileGood = false;
    void ProgCompile();
    void ProgCompileAfter();

    int Engine = 0;
    int MemMode = 0;
    string CompileCommand = "";

    string LibDir = "";
    string LibFiles = "";
    string TempDir = "";
    string ProgFileSrc = "";
    string ProgFileBin = "";
    string ProgCmd = "sdcc";
    string ViewCmd = "";

    bool BtnCompile = true;
    bool BtnReset = false;
    bool BtnRun = false;
    bool BtnAbort = false;

    int SwapPage = 0;
    int CodeLoc_ = 0;
    int CodeSize = 0;
    int DataLoc_ = 0;
    int DataSize = 0;

    string CompileMsg = "";

    void Init();

private:
    bool FileCopy(string SrcFile, string DstDir);
    void ClearTemp();
    void SysRun(string Cmd);
    string GetFilePath(string FileName);
    string GetFileName(string FileName);
};

#endif // PROJECTITEM_H
