#ifndef IOGRAPHWIN_H
#define IOGRAPHWIN_H

#include <QDialog>
#include "eden.h"
#include "appcore.h"
#include <vector>
#include <QColorDialog>

#define DimensionType double
#define MaxPlotWidth 20

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
    void Refresh();
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

    void DrawPoint(int X, int Y, int Z, uchar R, uchar G, uchar B);
    void DrawPointH(int X, int Y, int Z, uchar R, uchar G, uchar B);
    void DrawPointV(int X, int Y, int Z, uchar R, uchar G, uchar B);
    void DrawLine(int X0, int Y0, int Z0, int X1, int Y1, int Z1, uchar R, uchar G, uchar B);
    void DrawLine0(int X0, int Y0, int Z0, int X1, int Y1, int Z1, int R0, int G0, int B0, int R1, int G1, int B1);

    int PlotSizeP = 0;
    int PlotSizeL = 0;
    int BoundLP, BoundRP, BoundTP, BoundBP, BoundLL, BoundRL, BoundTL, BoundBL;

private slots:
    void on_Show0_clicked();

    void on_Show1_clicked();

    void on_Show2_clicked();

    void on_Show3_clicked();

    void on_GraphClear_clicked();

    void on_GraphColor_clicked();

    void on_GraphPlotSizeP_valueChanged(int arg1);

    void on_GraphPlotSizeL_valueChanged(int arg1);

    void on_GrScaleXX_textChanged(const QString &arg1);

    void on_GrScaleXY_textChanged(const QString &arg1);

    void on_GrScaleXZ_textChanged(const QString &arg1);

    void on_GrScaleYX_textChanged(const QString &arg1);

    void on_GrScaleYY_textChanged(const QString &arg1);

    void on_GrScaleYZ_textChanged(const QString &arg1);

    void on_GrScaleZX_textChanged(const QString &arg1);

    void on_GrScaleZY_textChanged(const QString &arg1);

    void on_GrScaleZZ_textChanged(const QString &arg1);

    void on_GrScaleX_textChanged(const QString &arg1);

    void on_GrScaleY_textChanged(const QString &arg1);

    void on_GrScaleZ_textChanged(const QString &arg1);

    void on_GrOffset1X_textChanged(const QString &arg1);

    void on_GrOffset1Y_textChanged(const QString &arg1);

    void on_GrOffset1Z_textChanged(const QString &arg1);

    void on_GrOffset2X_textChanged(const QString &arg1);

    void on_GrOffset2Y_textChanged(const QString &arg1);

    void on_GrOffset2Z_textChanged(const QString &arg1);

private:
    Ui::IOGraphWin *ui;
    int CurrentIO = 0;
    int CurrentIOX = -1;
    uchar MaskAll[MaxPlotWidth + 1][MaxPlotWidth + 1][MaxPlotWidth + 1];
    uchar Mask[MaxPlotWidth + 1][MaxPlotWidth + 1];
    uchar BackColorR;
    uchar BackColorG;
    uchar BackColorB;

    void RefreshParams();
    int RefreshNum = -1;
};

#endif // IOGRAPHWIN_H
