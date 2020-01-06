#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Core = NEW(AppCore, AppCore());
    ui->setupUi(this);
    GuiEvt();


    EdenClass::ConfigFile Sch;
    Sch.FileLoad(Eden::ApplicationDirectory() + "settings.txt");
    int N = 0;
    int TimerInterval = 500;

    string XLibDir = "lib";
    string XTempDir = "temp";
    string XProgCmd = "sdcc";

    XLibDir = Eden::MultilineEncode(XLibDir);
    Sch.ParamGet("LibDir", XLibDir);
    XLibDir = Eden::MultilineDecode(XLibDir);
    Sch.ParamGet("TempDir", XTempDir);
    Sch.ParamGet("ProgCmd", XProgCmd);
    Sch.ParamGet("TimerInterval", TimerInterval);

    ui->SetLibDirT->setPlainText(Eden::ToQStr(XLibDir));
    ui->SetTempDirT->setText(Eden::ToQStr(XTempDir));
    ui->SetProgCommandT->setText(Eden::ToQStr(XProgCmd));
    ui->SetTimerT->setValue(TimerInterval);

    Sch.ParamGet("ConsoleFontName", Core->ConsoleFontName);
    Sch.ParamGet("ConsoleFontSize", Core->ConsoleFontSize);
    Sch.ParamGet("SpreadsheetFontName", Core->SpreadsheetFontName);
    Sch.ParamGet("SpreadsheetFontSize", Core->SpreadsheetFontSize);

    ui->SetConsFontN->setText(Eden::ToQStr(Core->ConsoleFontName));
    ui->SetConsFontS->setValue(Core->ConsoleFontSize);
    ui->SetSheetFontN->setText(Eden::ToQStr(Core->SpreadsheetFontName));
    ui->SetSheetFontS->setValue(Core->SpreadsheetFontSize);

    ui->SetGraphFontT->setText(Eden::ToQStr(Core->GraphFontFile));

    SetEventEnabled = true;
    CommandCounterF = 100000 / TimerInterval;

    //XLibDir = Eden::CorrectDir(XLibDir);
    XTempDir = Eden::CorrectDir(XTempDir);

    for (int I = 0; I < Core->BundleCount; I++)
    {
        Core->Bundle[I]->LibDir = XLibDir;
        Core->Bundle[I]->TempDir = XTempDir;
        Core->Bundle[I]->ProgCmd = XProgCmd;
    }

    Sch.ParamGet("CompileCount", N);
    for (int I = 0; I < N; I++)
    {
        CompileSchemeName.push_back(Sch.ParamGetS("CompileName" + Eden::ToStr(I)));
        CompileSchemeEngine1.push_back(Sch.ParamGetI("CompileEngine1" + Eden::ToStr(I)));
        CompileSchemeEngine2.push_back(Sch.ParamGetI("CompileEngine2" + Eden::ToStr(I)));
        CompileSchemeCommand.push_back(Sch.ParamGetS("CompileCommand" + Eden::ToStr(I)));
        ui->CompileSchemeT->addItem(Eden::ToQStr(Sch.ParamGetS("CompileName" + Eden::ToStr(I))));
    }
    SchemeSort();
    ui->CompileSchemeT->setCurrentIndex(0);


    N = 0;
    Sch.ParamGet("ProjectCount", N);
    ui->ProjListT->addItem("");
    for (int I = 0; I < N; I++)
    {
        ProjectListName.push_back(Sch.ParamGetS("ProjectName" + Eden::ToStr(I)));
        ProjectListFile.push_back(Sch.ParamGetS("ProjectFile" + Eden::ToStr(I)));
        ui->ProjListT->addItem(Eden::ToQStr(Sch.ParamGetS("ProjectName" + Eden::ToStr(I))));
    }
    ProjectSort();
    ui->ProjListT->setCurrentIndex(0);


    connect(&Timer, SIGNAL(timeout()), this, SLOT(TimerEvent()));

    connect(this, SIGNAL(GuiEvtS()), this, SLOT(GuiEvt()));

    TimerEventEnabled = true;
    Timer.start(TimerInterval);
    on_ProjNew_clicked();

    BundleRefresh();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    TimerEventEnabled = false;
    for (int I = 0; I < Core->BundleCount; I++)
    {
        Core->Bundle[I]->ProgAbort();
    }
    Timer.stop();
    event++;
    event--;

    int N = CompileSchemeName.size();
    EdenClass::ConfigFile Sch;
    Sch.ParamClear();
    Sch.ParamSet("LibDir", Eden::MultilineEncode(Eden::ToStr(ui->SetLibDirT->toPlainText())));
    Sch.ParamSet("TempDir", Eden::ToStr(ui->SetTempDirT->text()));
    Sch.ParamSet("ProgCmd", Eden::ToStr(ui->SetProgCommandT->text()));
    Sch.ParamSet("TimerInterval", ui->SetTimerT->value());

    Sch.ParamSet("ConsoleFontName", Eden::ToStr(ui->SetConsFontN->text()));
    Sch.ParamSet("ConsoleFontSize", ui->SetConsFontS->value());
    Sch.ParamSet("SpreadsheetFontName", Eden::ToStr(ui->SetSheetFontN->text()));
    Sch.ParamSet("SpreadsheetFontSize", ui->SetSheetFontS->value());
    Sch.ParamSet("SpreadsheetFontName", Eden::ToStr(ui->SetSheetFontN->text()));
    Sch.ParamSet("GraphFontFile", Eden::ToStr(ui->SetGraphFontT->text()));

    Sch.ParamSet("CompileCount", N);
    for (int I = 0; I < N; I++)
    {
        Sch.ParamSet("CompileName" + Eden::ToStr(I), CompileSchemeName[I]);
        Sch.ParamSet("CompileEngine1" + Eden::ToStr(I), CompileSchemeEngine1[I]);
        Sch.ParamSet("CompileEngine2" + Eden::ToStr(I), CompileSchemeEngine2[I]);
        Sch.ParamSet("CompileCommand" + Eden::ToStr(I), CompileSchemeCommand[I]);
    }

    N = ProjectListName.size();
    Sch.ParamSet("ProjectCount", N);
    for (int I = 0; I < N; I++)
    {
        Sch.ParamSet("ProjectName" + Eden::ToStr(I), ProjectListName[I]);
        Sch.ParamSet("ProjectFile" + Eden::ToStr(I), ProjectListFile[I]);
    }

    Sch.FileSave(Eden::ApplicationDirectory() + "settings.txt");

    int I, L;
    L = IOConsoleWinX.size();
    for (I = 0; I < L; I++)
    {
        if (IOConsoleWinX[I] != NULL)
        {
            IOConsoleWinX[I]->close();
            DEL(IOConsoleWin, IOConsoleWinX[I]);
        }
    }
    L = IOSpreadsheetWinX.size();
    for (I = 0; I < L; I++)
    {
        if (IOSpreadsheetWinX[I] != NULL)
        {
            IOSpreadsheetWinX[I]->close();
            DEL(IOSpreadsheetWin, IOSpreadsheetWinX[I]);
        }
    }
    L = IOGraphWinX.size();
    for (I = 0; I < L; I++)
    {
        if (IOGraphWinX[I] != NULL)
        {
            IOGraphWinX[I]->close();
            DEL(IOGraphWin, IOGraphWinX[I]);
        }
    }
    //disconnect(&Timer, SIGNAL(timeout()), this, SLOT(TimerEvent()));
    //DEL(AppCore, Core);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_BtnCompile_clicked()
{
    BundleItemSet();
    ProgLoadCode(true);
    GuiEvt();
}

