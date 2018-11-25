#include "appcore.h"

AppCore::AppCore()
{
    EdenClass::ConfigFile Sch;
    Sch.FileLoad(Eden::ApplicationDirectory() + "settings.txt");
    Sch.ParamGet("GraphFontFile", GraphFontFile);
    GraphFont_ = NEW(GraphFont, GraphFont(GraphFontFile));

    IOConsole_[0] = NEW(IOConsole, IOConsole());
    IOConsole_[1] = NEW(IOConsole, IOConsole());
    IOConsole_[2] = NEW(IOConsole, IOConsole());
    IOConsole_[3] = NEW(IOConsole, IOConsole());
    IOSpreadsheet_[0] = NEW(IOSpreadsheet, IOSpreadsheet());
    IOSpreadsheet_[1] = NEW(IOSpreadsheet, IOSpreadsheet());
    IOSpreadsheet_[2] = NEW(IOSpreadsheet, IOSpreadsheet());
    IOSpreadsheet_[3] = NEW(IOSpreadsheet, IOSpreadsheet());
    IOGraph_[0] = NEW(IOGraph, IOGraph(GraphFont_));
    IOGraph_[1] = NEW(IOGraph, IOGraph(GraphFont_));
    IOGraph_[2] = NEW(IOGraph, IOGraph(GraphFont_));
    IOGraph_[3] = NEW(IOGraph, IOGraph(GraphFont_));

    MemBuffer_ = NEW(MemBuffer, MemBuffer());
    FileHandle_ = NEW(FileHandle, FileHandle());

    for (int I = 0; I < BundleCount; I++)
    {
        Bundle[I] = NEW(ProjectItem, ProjectItem());
    }

    for (int I = 0; I < BundleCount; I++)
    {
        for (int II = 0; II < BundleCount; II++)
        {
            Bundle[I]->Bundle[II] = Bundle[II];
        }
    }

}

AppCore::~AppCore()
{
    for (int I = 0; I < BundleCount; I++)
    {
        DEL(ProjectItem, Bundle[I]);
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
    DELNULL(MemBuffer, MemBuffer_);
    DELNULL(FileHandle, FileHandle_);
    DELNULL(GraphFont, GraphFont_);
}


string AppCore::InputBox(QWidget *Parent, string Query, string Title, string Default)
{
    bool OK;
    QString X = QInputDialog::getText(Parent, Eden::ToQStr(Title), Eden::ToQStr(Query), QLineEdit::Normal, Eden::ToQStr(Default), &OK);
    if (OK)
    {
        return Eden::ToStr(X);
    }
    else
    {
        return "";
    }
}

void AppCore::SaveLastPath(QString X, bool OpenDir)
{
    if (!X.isEmpty())
    {
        if (OpenDir)
        {
            LastPath = QFileInfo(X).filePath();
        }
        else
        {
            LastPath = QFileInfo(X).path();
        }
    }
}



string AppCore::GetStatus(llong &CommandCounter)
{
    int BundleIndex_ = BundleIndex;
    if (Bundle[BundleIndex_]->SM != NULL)
    {
        CommandCounter = Bundle[BundleIndex_]->SM->CommandCounter;
        return Bundle[BundleIndex_]->SM->GetStatus();
    }
    else
    {
        return "Unknown";
    }
}

uchar AppCore::GetStatusI(int BundleIndex_)
{
    if (Bundle[BundleIndex_]->SM != NULL)
    {
        return Bundle[BundleIndex_]->SM->StatusC;
    }
    else
    {
        return 255;
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
    int PX = (MemMapVOffset << 8);
    int P4 = 0;
    int VisW = 256 - MemMapHOffset;
    int VisH = 256 - MemMapVOffset;
    int BundleIndex_ = BundleIndex;
    if (MemMapBuff)
    {
        if (MemBuffer_->BufRaw[BundleIndex_] != NULL)
        {
            for (int Y = 0; Y < VisH; Y++)
            {
                for (int YY = 0; YY < MemMapV; YY++)
                {
                    for (int X = 0; X < VisW; X++)
                    {
                        for (int XX = 0; XX < MemMapH; XX++)
                        {
                            MemMapRaw[P4 + 0] = 0;
                            MemMapRaw[P4 + 1] = MemBuffer_->BufOpR[BundleIndex_][P] ? 255 : 0;
                            MemMapRaw[P4 + 2] = MemBuffer_->BufOpW[BundleIndex_][P] ? 255 : 0;
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
            for (int I = ((MemBmpW * MemBmpH) - 1); I >= 0; I--)
            {
                MemMapRaw[P4 + 0] = 0;
                MemMapRaw[P4 + 1] = 0;
                MemMapRaw[P4 + 2] = 0;
                MemMapRaw[P4 + 3] = 0;
                P4 += 4;
            }
        }
    }
    else
    {
        if (Bundle[BundleIndex_]->SM != NULL)
        {
            ScriptMachine * SM = Bundle[BundleIndex_]->SM;
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
                                    if (PX == SM->SwapPage)
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
                                    if (PX == SM->SwapPage)
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
                    PX += 256;
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
                                    if (PX == SM->SwapPage)
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
                    PX += 256;
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
    }
    return MemMap;
}

void AppCore::Compile()
{
    int BundleIndex_ = BundleIndex;
    Bundle[BundleIndex_]->ProgCompile();
    if (Bundle[BundleIndex_]->CompileGood)
    {
        Bundle[BundleIndex_]->SM->BundleIndex = BundleIndex_;
        Bundle[BundleIndex_]->SM->MemBuffer_ = MemBuffer_;
        Bundle[BundleIndex_]->SM->FileHandle_ = FileHandle_;
        Bundle[BundleIndex_]->SM->IOConsole_[0] = IOConsole_[0];
        Bundle[BundleIndex_]->SM->IOConsole_[1] = IOConsole_[1];
        Bundle[BundleIndex_]->SM->IOConsole_[2] = IOConsole_[2];
        Bundle[BundleIndex_]->SM->IOConsole_[3] = IOConsole_[3];
        Bundle[BundleIndex_]->SM->IOSpreadsheet_[0] = IOSpreadsheet_[0];
        Bundle[BundleIndex_]->SM->IOSpreadsheet_[1] = IOSpreadsheet_[1];
        Bundle[BundleIndex_]->SM->IOSpreadsheet_[2] = IOSpreadsheet_[2];
        Bundle[BundleIndex_]->SM->IOSpreadsheet_[3] = IOSpreadsheet_[3];
        Bundle[BundleIndex_]->SM->IOGraph_[0] = IOGraph_[0];
        Bundle[BundleIndex_]->SM->IOGraph_[1] = IOGraph_[1];
        Bundle[BundleIndex_]->SM->IOGraph_[2] = IOGraph_[2];
        Bundle[BundleIndex_]->SM->IOGraph_[3] = IOGraph_[3];
    }
    Bundle[BundleIndex_]->ProgCompileAfter();
}
