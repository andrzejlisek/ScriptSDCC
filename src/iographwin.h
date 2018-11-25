#ifndef IOGRAPHWIN_H
#define IOGRAPHWIN_H

#include <QDialog>
#include "eden.h"
#include "appcore.h"
#include <vector>
#include <QColorDialog>
#include <QFileDialog>

#define DimensionType double
#define MaxPlotWidth 25
#define Det3(X11, X12, X13, X21, X22, X23, X31, X32, X33) ((X11)*(X22)*(X33) + (X21)*(X32)*(X13) + (X31)*(X12)*(X23) - (X21)*(X12)*(X33) - (X11)*(X32)*(X23) - (X31)*(X22)*(X13))
#define VectorAngle(A1, B1, C1, A2, B2, C2) abs(acos((((A1) * (A2)) + ((B1) * (B2)) + ((C1 * C2))) / (sqrt(((A1) * (A1)) + ((B1) * (B1)) + ((C1) * (C1))) * sqrt(((A2) * (A2)) + ((B2) * (B2)) + ((C2) * (C2))))))

namespace Ui {
class IOGraphWin;
}

class IOGraphWin : public QDialog
{
    Q_OBJECT

public:
    explicit IOGraphWin(QWidget *parent = 0);
    ~IOGraphWin();
    AppCore * Core;
    void Refresh(bool ToFile, string FileName, int ScrW, int ScrH);
    int ImgW;
    int ImgH;
    int ImgW1;
    int ImgH1;
    int ImgWH;
    QImage * Img;
    uchar * ImgRaw;
    int * ImgZ;

    int ZeroX = 0;
    int ZeroY = 0;
    int ZeroZ = 0;

    DimensionType OffsetX = 0;
    DimensionType OffsetY= 0;
    DimensionType OffsetZ = 0;

    int OffsetXRender = 0;
    int OffsetYRender = 0;
    int OffsetZRender = 0;

    DimensionType DrawXtoX = 0;
    DimensionType DrawXtoY = 0;
    DimensionType DrawXtoZ = 0;

    DimensionType DrawYtoX = 0;
    DimensionType DrawYtoY = 0;
    DimensionType DrawYtoZ = 0;

    DimensionType DrawZtoX = 0;
    DimensionType DrawZtoY = 0;
    DimensionType DrawZtoZ = 0;

    DimensionType Angle1X = 0;
    DimensionType Angle1Y = 0;
    DimensionType Angle1Z = 0;

    DimensionType Angle2X = 0;
    DimensionType Angle2Y = 0;
    DimensionType Angle2Z = 0;

    double LightDistX = 0;
    double LightDistY = 0;
    double LightDistZ = 0;
    double LightDistXYZ = 0;
    void CalcColorInit();
    void CalcColor(uchar &R, uchar &G, uchar &B, int X, int Y, int Z, double LightAngle);

    void DrawPoint(int X, int Y, int Z, uchar R, uchar G, uchar B, double LightAngle);
    void DrawPointT(int X, int Y, int Z, uchar R, uchar G, uchar B);
    void DrawPoint0(int X, int Y, int Z, uchar R, uchar G, uchar B);
    void DrawPointH(int X, int Y, int Z, uchar R, uchar G, uchar B, double LightAngle);
    void DrawPointV(int X, int Y, int Z, uchar R, uchar G, uchar B, double LightAngle);
    void DrawPointD1(int X, int Y, int Z, uchar R, uchar G, uchar B, double LightAngle);
    void DrawPointD2(int X, int Y, int Z, uchar R, uchar G, uchar B, double LightAngle);
    void DrawLine(int X0, int Y0, int Z0, int X1, int Y1, int Z1, uchar R, uchar G, uchar B, double LightAngle);
    void DrawLine0(int X0, int Y0, int Z0, int X1, int Y1, int Z1, int R0, int G0, int B0, int R1, int G1, int B1, double LightAngle);

    void CalcTriangleAngle(int X1, int Y1, int Z1, int X2, int Y2, int Z2, int X3, int Y3, int Z3, double &LightAngle);

    void DrawHalfTriangle(int X0, int Y0, int Z0, int X1, int X2, int Y_, int Z1, int Z2, uchar R0, uchar G0, uchar B0, double LightAngle);
    void DrawTriangle(int X1, int Y1, int Z1, int X2, int Y2, int Z2, int X3, int Y3, int Z3, uchar R0, uchar G0, uchar B0, double LightAngle);
    void DrawHalfTriangle0(int X0, int Y0, int Z0, int X1, int X2, int Y_, int Z1, int Z2, uchar R0, uchar G0, uchar B0, uchar R1, uchar G1, uchar B1, uchar R2, uchar G2, uchar B2, double LightAngle);
    void DrawTriangle0(int X1, int Y1, int Z1, int X2, int Y2, int Z2, int X3, int Y3, int Z3, uchar R1, uchar G1, uchar B1, uchar R2, uchar G2, uchar B2, uchar R3, uchar G3, uchar B3, double LightAngle);