void MainWindow::on_BtnRun_clicked()
{
    ExecStart(Core->BundleIndex);
}

int MainWindow::ExecStart(int Idx)
{
    uchar StatusX = Core->GetStatusI(Idx);
    if ((StatusX != 0) && (StatusX != 2) && (StatusX != 3))
    {
        return 0;
    }

    Core->Bundle[Idx]->BtnCompile = false;
    Core->Bundle[Idx]->BtnRun = false;
    Core->Bundle[Idx]->BtnReset = false;
    Core->Bundle[Idx]->BtnAbort = true;
    GuiEvt();

    std::thread Thr(&MainWindow::ExecThr, this, Idx);
    Thr.detach();
    return 1;
}

void MainWindow::ExecThr(int Idx)
{
    Core->Bundle[Idx]->ProgRun();
    Core->Bundle[Idx]->BtnCompile = true;
    Core->Bundle[Idx]->BtnRun = true;
    Core->Bundle[Idx]->BtnReset = true;
    Core->Bundle[Idx]->BtnAbort = false;
    emit GuiEvtS();
}

void MainWindow::on_BtnReset_clicked()
{
    Core->Bundle[Core->BundleIndex]->ProgReset();
}

void MainWindow::on_BtnAbort_clicked()
{
    int Idx = Core->BundleIndex;
    Core->Bundle[Idx]->BtnAbort = false;
    GuiEvt();
    Core->Bundle[Idx]->ProgAbort();
}

