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

#define AppWindowTitle "ScriptSDCC 1.0"

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

    string CurrentFileName = "";

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

signals:
    void GuiEvtS();

private:
    bool BtnCompile = true;
    bool BtnReset = false;
    bool BtnRun = false;
    bool BtnAbort = false;

    bool TimerEventEnabled = false;
    Ui::MainWindow *ui;
    QTimer Timer;

    llong LastInstrCount = 0;

    void ExecThr();
    void CompileSchemeRefresh();
    void SchemeSort();
    void ProjectSort();

    void ProjectOpen(string FileName, bool Template);
    void ProjectSave(string FileName);
    QString LastPath = "";
    void SaveLastPath(QString X, bool OpenDir);
};

#endif // MAINWINDOW_H
