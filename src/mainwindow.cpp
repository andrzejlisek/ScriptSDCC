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

    Sch.ParamGet("LibDir", Core->LibDir);
    Sch.ParamGet("TempDir", Core->TempDir);
    Sch.ParamGet("ProgCmd", Core->ProgCmd);
    Sch.ParamGet("TimerInterval", TimerInterval);

    CommandCounterF = 100000 / TimerInterval;

    Core->LibDir = Eden::CorrectDir(Core->LibDir);
    Core->TempDir = Eden::CorrectDir(Core->TempDir);

    ui->SetLibDirT->setText(Eden::ToQStr(Core->LibDir));
    ui->SetTempDirT->setText(Eden::ToQStr(Core->TempDir));
    ui->SetProgCommandT->setText(Eden::ToQStr(Core->ProgCmd));
    ui->SetTimerT->setValue(TimerInterval);
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
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    TimerEventEnabled = false;
    Core->ProgAbort();
    Timer.stop();
    event++;
    event--;

    int N = CompileSchemeName.size();
    EdenClass::ConfigFile Sch;
    Sch.ParamClear();
    Sch.ParamSet("LibDir", Eden::ToStr(ui->SetLibDirT->text()));
    Sch.ParamSet("TempDir", Eden::ToStr(ui->SetTempDirT->text()));
    Sch.ParamSet("ProgCmd", Eden::ToStr(ui->SetProgCommandT->text()));
    Sch.ParamSet("TimerInterval", ui->SetTimerT->value());


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
        IOConsoleWinX[I]->close();
        DEL(IOConsoleWin, IOConsoleWinX[I]);
    }
    L = IOSpreadsheetWinX.size();
    for (I = 0; I < L; I++)
    {
        IOSpreadsheetWinX[I]->close();
        DEL(IOSpreadsheetWin, IOSpreadsheetWinX[I]);
    }
    L = IOGraphWinX.size();
    for (I = 0; I < L; I++)
    {
        IOGraphWinX[I]->close();
        DEL(IOGraphWin, IOGraphWinX[I]);
    }
    disconnect(&Timer, SIGNAL(timeout()), this, SLOT(TimerEvent()));
    DEL(AppCore, Core);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_BtnCompile_clicked()
{
    Core->Engine = ui->EngineT1->currentIndex();
    Core->MemMode = ui->EngineT2->currentIndex();
    Core->CompileCommand = Eden::ToStr(ui->CommandT->text());
    Core->ProgFileSrc = Eden::ToStr(ui->FileNameT1->text());
    Core->ProgFileBin = Eden::ToStr(ui->FileNameT2->text());
    Core->SwapPage = Eden::HexToInt(Eden::ToStr(ui->MemSwapT->text()));
    Core->CodeLoc = Eden::HexToInt(Eden::ToStr(ui->MemCodeT1->text()));
    Core->CodeSize = Eden::HexToInt(Eden::ToStr(ui->MemCodeT2->text()));
    Core->DataLoc = Eden::HexToInt(Eden::ToStr(ui->MemDataT1->text()));
    Core->DataSize = Eden::HexToInt(Eden::ToStr(ui->MemDataT2->text()));
    ui->CompileMsgT->setPlainText(Eden::ToQStr(Core->ProgCompile(Eden::ToStr(ui->LibraryT->text()))));

    if (Core->CompileGood)
    {
        BtnRun = true;
        BtnReset = true;
    }
    else
    {
        BtnRun = false;
        BtnReset = false;
    }
    GuiEvt();
}

void MainWindow::on_BtnRun_clicked()
{
    BtnCompile = false;
    BtnRun = false;
    BtnReset = false;
    BtnAbort = true;
    GuiEvt();

    std::thread Thr(&MainWindow::ExecThr, this);
    Thr.detach();
}

void MainWindow::ExecThr()
{
    Core->ProgRun();

    BtnCompile = true;
    BtnRun = true;
    BtnReset = true;
    BtnAbort = false;
    emit GuiEvtS();
}

void MainWindow::on_BtnReset_clicked()
{
    Core->ProgReset();
}