void MainWindow::TimerEvent()
{
    if (!TimerEventEnabled)
    {
        return;
    }

    llong CommandCounter = 0;
    ui->ProgStatusT->setText(Eden::ToQStr(Core->GetStatus(CommandCounter)));
    ui->ProgInstrT->setText(Eden::ToQStr(CommandCounter));
    if (CommandCounter > LastInstrCount)
    {
        ui->ProgIpsT->setText(Eden::ToQStr(((CommandCounter - LastInstrCount) * CommandCounterF) / 100LL));
    }
    else
    {
        ui->ProgIpsT->setText("0");
    }
    LastInstrCount = CommandCounter;
    int I, L;
    L = IOConsoleWinX.size();
    for (I = 0; I < L; I++)
    {
        if (IOConsoleWinX[I] != NULL)
        {
            if (IOConsoleWinX[I]->isVisible())
            {
                IOConsoleWinX[I]->Refresh();
            }
            else
            {
                DEL(IOConsoleWin, IOConsoleWinX[I]);
                if ((I + 1) < L)
                {
                    IOConsoleWinX[I] = IOConsoleWinX[L - 1];
                }
                IOConsoleWinX.pop_back();
            }
        }
    }
    for (I = 0; I < 4; I++)
    {
        Core->IOConsole_[I]->ResetChanged();
    }

    SpreadSheetN.clear();
    SpreadSheetR.clear();
    SpreadSheetC.clear();
    int CellR;
    int CellC;
    for (I = 0; I < 4; I++)
    {
        while (Core->IOSpreadsheet_[I]->CellChanged(CellR, CellC))
        {
            SpreadSheetN.push_back(I);
            SpreadSheetR.push_back(CellR);
            SpreadSheetC.push_back(CellC);
        }
    }
    L = IOSpreadsheetWinX.size();
    for (I = 0; I < L; I++)
    {
        if (IOSpreadsheetWinX[I] != NULL)
        {
            if (IOSpreadsheetWinX[I]->isVisible())
            {
                for (uint II = 0; II < SpreadSheetN.size(); II++)
                {
                    IOSpreadsheetWinX[I]->RefreshVal(SpreadSheetN[II], SpreadSheetR[II], SpreadSheetC[II]);
                }
            }
            else
            {
                DEL(IOSpreadsheetWin, IOSpreadsheetWinX[I]);
                if ((I + 1) < L)
                {
                    IOSpreadsheetWinX[I] = IOSpreadsheetWinX[L - 1];
                }
                IOSpreadsheetWinX.pop_back();
            }
        }
    }
    L = IOGraphWinX.size();
    for (I = 0; I < L; I++)
    {
        if (IOGraphWinX[I] != NULL)
        {
            if (IOGraphWinX[I]->isVisible())
            {
                IOGraphWinX[I]->Refresh(false, "", 0, 0);
            }
            else
            {
                DEL(IOGraphWin, IOGraphWinX[I]);
                if ((I + 1) < L)
                {
                    IOGraphWinX[I] = IOGraphWinX[L - 1];
                }
                IOGraphWinX.pop_back();
            }
        }
    }
    Core->IOGraph_[0]->NeedRedrawLast = -1;
    Core->IOGraph_[1]->NeedRedrawLast = -1;
    Core->IOGraph_[2]->NeedRedrawLast = -1;
    Core->IOGraph_[3]->NeedRedrawLast = -1;
    Core->IOGraph_[0]->NeedRedraw = false;
    Core->IOGraph_[1]->NeedRedraw = false;
    Core->IOGraph_[2]->NeedRedraw = false;
    Core->IOGraph_[3]->NeedRedraw = false;

    if (ui->ProjectsTabs->currentIndex() == 3)
    {
        QImage * X = Core->MemMapRepaint();
        if (ui->MemMap->ImgX != NULL)
        {
            if (ui->MemMap->ImgX != X)
            {
                QImage * XX = ui->MemMap->ImgX;
                ui->MemMap->ImgX = NULL;
                DEL(QImage, XX);
            }
        }
        ui->MemMap->ImgX = X;
        ui->MemMap->repaint();
        ui->FileInfo->setText(Eden::ToQStr(Core->FileHandle_->Info(Core->BundleIndex)));
    }
}

void MainWindow::on_BtnConsole_clicked()
{
    IOConsoleWin * IOConsoleWin_ = NEW(IOConsoleWin, IOConsoleWin());
    IOConsoleWin_->Core = Core;
    IOConsoleWin_->SetDispFont();
    IOConsoleWin_->show();
    IOConsoleWin_->Refresh();
    IOConsoleWinX.push_back(IOConsoleWin_);
}

void MainWindow::on_BtnSpreadsheet_clicked()
{
    IOSpreadsheetWin * IOSpreadsheetWin_ = NEW(IOSpreadsheetWin, IOSpreadsheetWin());
    IOSpreadsheetWin_->Core = Core;
    IOSpreadsheetWin_->SetDispFont();
    IOSpreadsheetWin_->show();
    IOSpreadsheetWin_->Refresh();
    IOSpreadsheetWinX.push_back(IOSpreadsheetWin_);
}

void MainWindow::on_BtnGraph_clicked()
{
    IOGraphWin * IOGraphWin_ = NEW(IOGraphWin, IOGraphWin());
    IOGraphWin_->Core = Core;
    IOGraphWin_->show();
    IOGraphWin_->Refresh(false, "", 0, 0);
    IOGraphWinX.push_back(IOGraphWin_);
}

void MainWindow::on_MemMapHoriI_clicked()
{
    Core->MemMapH++;
}

void MainWindow::on_MemMapHoriO_clicked()
{
    if (Core->MemMapH > 1)
    {
        Core->MemMapH--;
    }
}

void MainWindow::on_MemMapVertI_clicked()
{
    Core->MemMapV++;
}

void MainWindow::on_MemMapVertO_clicked()
{
    if (Core->MemMapV > 1)
    {
        Core->MemMapV--;
    }
}

void MainWindow::on_MemMapHScroll_valueChanged(int value)
{
    Core->MemMapHOffset = value;
}

void MainWindow::on_MemMapVScroll_valueChanged(int value)
{
    Core->MemMapVOffset = value;
}

void MainWindow::on_MemMapDispCode_clicked()
{
    Core->MemMapBuff = false;
    Core->MemMapData = false;
}

void MainWindow::on_MemMapDispData_clicked()
{
    Core->MemMapBuff = false;
    Core->MemMapData = true;
}

void MainWindow::on_MemMapDispBuff_clicked()
{
    Core->MemMapBuff = true;
}

void MainWindow::ProgLoadCode(bool CompileCode)
{
    int Idx = Core->BundleIndex;
    if (Core->Bundle[Idx]->SM != NULL)
    {
        QObject::disconnect(Core->Bundle[Idx]->SM, SIGNAL(CoreInvoke(uchar, uchar, uchar)), this, SLOT(CoreInvoke(uchar, uchar, uchar)));
    }
    Core->Compile(CompileCode);
    QObject::connect(Core->Bundle[Idx]->SM, SIGNAL(CoreInvoke(uchar, uchar, uchar)), this, SLOT(CoreInvoke(uchar, uchar, uchar)));

    ui->CompileMsgT->setPlainText(Eden::ToQStr(Core->Bundle[Idx]->CompileMsg));
}

