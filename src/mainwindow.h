#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "appcore.h"
#include <iostream>
#include <thread>
#include <QTimer>
#include "ioconsolewin.h"
#include "iospreadsheetwin.h"
#include "iographwin.h"
#include <vector>
#include <QCloseEvent>
#include "configfile.h"
#include <QFileDialog>

#define AppWindowTitle "ScriptSDCC 3.0"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    llong CommandCounterF = 1;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    AppCore * Core;
    vector<IOConsoleWin*> IOConsoleWinX;
    vector<IOSpreadsheetWin*> IOSpreadsheetWinX;
    vector<IOGraphWin*> IOGraphWinX;
    void closeEvent(QCloseEvent *event);

    vector<int> SpreadSheetN;
    vector<int> SpreadSheetR;
    vector<int> SpreadSheetC;

    vector<string> CompileSchemeName;
    vector<int> CompileSchemeEngine1;
    vector<int> CompileSchemeEngine2;
    vector<string> CompileSchemeCommand;

    vector<string> ProjectListName;
    vector<string> ProjectListFile;

    void ProgLoadCode(bool CompileCode);

private slots:
    void TimerEvent();

    void on_BtnCompile_clicked();

    void on_BtnRun_clicked();

    void on_BtnReset_clicked();

    void on_BtnAbort_clicked();

    void on_BtnConsole_clicked();

    void on_BtnSpreadsheet_clicked();

    void on_BtnGraph_clicked();

    void on_MemMapHoriI_clicked();

    void on_MemMapHoriO_clicked();

    void on_MemMapVertI_clicked();

    void on_MemMapVertO_clicked();

    void on_MemMapHScroll_valueChanged(int value);

    void on_MemMapVScroll_valueChanged(int value);

    void on_MemMapDispCode_clicked();

    void on_MemMapDispData_clicked();

    void on_ProjNew_clicked();

    void on_ProjOpen_clicked();

    void on_ProjSave_clicked();

    void on_ProjSaveAs_clicked();

    void on_CompileSchemeAdd_clicked();

    void on_CompileSchemeRem_clicked();

    void on_CompileSchemeT_currentIndexChanged(int index);

    void GuiEvt();

    void on_ViewCommandB_clicked();

    void on_ProjListAdd_clicked();

    void on_ProjListRem_clicked();

    void on_ProjListT_currentIndexChanged(int index);

    void on_SetProgCommandT_textEdited(const QString &arg1);

    void on_SetTempDirT_textEdited(const QString &arg1);

    void on_SetConsFontN_textEdited(const QString &arg1);

    void on_SetConsFontS_valueChanged(int arg1);

    void on_SetSheetFontN_textEdited(const QString &arg1);

    void on_SetSheetFontS_valueChanged(int arg1);

    void CoreInvoke(uchar Idx, uchar Param1, uchar Param2);

    void on_MemMapDispBuff_clicked();

    void on_MemFileLoad_clicked();

    void on_MemFileSave_clicked();

    void on_BundleNew_clicked();

    void on_BundleOpen_clicked();

    void on_BundleSave_clicked();

    void on_BundleSaveAs_clicked();

    void on_BundleItemUp_clicked();

    void on_BundleItemDown_clicked();

    void on_BundleList_currentRowChanged(int currentRow);

    void on_BundleItemDescT_textEdited(const QString &arg1);

    void on_SetLibDirT_textChanged();

signals:
    void GuiEvtS();

private:
    bool TimerEventEnabled = false;
    Ui::MainWindow *ui;
    QTimer Timer;

    llong LastInstrCount = 0;

    int ExecStart(int Idx);
    void ExecThr(int Idx);
    void CompileSchemeRefresh();
    void SchemeSort();
    void ProjectSort();

    void ProjectOpen(string FileName, bool Template, bool NoRefresh);
    void ProjectSave(string FileName);
    bool SetEventEnabled = false;
    void BundleRefresh();

    void BundleItemGet();
    void BundleItemSet();
};

#endif // MAINWINDOW_H