void MainWindow::on_BtnAbort_clicked()
{
    BtnAbort = false;
    GuiEvt();
    Core->ProgAbort();
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
                IOGraphWinX[I]->Refresh();
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
    Core->IOGraph_[0]->NeedRedraw = false;
    Core->IOGraph_[1]->NeedRedraw = false;
    Core->IOGraph_[2]->NeedRedraw = false;
    Core->IOGraph_[3]->NeedRedraw = false;

    if (ui->ProjectsTabs->currentIndex() == 2)
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
    }
}

void MainWindow::on_BtnConsole_clicked()
{
    IOConsoleWin * IOConsoleWin_ = NEW(IOConsoleWin, IOConsoleWin());
    IOConsoleWin_->Core = Core;
    IOConsoleWin_->show();
    IOConsoleWin_->Refresh();
    IOConsoleWinX.push_back(IOConsoleWin_);
}

void MainWindow::on_BtnSpreadsheet_clicked()
{
    IOSpreadsheetWin * IOSpreadsheetWin_ = NEW(IOSpreadsheetWin, IOSpreadsheetWin());
    IOSpreadsheetWin_->Core = Core;
    IOSpreadsheetWin_->show();
    IOSpreadsheetWin_->Refresh();
    IOSpreadsheetWinX.push_back(IOSpreadsheetWin_);
}

void MainWindow::on_BtnGraph_clicked()
{
    IOGraphWin * IOGraphWin_ = NEW(IOGraphWin, IOGraphWin());
    IOGraphWin_->Core = Core;
    IOGraphWin_->show();
    IOGraphWin_->Refresh();
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
    Core->MemMapData = false;
}

void MainWindow::on_MemMapDispData_clicked()
{
    Core->MemMapData = true;
}

void MainWindow::ProjectOpen(string FileName, bool Template)
{
    if ((FileName != "") && Eden::FileExists(FileName))
    {
        if (!Template)
        {
            CurrentFileName = FileName;
            string X = AppWindowTitle;
            setWindowTitle(Eden::ToQStr(X + " - " + CurrentFileName));
        }

        EdenClass::ConfigFile CF;
        CF.FileLoad(FileName);
        ui->ViewCommandT->setText(Eden::ToQStr(CF.ParamGetS("ViewCommand")));
        ui->FileNameT1->setText(Eden::ToQStr(CF.ParamGetS("FileNameSource")));
        ui->FileNameT2->setText(Eden::ToQStr(CF.ParamGetS("FileNameBinary")));
        ui->EngineT1->setCurrentIndex(CF.ParamGetI("Engine"));
        ui->EngineT2->setCurrentIndex(CF.ParamGetI("Memory"));
        ui->CommandT->setText(Eden::ToQStr(CF.ParamGetS("Command")));
        ui->MemCodeT1->setText(Eden::ToQStr(CF.ParamGetS("MemCodeLoc")));
        ui->MemCodeT2->setText(Eden::ToQStr(CF.ParamGetS("MemCodeSize")));
        ui->MemDataT1->setText(Eden::ToQStr(CF.ParamGetS("MemDataLoc")));
        ui->MemDataT2->setText(Eden::ToQStr(CF.ParamGetS("MemDataSize")));
        ui->MemSwapT->setText(Eden::ToQStr(CF.ParamGetS("MemSwapLoc")));
        ui->LibraryT->setText(Eden::ToQStr(CF.ParamGetS("LibraryFiles")));
        ui->DescrT->setPlainText(Eden::ToQStr(Eden::MultilineDecode(CF.ParamGetS("Description"))));
        ui->CompileSchemeT->setCurrentText("");
    }
}

void MainWindow::ProjectSave(string FileName)
{
    if (FileName != "")
    {
        EdenClass::ConfigFile CF;
        CF.ParamClear();
        CF.ParamSet("ViewCommand", Eden::ToStr(ui->ViewCommandT->text()));
        CF.ParamSet("FileNameSource", Eden::ToStr(ui->FileNameT1->text()));
        CF.ParamSet("FileNameBinary", Eden::ToStr(ui->FileNameT2->text()));
        CF.ParamSet("Engine", ui->EngineT1->currentIndex());
        CF.ParamSet("Memory", ui->EngineT2->currentIndex());
        CF.ParamSet("Command", Eden::ToStr(ui->CommandT->text()));
        CF.ParamSet("MemCodeLoc", Eden::ToStr(ui->MemCodeT1->text()));
        CF.ParamSet("MemCodeSize", Eden::ToStr(ui->MemCodeT2->text()));
        CF.ParamSet("MemDataLoc", Eden::ToStr(ui->MemDataT1->text()));
        CF.ParamSet("MemDataSize", Eden::ToStr(ui->MemDataT2->text()));
        CF.ParamSet("MemSwapLoc", Eden::ToStr(ui->MemSwapT->text()));
        CF.ParamSet("LibraryFiles", Eden::ToStr(ui->LibraryT->text()));
        CF.ParamSet("Description", Eden::MultilineEncode(Eden::ToStr(ui->DescrT->toPlainText())));
        CF.FileSave(FileName);
    }
}