void MainWindow::ProjectOpen(string FileName, bool Template, bool NoRefresh)
{
    if ((FileName != "") && Eden::FileExists(FileName))
    {
        int Idx = Core->BundleIndex;
        if (!Template)
        {
            Core->Bundle[Idx]->BundleFile = FileName;
        }

        EdenClass::ConfigFile CF;
        CF.FileLoad(FileName);
        Core->Bundle[Idx]->ProgCode = CF.ParamGetS("ProgCode");
        Core->Bundle[Idx]->CompileMsg = Eden::MultilineDecode(CF.ParamGetS("CompileMsg"));
        Core->Bundle[Idx]->ViewCmd = CF.ParamGetS("ViewCommand");
        Core->Bundle[Idx]->ProgFileSrc = CF.ParamGetS("FileNameSource");
        Core->Bundle[Idx]->ProgFileBin = CF.ParamGetS("FileNameBinary");
        Core->Bundle[Idx]->Engine = CF.ParamGetI("Engine");
        Core->Bundle[Idx]->MemMode = CF.ParamGetI("Memory");
        Core->Bundle[Idx]->CompileCommand = CF.ParamGetS("Command");
        Core->Bundle[Idx]->CodeLoc_ = Eden::HexToInt(CF.ParamGetS("MemCodeLoc"));
        Core->Bundle[Idx]->CodeSize = Eden::HexToInt(CF.ParamGetS("MemCodeSize"));
        Core->Bundle[Idx]->DataLoc_ = Eden::HexToInt(CF.ParamGetS("MemDataLoc"));
        Core->Bundle[Idx]->DataSize = Eden::HexToInt(CF.ParamGetS("MemDataSize"));
        Core->Bundle[Idx]->SwapPage = Eden::HexToInt(CF.ParamGetS("MemSwapLoc"));
        Core->Bundle[Idx]->LibFiles = CF.ParamGetS("LibraryFiles");
        Core->Bundle[Idx]->Descr = CF.ParamGetS("Description");
        ProgLoadCode(false);


        GuiEvt();
        //ui->CompileSchemeT->setCurrentText("");

        if (!NoRefresh)
        {
            BundleItemGet();
            BundleRefresh();
        }
    }
}

void MainWindow::ProjectSave(string FileName)
{
    if (FileName != "")
    {
        int Idx = Core->BundleIndex;
        Core->Bundle[Idx]->BundleFile = FileName;

        BundleItemSet();
        EdenClass::ConfigFile CF;
        CF.ParamClear();
        CF.ParamSet("ProgCode", Core->Bundle[Idx]->ProgCode);
        CF.ParamSet("CompileMsg", Eden::MultilineEncode(Core->Bundle[Idx]->CompileMsg));
        CF.ParamSet("ViewCommand", Core->Bundle[Idx]->ViewCmd);
        CF.ParamSet("FileNameSource", Core->Bundle[Idx]->ProgFileSrc);
        CF.ParamSet("FileNameBinary", Core->Bundle[Idx]->ProgFileBin);
        CF.ParamSet("Engine", Core->Bundle[Idx]->Engine);
        CF.ParamSet("Memory", Core->Bundle[Idx]->MemMode);
        CF.ParamSet("Command", Core->Bundle[Idx]->CompileCommand);
        CF.ParamSet("MemCodeLoc", Eden::IntToHex8(Core->Bundle[Idx]->CodeLoc_));
        CF.ParamSet("MemCodeSize", Eden::IntToHex8(Core->Bundle[Idx]->CodeSize));
        CF.ParamSet("MemDataLoc", Eden::IntToHex8(Core->Bundle[Idx]->DataLoc_));
        CF.ParamSet("MemDataSize", Eden::IntToHex8(Core->Bundle[Idx]->DataSize));
        CF.ParamSet("MemSwapLoc", Eden::IntToHex8(Core->Bundle[Idx]->SwapPage));
        CF.ParamSet("LibraryFiles", Core->Bundle[Idx]->LibFiles);
        CF.ParamSet("Description", Core->Bundle[Idx]->Descr);
        CF.FileSave(FileName);
        BundleRefresh();
    }
}

void MainWindow::on_ProjNew_clicked()
{
    int Idx = Core->BundleIndex;
    Core->Bundle[Idx]->BundleFile = "";
    setWindowTitle(Eden::ToQStr(AppWindowTitle));

    ProjectOpen(Eden::ApplicationDirectory() + "default" + ProjFileExt, true, false);
    if (CompileSchemeName.size() > 0)
    {
        ui->CompileSchemeT->setCurrentIndex(0);
        on_CompileSchemeT_currentIndexChanged(0);
    }
    else
    {
        ui->CompileSchemeT->setCurrentText("");
    }
    BundleItemGet();
    BundleRefresh();
}

void MainWindow::on_ProjOpen_clicked()
{
    string FileName = "";
    QString X = ProjFileExt;
    QString FileNameX = QFileDialog::getOpenFileName(this, "Open program", Core->LastPath, "Files (*." + X + ")");
    Core->SaveLastPath(FileNameX, false);
    if (!FileNameX.isEmpty())
    {
        FileName = Eden::ToStr(FileNameX);
    }
    ProjectOpen(FileName, false, false);
}

void MainWindow::on_ProjSave_clicked()
{
    if (Core->Bundle[Core->BundleIndex]->BundleFile == "")
    {
        on_ProjSaveAs_clicked();
        return;
    }

    ProjectSave(Core->Bundle[Core->BundleIndex]->BundleFile);
}

