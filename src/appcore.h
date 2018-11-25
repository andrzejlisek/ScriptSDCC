#ifndef APPCORE_H
#define APPCORE_H

#include <string>
#include <iostream>
#include <sstream>
#include "eden.h"
#include "projectitem.h"
#include "membuffer.h"
#include "graphfont.h"
#include <QInputDialog>
#include "configfile.h"
#include "filehandle.h"

#define ProjFileExt "sdc"
#define BundFileExt "sdb"

using namespace std;

class AppCore
{
public:
    QString LastPath = "";
    string InputBox(QWidget *Parent, string Query, string Title, string Default);
    void SaveLastPath(QString X, bool OpenDir);

    AppCore();
    ~AppCore();

    MemBuffer * MemBuffer_;

    GraphFont * GraphFont_;
    string GraphFontFile;

    string ConsoleFontName = "Courier New";
    int ConsoleFontSize = 12;
    string SpreadsheetFontName = "Courier New";
    int SpreadsheetFontSize = 12;



    string GetStatus(llong &CommandCounter);
    uchar GetStatusI(int BundleIndex_);

    IOConsole * IOConsole_[4];
    IOSpreadsheet * IOSpreadsheet_[4];
    IOGraph * IOGraph_[4];

    QImage * MemMap = NULL;
    int MemMapH = 1;
    int MemMapV = 1;
    int MemMapHOffset = 0;
    int MemMapVOffset = 0;
    bool MemMapBuff = false;
    bool MemMapData = false;
    QImage * MemMapRepaint();


    int BundleIndex = 0;
    ProjectItem * Bundle[256];
    int BundleCount = 256;

    FileHandle * FileHandle_;

    string BundleFileName = "";

    void Compile();
};

#endif // APPCORE_H