    int PlotSizeP = 0;
    bool PlotPointSquare = false;
    int PlotSizeL = 0;
    bool PlotLineCross = false;
    int PlotSizeP2 = 0;
    int PlotSizeL2 = 0;
    bool PlotSurface = true;
    bool PlotText = true;
    int BoundLP, BoundRP, BoundTP, BoundBP, BoundLL, BoundRL, BoundTL, BoundBL;
    int PlotSizeDiagonal1, PlotSizeDiagonal2, PlotSizeDiagonal3, PlotSizeDiagonal4;

    int LightMode = 0;
    int LightAngleH = 0;
    int LightAngleV = 0;
    int LightDistance0 = 0;
    int LightDistance1 = 0;
    int LightDistance2 = 0;
    double LightAngleHradP = 0;
    double LightAngleVradP = 0;
    double LightAngleHradN = 0;
    double LightAngleVradN = 0;
    double LightAngle_rad = 0;
    double LightDistance0P_ = 0;
    double LightDistance0N_ = 0;
    double LightDistance1_ = 0;
    double LightDistance2_ = 0;

    double LightVectorX = 0;
    double LightVectorY = 0;
    double LightVectorZ = 0;

    uchar Light1R = 0;
    uchar Light1G = 0;
    uchar Light1B = 0;
    uchar Light2R = 0;
    uchar Light2G = 0;
    uchar Light2B = 0;
    double Light1R_ = 0;
    double Light1G_ = 0;
    double Light1B_ = 0;
    double Light2R_ = 0;
    double Light2G_ = 0;
    double Light2B_ = 0;

private slots:
    void on_Show0_clicked();

    void on_Show1_clicked();

    void on_Show2_clicked();

    void on_Show3_clicked();

    void on_GraphClear_clicked();

    void on_GraphColor_clicked();

    void on_GraphPlotSizeP_valueChanged(int arg1);

    void on_GraphPlotSizeL_valueChanged(int arg1);

    void on_GrScaleXX_valueChanged(int arg1);

    void on_GrScaleXY_valueChanged(int arg1);

    void on_GrScaleXZ_valueChanged(int arg1);

    void on_GrScaleYX_valueChanged(int arg1);

    void on_GrScaleYY_valueChanged(int arg1);

    void on_GrScaleYZ_valueChanged(int arg1);

    void on_GrScaleZX_valueChanged(int arg1);

    void on_GrScaleZY_valueChanged(int arg1);

    void on_GrScaleZZ_valueChanged(int arg1);

    void on_GrScaleX_valueChanged(int arg1);

    void on_GrScaleY_valueChanged(int arg1);

    void on_GrScaleZ_valueChanged(int arg1);

    void on_GrOffset1X_valueChanged(int arg1);

    void on_GrOffset1Y_valueChanged(int arg1);

    void on_GrOffset1Z_valueChanged(int arg1);

    void on_GrOffset2X_valueChanged(int arg1);

    void on_GrOffset2Y_valueChanged(int arg1);

    void on_GrOffset2Z_valueChanged(int arg1);

    void on_Rot1X_valueChanged(int arg1);

    void on_Rot1Y_valueChanged(int arg1);

    void on_Rot1Z_valueChanged(int arg1);

    void on_Rot2X_valueChanged(int arg1);

    void on_Rot2Y_valueChanged(int arg1);

    void on_Rot2Z_valueChanged(int arg1);

    void on_ValueStepT_valueChanged(int arg1);

    void on_Rot1Order_currentIndexChanged(int index);

    void on_Rot2Order_currentIndexChanged(int index);

    void on_RenderFile_clicked();

    void on_GraphPlotSurface_toggled(bool checked);

    void on_LightMode_currentIndexChanged(int index);

    void on_LightAngleVT_valueChanged(int arg1);

    void on_LightAngleHT_valueChanged(int arg1);

    void on_LightDist0T_valueChanged(int arg1);

    void on_LightDist1T_valueChanged(int arg1);

    void on_LightDist2T_valueChanged(int arg1);

    void on_GraphInv_toggled(bool checked);

    void on_ShowHide1_toggled(bool checked);

    void on_ShowHide2_toggled(bool checked);

    void on_ShowHide3_toggled(bool checked);

    void on_ShowHide4_toggled(bool checked);

    void on_GraphPlotText_toggled(bool checked);

private:
    bool InverseDraw = false;
    Ui::IOGraphWin *ui;
    int CurrentIO = 0;
    int CurrentIOX = -1;
    uchar MaskAll[MaxPlotWidth + 1][MaxPlotWidth + 1][MaxPlotWidth + 1];
    uchar Mask[MaxPlotWidth + 1][MaxPlotWidth + 1];
    uchar BackColorR;
    uchar BackColorG;
    uchar BackColorB;
    uchar BackColorRI;
    uchar BackColorGI;
    uchar BackColorBI;

    void RefreshParams(int Dummy);
    int RefreshNum = -1;
    void RotateScale(double &Angle, double &ScaleX, double &ScaleY);
    bool RefreshEnabled = true;
};

#endif // IOGRAPHWIN_H