void MainWindow::on_ProjSaveAs_clicked()
{
    string FileName = "";
    QString X = ProjFileExt;
    QString FileNameX = QFileDialog::getSaveFileName(this, "Save program", Core->LastPath, "Files (*." + X + ")");
    Core->SaveLastPath(FileNameX, false);
    if (!FileNameX.isEmpty())
    {
        FileName = Eden::ToStr(FileNameX);
    }

    if (FileName != "")
    {
        FileName = Eden::FileExtension(FileName, ProjFileExt);
        ProjectSave(FileName);
    }
    BundleItemGet();
}

void MainWindow::CompileSchemeRefresh()
{
    ui->CompileSchemeT->clear();
    for (uint I = 0; I < CompileSchemeName.size(); I++)
    {
        ui->CompileSchemeT->addItem(Eden::ToQStr(CompileSchemeName[I]));
    }
}

void MainWindow::on_CompileSchemeAdd_clicked()
{
    string SchemeName = Eden::ToStr(ui->CompileSchemeT->currentText());
    if (SchemeName == "")
    {
        return;
    }
    for (uint I = 0; I < CompileSchemeName.size(); I++)
    {
        if (CompileSchemeName[I] == SchemeName)
        {
            CompileSchemeEngine1[I] = ui->EngineT1->currentIndex();
            CompileSchemeEngine2[I] = ui->EngineT2->currentIndex();
            CompileSchemeCommand[I] = Eden::ToStr(ui->CommandT->text());
            return;
        }
    }
    CompileSchemeName.push_back(SchemeName);
    CompileSchemeEngine1.push_back(ui->EngineT1->currentIndex());
    CompileSchemeEngine2.push_back(ui->EngineT2->currentIndex());
    CompileSchemeCommand.push_back(Eden::ToStr(ui->CommandT->text()));
    ui->CompileSchemeT->addItem(Eden::ToQStr(SchemeName));

    SchemeSort();
}

void MainWindow::on_CompileSchemeRem_clicked()
{
    string SchemeName = Eden::ToStr(ui->CompileSchemeT->currentText());
    uint K = CompileSchemeName.size();
    for (uint I = 0; I < K; I++)
    {
        if (CompileSchemeName[I] == SchemeName)
        {
            for (uint J = (I + 1); J < K; J++)
            {
                CompileSchemeName[J - 1] = CompileSchemeName[J];
                CompileSchemeEngine1[J - 1] = CompileSchemeEngine1[J];
                CompileSchemeEngine2[J - 1] = CompileSchemeEngine2[J];
                CompileSchemeCommand[J - 1] = CompileSchemeCommand[J];
                ui->CompileSchemeT->setItemText(J - 1, Eden::ToQStr(CompileSchemeName[J]));
            }
            CompileSchemeName.pop_back();
            CompileSchemeEngine1.pop_back();
            CompileSchemeEngine2.pop_back();
            CompileSchemeCommand.pop_back();
            ui->CompileSchemeT->removeItem(K - 1);
            return;
        }
    }
}

void MainWindow::on_CompileSchemeT_currentIndexChanged(int index)
{
    ui->EngineT1->setCurrentIndex(CompileSchemeEngine1[index]);
    ui->EngineT2->setCurrentIndex(CompileSchemeEngine2[index]);
    ui->CommandT->setText(Eden::ToQStr(CompileSchemeCommand[index]));
}

void MainWindow::ProjectSort()
{
    string ProjName = Eden::ToStr(ui->ProjListT->currentText());
    int N = ProjectListName.size();
    for (int I = 0; I < N; I++)
    {
        for (int J = 0; J < N; J++)
        {
            if (ProjectListName[I] < ProjectListName[J])
            {
                string TempS;
                TempS = ProjectListName[I];
                ProjectListName[I] = ProjectListName[J];
                ProjectListName[J] = TempS;
                TempS = ProjectListFile[I];
                ProjectListFile[I] = ProjectListFile[J];
                ProjectListFile[J] = TempS;
                ui->ProjListT->setItemText(I + 1, Eden::ToQStr(ProjectListName[I]));
                ui->ProjListT->setItemText(J + 1, Eden::ToQStr(ProjectListName[J]));
            }
        }
    }
    ui->ProjListT->setCurrentText(Eden::ToQStr(ProjName));
}

void MainWindow::SchemeSort()
{
    string SchemeName = Eden::ToStr(ui->CompileSchemeT->currentText());
    int N = CompileSchemeName.size();
    for (int I = 0; I < N; I++)
    {
        for (int J = 0; J < N; J++)
        {
            if (CompileSchemeName[I] < CompileSchemeName[J])
            {
                string TempS;
                int TempI;
                TempS = CompileSchemeName[I];
                CompileSchemeName[I] = CompileSchemeName[J];
                CompileSchemeName[J] = TempS;
                TempI = CompileSchemeEngine1[I];
                CompileSchemeEngine1[I] = CompileSchemeEngine1[J];
                CompileSchemeEngine1[J] = TempI;
                TempI = CompileSchemeEngine2[I];
                CompileSchemeEngine2[I] = CompileSchemeEngine2[J];
                CompileSchemeEngine2[J] = TempI;
                TempS = CompileSchemeCommand[I];
                CompileSchemeCommand[I] = CompileSchemeCommand[J];
                CompileSchemeCommand[J] = TempS;
                ui->CompileSchemeT->setItemText(I, Eden::ToQStr(CompileSchemeName[I]));
                ui->CompileSchemeT->setItemText(J, Eden::ToQStr(CompileSchemeName[J]));
            }
        }
    }
    ui->CompileSchemeT->setCurrentText(Eden::ToQStr(SchemeName));
}