void MainWindow::on_ProjNew_clicked()
{
    CurrentFileName = "";
    setWindowTitle(Eden::ToQStr(AppWindowTitle));

    ui->ViewCommandT->setText("");
    ui->FileNameT1->setText("");
    ui->FileNameT2->setText("");
    ui->EngineT1->setCurrentIndex(0);
    ui->EngineT2->setCurrentIndex(0);
    ui->CommandT->setText("");
    ui->MemCodeT1->setText("");
    ui->MemCodeT2->setText("");
    ui->MemDataT1->setText("");
    ui->MemDataT2->setText("");
    ui->MemSwapT->setText("");
    ui->LibraryT->setText("");
    ui->DescrT->setPlainText("");
    ProjectOpen(Eden::ApplicationDirectory() + "default.txt", true);
    if (CompileSchemeName.size() > 0)
    {
        ui->CompileSchemeT->setCurrentIndex(0);
        on_CompileSchemeT_currentIndexChanged(0);
    }
    else
    {
        ui->CompileSchemeT->setCurrentText("");
    }
}

void MainWindow::on_ProjOpen_clicked()
{
    string FileName = "";
    if (Eden::FileExists(Eden::ApplicationDirectory() + "testprog.txt"))
    {
        FileName = Eden::ApplicationDirectory() + "testprog.txt";
    }
    else
    {
        QString FileNameX = QFileDialog::getOpenFileName(this, "Open program", LastPath, "Files (*)");
        SaveLastPath(FileNameX, false);
        if (!FileNameX.isEmpty())
        {
            FileName = Eden::ToStr(FileNameX);
        }
    }
    ProjectOpen(FileName, false);
}

void MainWindow::on_ProjSave_clicked()
{
    if (CurrentFileName == "")
    {
        on_ProjSaveAs_clicked();
        return;
    }

    ProjectSave(CurrentFileName);
}

void MainWindow::on_ProjSaveAs_clicked()
{
    string FileName = "";
    if (Eden::FileExists(Eden::ApplicationDirectory() + "testprog.txt"))
    {
        FileName = Eden::ApplicationDirectory() + "testprog.txt";
    }
    else
    {
        QString FileNameX = QFileDialog::getSaveFileName(this, "Save program", LastPath, "Files (*)");
        SaveLastPath(FileNameX, false);
        if (!FileNameX.isEmpty())
        {
            FileName = Eden::ToStr(FileNameX);
        }
    }

    if (FileName != "")
    {
        CurrentFileName = FileName;
        string X = AppWindowTitle;
        setWindowTitle(Eden::ToQStr(X + " - " + CurrentFileName));
        ProjectSave(FileName);
    }
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
    ui->BtnCompile->setEnabled(BtnCompile);
    ui->BtnReset->setEnabled(BtnReset);
    ui->BtnRun->setEnabled(BtnRun);
    ui->BtnAbort->setEnabled(BtnAbort);
}

void MainWindow::SaveLastPath(QString X, bool OpenDir)
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

void MainWindow::on_ViewCommandB_clicked()
{
    string X = Eden::ToStr(ui->ViewCommandT->text());
    system(X.c_str());
}

void MainWindow::on_ProjListAdd_clicked()
{
    string ProjName = Eden::ToStr(ui->ProjListT->currentText());
    if ((CurrentFileName != "") && (ProjName != ""))
    {
        for (uint I = 0; I < ProjectListName.size(); I++)
        {
            if (ProjectListName[I] == ProjName)
            {
                ProjectListFile[I] = CurrentFileName;
                return;
            }
        }
        ProjectListName.push_back(ProjName);
        ProjectListFile.push_back(CurrentFileName);
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
        ProjectOpen(ProjectListFile[index - 1], false);
    }
}
