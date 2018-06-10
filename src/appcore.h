#ifndef APPCORE_H
#define APPCORE_H

#include <string>
#include <iostream>
#include <sstream>
#include "scriptmachine.h"
#include "scriptmachinemcs51.h"
#include "scriptmachinez180.h"
#include "eden.h"

#define ProjFileExt "sdc"

using namespace std;

class AppCore
{
public:
    AppCore();
    ~AppCore();
    bool CompileGood = false;
    string ProgCompile(string LibFiles);
    void ProgRun();
    void ProgAbort();
    void ProgReset();
    ScriptMachine * SM = NULL;

    string ConsoleFontName = "Courier New";
    int ConsoleFontSize = 12;
    string SpreadsheetFontName = "Courier New";
    int SpreadsheetFontSize = 12;


    string LibDir = "";
    string TempDir = "";
    string ProgFileSrc = "";
    string ProgFileBin = "";
    string ProgCmd = "sdcc";

    string GetStatus(llong &CommandCounter);

    IOConsole * IOConsole_[4];
    IOSpreadsheet * IOSpreadsheet_[4];
    IOGraph * IOGraph_[4];

    int Engine = 0;
    int MemMode = 0;
    string CompileCommand = "";

    QImage * MemMap = NULL;
    int MemMapH = 1;
    int MemMapV = 1;
    int MemMapHOffset = 0;
    int MemMapVOffset = 0;
    bool MemMapData = false;
    QImage * MemMapRepaint();

    int SwapPage = 0;
    int CodeLoc = 0;
    int CodeSize = 0;
    int DataLoc = 0;
    int DataSize = 0;

    string CurrentFileName = "";

private:
    bool FileCopy(string SrcFile, string DstDir);
    void ClearTemp();
    void SysRun(string Cmd);
    string GetFilePath(string FileName);
    string GetFileName(string FileName);
};

#endif // APPCORE_H