void MainWindow::GuiEvt()
{
    int Idx = Core->BundleIndex;
    ui->BtnCompile->setEnabled(Core->Bundle[Idx]->BtnCompile);
    ui->BtnReset->setEnabled(Core->Bundle[Idx]->BtnReset);
    ui->BtnRun->setEnabled(Core->Bundle[Idx]->BtnRun);
    ui->BtnAbort->setEnabled(Core->Bundle[Idx]->BtnAbort);
}

void MainWindow::on_ViewCommandB_clicked()
{
    string X = Eden::ToStr(ui->ViewCommandT->text());
    Core->Bundle[Core->BundleIndex]->SysRun(X, false);
}

void MainWindow::on_ProjListAdd_clicked()
{
    string ProjName = Eden::ToStr(ui->ProjListT->currentText());
    if ((Core->Bundle[Core->BundleIndex]->BundleFile != "") && (ProjName != ""))
    {
        for (uint I = 0; I < ProjectListName.size(); I++)
        {
            if (ProjectListName[I] == ProjName)
            {
                ProjectListFile[I] = Core->Bundle[Core->BundleIndex]->BundleFile;
                return;
            }
        }
        ProjectListName.push_back(ProjName);
        ProjectListFile.push_back(Core->Bundle[Core->BundleIndex]->BundleFile);
        ui->ProjListT->addItem(Eden::ToQStr(ProjName));
        ProjectSort();
    }
}

void MainWindow::on_ProjListRem_clicked()
{
    string ProjName = Eden::ToStr(ui->ProjListT->currentText());
    if (ProjName != "")
    {
        uint K = ProjectListName.size();
        for (uint I = 0; I < K; I++)
        {
            if (ProjectListName[I] == ProjName)
            {
                for (uint J = (I + 1); J < K; J++)
                {
                    ProjectListName[J - 1] = ProjectListName[J];
                    ProjectListFile[J - 1] = ProjectListFile[J];
                    ui->ProjListT->setItemText(J, Eden::ToQStr(ProjectListName[J]));
                }
                ProjectListName.pop_back();
                ProjectListFile.pop_back();
                ui->ProjListT->removeItem(K);
                return;
            }
        }
    }
}

void MainWindow::on_ProjListT_currentIndexChanged(int index)
{
    if (index > 0)
    {
        ProjectOpen(ProjectListFile[index - 1], false, false);
    }
}

void MainWindow::on_SetProgCommandT_textEdited(const QString &arg1)
{
    if (SetEventEnabled)
    {
        Core->Bundle[Core->BundleIndex]->ProgCmd = Eden::ToStr(arg1);
    }
}

void MainWindow::on_SetTempDirT_textEdited(const QString &arg1)
{
    if (SetEventEnabled)
    {
        Core->Bundle[Core->BundleIndex]->TempDir = Eden::CorrectDir(Eden::ToStr(arg1));
    }
}

void MainWindow::on_SetConsFontN_textEdited(const QString &arg1)
{
    if (SetEventEnabled)
    {
        Core->ConsoleFontName = Eden::ToStr(arg1);
    }
}

void MainWindow::on_SetConsFontS_valueChanged(int arg1)
{
    if (SetEventEnabled)
    {
        Core->ConsoleFontSize = arg1;
    }
}

void MainWindow::on_SetSheetFontN_textEdited(const QString &arg1)
{
    if (SetEventEnabled)
    {
        Core->SpreadsheetFontName = Eden::ToStr(arg1);
    }
}

void MainWindow::on_SetSheetFontS_valueChanged(int arg1)
{
    if (SetEventEnabled)
    {
        Core->SpreadsheetFontSize = arg1;
    }
}

void MainWindow::BundleRefresh()
{
    int I;
    if (ui->BundleList->count() == 0)
    {
        for (I = 0; I < Core->BundleCount; I++)
        {
            ui->BundleList->addItem("X");
        }
        ui->BundleList->setCurrentRow(0);
    }
    for (I = 0; I < Core->BundleCount; I++)
    {
        if (Core->Bundle[I]->BundleDesc != "")
        {
            ui->BundleList->item(I)->setText(Eden::ToQStr(I) + "  " + Eden::ToQStr(Core->Bundle[I]->BundleDesc));
        }
        else
        {
            ui->BundleList->item(I)->setText(Eden::ToQStr(I) + "  " + Eden::ToQStr(Core->Bundle[I]->BundleFile));
        }
    }
}

void MainWindow::BundleItemGet()
{
    int Idx = Core->BundleIndex;
    ui->CompileMsgT->setPlainText(Eden::ToQStr(Core->Bundle[Idx]->CompileMsg));
    ui->ViewCommandT->setText(Eden::ToQStr(Core->Bundle[Idx]->ViewCmd));
    ui->EngineT1->setCurrentIndex(Core->Bundle[Idx]->Engine);
    ui->EngineT2->setCurrentIndex(Core->Bundle[Idx]->MemMode);
    ui->CommandT->setText(Eden::ToQStr(Core->Bundle[Idx]->CompileCommand));
    ui->FileNameT1->setText(Eden::ToQStr(Core->Bundle[Idx]->ProgFileSrc));
    ui->FileNameT2->setText(Eden::ToQStr(Core->Bundle[Idx]->ProgFileBin));
    ui->MemSwapT->setText(Eden::ToQStr(Eden::IntToHex8(Core->Bundle[Idx]->SwapPage)));
    ui->MemCodeT1->setText(Eden::ToQStr(Eden::IntToHex8(Core->Bundle[Idx]->CodeLoc_)));
    ui->MemCodeT2->setText(Eden::ToQStr(Eden::IntToHex8(Core->Bundle[Idx]->CodeSize)));
    ui->MemDataT1->setText(Eden::ToQStr(Eden::IntToHex8(Core->Bundle[Idx]->DataLoc_)));
    ui->MemDataT2->setText(Eden::ToQStr(Eden::IntToHex8(Core->Bundle[Idx]->DataSize)));
    ui->LibraryT->setText(Eden::ToQStr(Core->Bundle[Idx]->LibFiles));
    ui->DescrT->setPlainText(Eden::ToQStr(Eden::MultilineDecode(Core->Bundle[Idx]->Descr)));
    GuiEvt();
    ui->BundleItemFileT->setText(Eden::ToQStr(Core->Bundle[Idx]->BundleFile));
    ui->BundleItemDescT->setText(Eden::ToQStr(Core->Bundle[Idx]->BundleDesc));

    string X = AppWindowTitle;
    setWindowTitle(Eden::ToQStr(X + " - " + Core->Bundle[Idx]->BundleFile));
}

void MainWindow::BundleItemSet()
{
    int Idx = Core->BundleIndex;
    Core->Bundle[Idx]->ViewCmd = Eden::ToStr(ui->ViewCommandT->text());
    Core->Bundle[Idx]->Engine = ui->EngineT1->currentIndex();
    Core->Bundle[Idx]->MemMode = ui->EngineT2->currentIndex();
    Core->Bundle[Idx]->CompileCommand = Eden::ToStr(ui->CommandT->text());
    Core->Bundle[Idx]->ProgFileSrc = Eden::ToStr(ui->FileNameT1->text());
    Core->Bundle[Idx]->ProgFileBin = Eden::ToStr(ui->FileNameT2->text());
    Core->Bundle[Idx]->SwapPage = Eden::HexToInt(Eden::ToStr(ui->MemSwapT->text()));
    Core->Bundle[Idx]->CodeLoc_ = Eden::HexToInt(Eden::ToStr(ui->MemCodeT1->text()));
    Core->Bundle[Idx]->CodeSize = Eden::HexToInt(Eden::ToStr(ui->MemCodeT2->text()));
    Core->Bundle[Idx]->DataLoc_ = Eden::HexToInt(Eden::ToStr(ui->MemDataT1->text()));
    Core->Bundle[Idx]->DataSize = Eden::HexToInt(Eden::ToStr(ui->MemDataT2->text()));
    Core->Bundle[Idx]->LibFiles = Eden::ToStr(ui->LibraryT->text());
    Core->Bundle[Idx]->Descr = Eden::MultilineEncode(Eden::ToStr(ui->DescrT->toPlainText()));
}

void MainWindow::CoreInvoke(uchar Idx, uchar Param1, uchar Param2)
{
    switch (Param1)
    {
        case 0:
            {
                uchar StatusX = Core->GetStatusI(Param2);
                uchar StatusResult = 2;
                switch (StatusX)
                {
                    case 0:
                    case 2:
                    case 3:
                        StatusResult = 0;
                        break;
                    case 1:
                        StatusResult = 1;
                        break;
                }
                Core->Bundle[Idx]->SM->CoreInvokeWait = StatusResult;
            }
            break;
        case 1:
            {
                Core->Bundle[Idx]->SM->CoreInvokeWait = ExecStart(Param2);
            }
            break;
    }
}

void MainWindow::on_MemFileLoad_clicked()
{
    string FileName = "";
    QString FileNameX = QFileDialog::getOpenFileName(this, "Load file to memory", Core->LastPath, "Files (*.*)");
    Core->SaveLastPath(FileNameX, false);
    if (!FileNameX.isEmpty())
    {
        FileName = Eden::ToStr(FileNameX);
    }

    if (FileName != "")
    {
        int MemAddr = Eden::ToInt(Core->InputBox(this, "Address", "Load from file", "0"));
        int MemSize = Eden::ToInt(Core->InputBox(this, "Size", "Load from file", "65536"));
        if ((MemAddr >= 0) && ((MemAddr + MemSize) <= 65536) && (MemSize > 0))
        {
            if (Eden::FileExists(FileName))
            {
                int SizeX = Eden::FileSize(FileName);
                if (MemSize > SizeX)
                {
                    MemSize = SizeX;
                }
                if (MemSize > 0)
                {
                    fstream FS(FileName, ios::binary | ios::in);
                    if (FS.is_open())
                    {
                        uchar * Temp = NEWARR(uchar, uchar[MemSize]);
                        FS.read((char*)Temp, MemSize);
                        uchar * TempM = NULL;
                        uchar * TempX = NULL;
                        int Idx = Core->BundleIndex;
                        Core->MemBuffer_->PrepareBuf(Idx);
                        TempM = Core->MemBuffer_->BufRaw[Idx];
                        TempX = Core->MemBuffer_->BufOpW[Idx];
                        for (int I = 0; I < MemSize; I++)
                        {
                            TempM[MemAddr + I] = Temp[I];
                            TempX[MemAddr + I] = 1;
                        }
                        DELARR(uchar, Temp);
                        FS.close();
                    }
                }
            }
        }
    }
}

void MainWindow::on_MemFileSave_clicked()
{
    string FileName = "";
    QString FileNameX = QFileDialog::getSaveFileName(this, "Save image from memory", Core->LastPath, "Files (*.*)");
    Core->SaveLastPath(FileNameX, false);
    if (!FileNameX.isEmpty())
    {
        FileName = Eden::ToStr(FileNameX);
    }

    if (FileName != "")
    {
        int MemAddr = Eden::ToInt(Core->InputBox(this, "Address", "Save to file", "0"));
        int MemSize = Eden::ToInt(Core->InputBox(this, "Size", "Save to file", "65536"));
        if ((MemAddr >= 0) && ((MemAddr + MemSize) <= 65536) && (MemSize > 0))
        {
            fstream FS(FileName, ios::binary | ios::out);
            if (FS.is_open())
            {
                uchar * Temp = NEWARR(uchar, uchar[MemSize]);
                uchar * TempM = NULL;
                uchar * TempX = NULL;
                int Idx = Core->BundleIndex;

                Core->MemBuffer_->PrepareBuf(Idx);
                TempM = Core->MemBuffer_->BufRaw[Idx];
                TempX = Core->MemBuffer_->BufOpR[Idx];
                for (int I = 0; I < MemSize; I++)
                {
                    Temp[I] = TempM[MemAddr + I];
                    TempX[MemAddr + I] = 1;
                }
                FS.write((char*)Temp, MemSize);
                DELARR(uchar, Temp);
                FS.close();
            }
        }
    }
}

void MainWindow::on_BundleNew_clicked()
{
    Core->BundleFileName = "";
    for (int I = 0; I < Core->BundleCount; I++)
    {
        Core->Bundle[I]->Init();
    }
    BundleRefresh();
}

void MainWindow::on_BundleOpen_clicked()
{
    string FileName = "";
    QString X = BundFileExt;
    QString FileNameX = QFileDialog::getOpenFileName(this, "Open bundle", Core->LastPath, "Files (*." + X + ")");
    Core->SaveLastPath(FileNameX, false);
    if (!FileNameX.isEmpty())
    {
        FileName = Eden::ToStr(FileNameX);
    }
    if ((FileName != "") && Eden::FileExists(FileName))
    {
        Core->BundleFileName = FileName;
        for (int I = 0; I < Core->BundleCount; I++)
        {
            Core->Bundle[I]->Init();
        }

        EdenClass::ConfigFile Sch;
        Sch.FileLoad(FileName);
        int I = Sch.ParamGetI("Number");
        if (I > Core->BundleCount)
        {
            I = Core->BundleCount;
        }

        int Idx = Core->BundleIndex;
        for (int I = 0; I < Core->BundleCount; I++)
        {
            Core->Bundle[I]->BundleFile = Sch.ParamGetS("File" + Eden::ToStr(I));
            Core->Bundle[I]->BundleDesc = Sch.ParamGetS("Desc" + Eden::ToStr(I));
            Core->BundleIndex = I;
            ProjectOpen(Core->Bundle[I]->BundleFile, false, true);
        }
        Core->BundleIndex = Idx;
        BundleItemGet();
        BundleRefresh();
    }
}

void MainWindow::on_BundleSave_clicked()
{
    if (Core->BundleFileName == "")
    {
        on_BundleSaveAs_clicked();
    }
    else
    {
        string FileName = Core->BundleFileName;
        EdenClass::ConfigFile Sch;
        Sch.ParamClear();
        Sch.ParamSet("Number", Core->BundleCount);
        for (int I = 0; I < Core->BundleCount; I++)
        {
            Sch.ParamSet("File" + Eden::ToStr(I), Core->Bundle[I]->BundleFile);
            Sch.ParamSet("Desc" + Eden::ToStr(I), Core->Bundle[I]->BundleDesc);
        }
        Sch.FileSave(FileName);
        BundleItemGet();
    }
}

void MainWindow::on_BundleSaveAs_clicked()
{
    string FileName = "";
    QString X = BundFileExt;
    QString FileNameX = QFileDialog::getSaveFileName(this, "Save program", Core->LastPath, "Files (*." + X + ")");
    Core->SaveLastPath(FileNameX, false);
    if (!FileNameX.isEmpty())
    {
        FileName = Eden::ToStr(FileNameX);
        FileName = Eden::FileExtension(FileName, BundFileExt);
        Core->BundleFileName = FileName;
        on_BundleSave_clicked();
    }
}

void MainWindow::on_BundleItemUp_clicked()
{
    int Idx = Core->BundleIndex;
    if (Idx > 0)
    {
        ProjectItem * BundleTemp = Core->Bundle[Idx];
        Core->Bundle[Idx] = Core->Bundle[Idx - 1];
        Core->Bundle[Idx - 1] = BundleTemp;
        BundleRefresh();
        ui->BundleList->setCurrentRow(Idx - 1);
    }
}

void MainWindow::on_BundleItemDown_clicked()
{
    int Idx = Core->BundleIndex;
    if (Idx < 255)
    {
        ProjectItem * BundleTemp = Core->Bundle[Idx];
        Core->Bundle[Idx] = Core->Bundle[Idx + 1];
        Core->Bundle[Idx + 1] = BundleTemp;
        BundleRefresh();
        ui->BundleList->setCurrentRow(Idx + 1);
    }
}

void MainWindow::on_BundleList_currentRowChanged(int currentRow)
{
    Core->BundleIndex = currentRow;
    BundleItemGet();
}

void MainWindow::on_BundleItemDescT_textEdited(const QString &arg1)
{
    Core->Bundle[Core->BundleIndex]->BundleDesc = Eden::ToStr(arg1);
    BundleRefresh();
}

void MainWindow::on_SetLibDirT_textChanged()
{
    if (SetEventEnabled)
    {
        Core->Bundle[Core->BundleIndex]->LibDir = Eden::ToStr(ui->SetLibDirT->toPlainText());
    }
}
