#include "iographwin.h"
#include "ui_iographwin.h"

IOGraphWin::IOGraphWin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IOGraphWin)
{
    BackColorR = 0;
    BackColorG = 0;
    BackColorB = 0;
    BackColorRI = 255;
    BackColorGI = 255;
    BackColorBI = 255;

    for (int I = 0; I <= MaxPlotWidth; I++)
    {
        for (int Y = 0; Y <= MaxPlotWidth; Y++)
        {
            for (int X = 0; X <= MaxPlotWidth; X++)
            {
                MaskAll[I][X][Y] = (((X * X) + (Y * Y)) <= (I * I)) ? 1 : 0;
            }
        }
    }

    ImgW = 0;
    ImgH = 0;
    Img = NULL;
    ImgRaw = NULL;
    ImgZ = NULL;

    Qt::WindowFlags flags = 0;
    flags |= Qt::WindowMaximizeButtonHint;
    //flags |= Qt::WindowContextHelpButtonHint;
    flags |= Qt::WindowCloseButtonHint;
    //flags |= Qt::WindowStaysOnTopHint;
    flags |= Qt::WindowMinimizeButtonHint;
    setWindowFlags(flags);
    ui->setupUi(this);
    RefreshParams(0);
}

IOGraphWin::~IOGraphWin()
{
    if (Img != NULL)
    {
        DEL(QImage, Img);
    }
    delete ui;
}

void IOGraphWin::on_Show0_clicked()
{
    setWindowTitle("Graph 0");
    CurrentIOX = -1;
    CurrentIO = 0;
    Refresh(false, "", 0, 0);
}

void IOGraphWin::on_Show1_clicked()
{
    setWindowTitle("Graph 1");
    CurrentIOX = -1;
    CurrentIO = 1;
    Refresh(false, "", 0, 0);
}

void IOGraphWin::on_Show2_clicked()
{
    setWindowTitle("Graph 2");
    CurrentIOX = -1;
    CurrentIO = 2;
    Refresh(false, "", 0, 0);
}

void IOGraphWin::on_Show3_clicked()
{
    setWindowTitle("Graph 3");
    CurrentIOX = -1;
    CurrentIO = 3;
    Refresh(false, "", 0, 0);
}

void IOGraphWin::CalcColorInit()
{
    LightMode = ui->LightMode->currentIndex();
    LightAngleH = ui->LightAngleHT->value();
    LightAngleV = ui->LightAngleVT->value();
    LightDistance0 = ui->LightDist0T->value();
    LightDistance1 = ui->LightDist1T->value();
    LightDistance2 = ui->LightDist2T->value();
    if (LightDistance1 < 0)
    {
        LightDistance1 = 0 - LightDistance1;
        Light1R = BackColorRI;
        Light1G = BackColorGI;
        Light1B = BackColorBI;
    }
    else
    {
        Light1R = BackColorR;
        Light1G = BackColorG;
        Light1B = BackColorB;
    }
    if (LightDistance2 < 0)
    {
        LightDistance2 = 0 - LightDistance2;
        Light2R = BackColorRI;
        Light2G = BackColorGI;
        Light2B = BackColorBI;
    }
    else
    {
        Light2R = BackColorR;
        Light2G = BackColorG;
        Light2B = BackColorB;
    }
    Light1R_ = Light1R;
    Light1G_ = Light1G;
    Light1B_ = Light1B;
    Light2R_ = Light2R;
    Light2G_ = Light2G;
    Light2B_ = Light2B;

    LightDistance0P_ = LightDistance0;
    LightDistance0N_ = 0 - LightDistance0;
    LightDistance1_ = LightDistance1;
    LightDistance2_ = LightDistance2;

    LightAngleHradP = ((double)(LightAngleH)) * M_PI / 180.0;
    LightAngleHradN = ((double)(0 - LightAngleH)) * M_PI / 180.0;
    LightAngleVradP = ((double)(LightAngleV)) * M_PI / 180.0;
    LightAngleVradN = ((double)(0 - LightAngleV)) * M_PI / 180.0;

    LightDistX = sin(LightAngleHradP) * cos(LightAngleVradP);
    LightDistY = (0.0 - cos(LightAngleHradP)) * cos(LightAngleVradP);
    LightDistZ = sin(LightAngleVradP);
    LightDistXYZ = sqrt((LightDistX * LightDistX) + (LightDistY * LightDistY) + (LightDistZ * LightDistZ));

    LightVectorX = sin(LightAngleHradP) * cos(LightAngleVradP);
    LightVectorY = cos(LightAngleHradP) * cos(LightAngleVradP);
    LightVectorZ = sin(LightAngleVradP);
}

void IOGraphWin::CalcColor(uchar &R, uchar &G, uchar &B, int X, int Y, int Z, double LightAngle)
{
    if (LightMode > 0)
    {
        int R0 = R;
        int G0 = G;
        int B0 = B;
        double Dist1 = 0;
        double Dist2;


        /*R0 = 255;
        G0 = 0;
        B0 = 0;
        Light2R = 0;
        Light2G = 255;
        Light2B = 0;
        Light2R_ = 0;
        Light2G_ = 255;
        Light2B_ = 0;
        Light1R = 0;
        Light1G = 0;
        Light1B = 255;
        Light1R_ = 0;
        Light1G_ = 0;
        Light1B_ = 255;*/



        if (LightMode == 1)
        {
            double DX = X - ZeroX;
            double DY = Y - ZeroY;
            double DZ = Z - ZeroZ;

            Dist1 = ((LightDistX * DX) + (LightDistY * DY) + (LightDistZ * DZ)) / LightDistXYZ;
        }

        if (LightMode == 2)
        {
            double SurfaceAngle = LightAngle * 180.0 / M_PI;
            Dist1 = 0.0 - SurfaceAngle + 90.0;
        }

        Dist2 = 0.0 - Dist1;

        if (LightDistance1 > 0)
        {
            if ((Dist1 > LightDistance0P_) && (Dist1 < (LightDistance1_ + LightDistance0P_)))
            {
                double Prop = (Dist1 - LightDistance0P_) / LightDistance1_;
                double Prop0 = 1.0 - Prop;
                R0 = (Prop0 * ((double)R0)) + (Prop * Light1R_);
                G0 = (Prop0 * ((double)G0)) + (Prop * Light1G_);
                B0 = (Prop0 * ((double)B0)) + (Prop * Light1B_);
            }
            else
            {
                if (Dist1 >= (LightDistance1_ + LightDistance0P_))
                {
                    R0 = Light1R;
                    G0 = Light1G;
                    B0 = Light1B;
                }
            }
        }
        if (LightDistance2 > 0)
        {
            if ((Dist2 > LightDistance0N_) && (Dist2 < (LightDistance2_ + LightDistance0N_)))
            {
                double Prop = (Dist2 - LightDistance0N_) / (LightDistance2_);
                double Prop0 = 1.0 - Prop;
                R0 = (Prop0 * ((double)R0)) + (Prop * Light2R_);
                G0 = (Prop0 * ((double)G0)) + (Prop * Light2G_);
                B0 = (Prop0 * ((double)B0)) + (Prop * Light2B_);
            }
            else
            {
                if (Dist2 >= (LightDistance2_ + LightDistance0N_))
                {
                    R0 = Light2R;
                    G0 = Light2G;
                    B0 = Light2B;
                }
            }
        }

        /*double Dist000 = sqrt(DX * DX + DY * DY + DZ * DZ);
        if ((Dist000 >= 98.0) && (Dist000 <= 102.0))
        {
            R0 = 255;
            G0 = 0;
            B0 = 0;
        }
        else
        {
            R0 = 0;
            G0 = 0;
            B0 = 255;
        }

        if ((Dist1 >= 98.0) && (Dist1 <= 102.0))
        {
            G0 = 255;
        }*/


        R = Range(R0, 0, 255);
        G = Range(G0, 0, 255);
        B = Range(B0, 0, 255);
    }
}

void IOGraphWin::DrawPoint(int X, int Y, int Z, uchar R, uchar G, uchar B, double LightAngle)
{
    int DrawX = X + ZeroX;
    int DrawY = ZeroY - Y;
    int DrawZ = Z + ZeroZ;
    uchar R0, G0, B0;

    if ((DrawX >= BoundLP) && (DrawY >= BoundTP) && (DrawX <= BoundRP) && (DrawY <= BoundBP))
    {
        int Pos = (DrawY * ImgW) + DrawX;
        int XX, YY, PosX, Pos4X;
        if (InverseDraw)
        {
            for (YY = 0; YY <= PlotSizeP; YY++)
            {
                for (XX = 0; XX <= PlotSizeP; XX++)
                {
                    if (Mask[XX][YY])
                    {
                        PosX = Pos + (ImgW * (YY)) + XX;
                        if (ImgZ[PosX] < DrawZ)
                        {
                            R0 = R;
                            G0 = G;
                            B0 = B;
                            CalcColor(R0, G0, B0, DrawX + XX, DrawY + YY, DrawZ, LightAngle);
                            ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                        }

                        PosX = Pos + (ImgW * (YY)) - XX - PlotSizeP2;
                        if (ImgZ[PosX] < DrawZ)
                        {
                            R0 = R;
                            G0 = G;
                            B0 = B;
                            CalcColor(R0, G0, B0, DrawX - XX, DrawY + YY, DrawZ, LightAngle);
                            ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                        }

                        PosX = Pos - (ImgW * (YY + PlotSizeP2)) + XX;
                        if (ImgZ[PosX] < DrawZ)
                        {
                            R0 = R;
                            G0 = G;
                            B0 = B;
                            CalcColor(R0, G0, B0, DrawX + XX, DrawY - YY, DrawZ, LightAngle);
                            ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                        }

                        PosX = Pos - (ImgW * (YY + PlotSizeP2)) - XX - PlotSizeP2;
                        if (ImgZ[PosX] < DrawZ)
                        {
                            R0 = R;
                            G0 = G;
                            B0 = B;
                            CalcColor(R0, G0, B0, DrawX - XX, DrawY - YY, DrawZ, LightAngle);
                            ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                        }
                    }
                }
            }
        }
        else
        {
            for (YY = 0; YY <= PlotSizeP; YY++)
            {
                for (XX = 0; XX <= PlotSizeP; XX++)
                {
                    if (Mask[XX][YY])
                    {
                        PosX = Pos + (ImgW * (YY)) + XX;
                        if (ImgZ[PosX] <= DrawZ)
                        {
                            R0 = R;
                            G0 = G;
                            B0 = B;
                            CalcColor(R0, G0, B0, DrawX + XX, DrawY + YY, DrawZ, LightAngle);
                            ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                        }

                        PosX = Pos + (ImgW * (YY)) - XX - PlotSizeP2;
                        if (ImgZ[PosX] <= DrawZ)
                        {
                            R0 = R;
                            G0 = G;
                            B0 = B;
                            CalcColor(R0, G0, B0, DrawX - XX, DrawY + YY, DrawZ, LightAngle);
                            ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                        }

                        PosX = Pos - (ImgW * (YY + PlotSizeP2)) + XX;
                        if (ImgZ[PosX] <= DrawZ)
                        {
                            R0 = R;
                            G0 = G;
                            B0 = B;
                            CalcColor(R0, G0, B0, DrawX + XX, DrawY - YY, DrawZ, LightAngle);
                            ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                        }

                        PosX = Pos - (ImgW * (YY + PlotSizeP2)) - XX - PlotSizeP2;
                        if (ImgZ[PosX] <= DrawZ)
                        {
                            R0 = R;
                            G0 = G;
                            B0 = B;
                            CalcColor(R0, G0, B0, DrawX - XX, DrawY - YY, DrawZ, LightAngle);
                            ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                        }
                    }
                }
            }
        }
    }
}

void IOGraphWin::DrawPointT(int X, int Y, int Z, uchar R, uchar G, uchar B)
{
    int DrawX = X + ZeroX;
    int DrawY = ZeroY - Y;
    int DrawZ = Z + ZeroZ;
    uchar R0, G0, B0;

    if ((DrawX >= 0) && (DrawY >= 0) && (DrawX <= ImgW1) && (DrawY <= ImgH1))
    {
        int Pos = (DrawY * ImgW) + DrawX;
        int PosX, Pos4X;
        if (InverseDraw)
        {
            PosX = Pos;
            if (ImgZ[PosX] < DrawZ)
            {
                R0 = R;
                G0 = G;
                B0 = B;
                ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
            }
        }
        else
        {
            PosX = Pos;
            if (ImgZ[PosX] <= DrawZ)
            {
                R0 = R;
                G0 = G;
                B0 = B;
                ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
            }
        }
    }
}

void IOGraphWin::DrawPoint0(int X, int Y, int Z, uchar R, uchar G, uchar B)
{
    int DrawX = X + ZeroX;
    int DrawY = ZeroY - Y;
    int DrawZ = Z + ZeroZ;
    uchar R0, G0, B0;

    if ((DrawX >= BoundLP) && (DrawY >= BoundTP) && (DrawX <= BoundRP) && (DrawY <= BoundBP))
    {
        int Pos = (DrawY * ImgW) + DrawX;
        int XX, YY, PosX, Pos4X;
        if (InverseDraw)
        {
            for (YY = 0; YY <= PlotSizeP; YY++)
            {
                for (XX = 0; XX <= PlotSizeP; XX++)
                {
                    if (Mask[XX][YY])
                    {
                        PosX = Pos + (ImgW * (YY)) + XX;
                        if (ImgZ[PosX] < DrawZ)
                        {
                            R0 = R;
                            G0 = G;
                            B0 = B;
                            ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                        }

                        PosX = Pos + (ImgW * (YY)) - XX - PlotSizeP2;
                        if (ImgZ[PosX] < DrawZ)
                        {
                            R0 = R;
                            G0 = G;
                            B0 = B;
                            ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                        }

                        PosX = Pos - (ImgW * (YY + PlotSizeP2)) + XX;
                        if (ImgZ[PosX] < DrawZ)
                        {
                            R0 = R;
                            G0 = G;
                            B0 = B;
                            ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                        }

                        PosX = Pos - (ImgW * (YY + PlotSizeP2)) - XX - PlotSizeP2;
                        if (ImgZ[PosX] < DrawZ)
                        {
                            R0 = R;
                            G0 = G;
                            B0 = B;
                            ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                        }
                    }
                }
            }
        }
        else
        {
            for (YY = 0; YY <= PlotSizeP; YY++)
            {
                for (XX = 0; XX <= PlotSizeP; XX++)
                {
                    if (Mask[XX][YY])
                    {
                        PosX = Pos + (ImgW * (YY)) + XX;
                        if (ImgZ[PosX] <= DrawZ)
                        {
                            R0 = R;
                            G0 = G;
                            B0 = B;
                            ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                        }

                        PosX = Pos + (ImgW * (YY)) - XX - PlotSizeP2;
                        if (ImgZ[PosX] <= DrawZ)
                        {
                            R0 = R;
                            G0 = G;
                            B0 = B;
                            ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                        }

                        PosX = Pos - (ImgW * (YY + PlotSizeP2)) + XX;
                        if (ImgZ[PosX] <= DrawZ)
                        {
                            R0 = R;
                            G0 = G;
                            B0 = B;
                            ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                        }

                        PosX = Pos - (ImgW * (YY + PlotSizeP2)) - XX - PlotSizeP2;
                        if (ImgZ[PosX] <= DrawZ)
                        {
                            R0 = R;
                            G0 = G;
                            B0 = B;
                            ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                        }
                    }
                }
            }
        }
    }
}

void IOGraphWin::DrawPointH(int X, int Y, int Z, uchar R, uchar G, uchar B, double LightAngle)
{
    int DrawX = X + ZeroX;
    int DrawY = ZeroY - Y;
    int DrawZ = Z + ZeroZ;
    uchar R0, G0, B0;

    if ((DrawX >= BoundLL) && (DrawY >= 0) && (DrawX <= BoundRL) && (DrawY <= ImgH1))
    {
        int Pos = (DrawY * ImgW) + DrawX;
        int XX, PosX, Pos4X;
        if (InverseDraw)
        {
            for (XX = 0 - PlotSizeL - PlotSizeL2; XX <= PlotSizeL; XX++)
            {
                PosX = Pos + XX;
                if (ImgZ[PosX] < DrawZ)
                {
                    R0 = R;
                    G0 = G;
                    B0 = B;
                    CalcColor(R0, G0, B0, DrawX + XX, DrawY, DrawZ, LightAngle);
                    ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                }
            }
        }
        else
        {
            for (XX = 0 - PlotSizeL - PlotSizeL2; XX <= PlotSizeL; XX++)
            {
                PosX = Pos + XX;
                if (ImgZ[PosX] <= DrawZ)
                {
                    R0 = R;
                    G0 = G;
                    B0 = B;
                    CalcColor(R0, G0, B0, DrawX + XX, DrawY, DrawZ, LightAngle);
                    ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                }
            }
        }
    }
}

void IOGraphWin::DrawPointV(int X, int Y, int Z, uchar R, uchar G, uchar B, double LightAngle)
{
    int DrawX = X + ZeroX;
    int DrawY = ZeroY - Y;
    int DrawZ = Z + ZeroZ;
    uchar R0, G0, B0;

    if ((DrawX >= 0) && (DrawY >= BoundTL) && (DrawX <= ImgW1) && (DrawY <= BoundBL))
    {
        int Pos = (DrawY * ImgW) + DrawX;
        int YY, PosX, Pos4X;
        if (InverseDraw)
        {
            for (YY = 0 - PlotSizeL - PlotSizeL2; YY <= PlotSizeL; YY++)
            {
                PosX = Pos + (ImgW * YY);
                if (ImgZ[PosX] < DrawZ)
                {
                    R0 = R;
                    G0 = G;
                    B0 = B;
                    CalcColor(R0, G0, B0, DrawX, DrawY + YY, DrawZ, LightAngle);
                    ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                }
            }
        }
        else
        {
            for (YY = 0 - PlotSizeL - PlotSizeL2; YY <= PlotSizeL; YY++)
            {
                PosX = Pos + (ImgW * YY);
                if (ImgZ[PosX] <= DrawZ)
                {
                    R0 = R;
                    G0 = G;
                    B0 = B;
                    CalcColor(R0, G0, B0, DrawX, DrawY + YY, DrawZ, LightAngle);
                    ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                }
            }
        }
    }
}

void IOGraphWin::DrawPointD1(int X, int Y, int Z, uchar R, uchar G, uchar B, double LightAngle)
{
    int DrawX = X + ZeroX;
    int DrawY = ZeroY - Y;
    int DrawZ = Z + ZeroZ;
    uchar R0, G0, B0;

    if ((DrawX >= BoundLL) && (DrawY >= BoundTL) && (DrawX <= BoundRL) && (DrawY <= BoundBL))
    {
        int Pos = (DrawY * ImgW) + DrawX;
        int XX, PosX, Pos4X;
        if (InverseDraw)
        {
            for (XX = 0 - PlotSizeDiagonal1; XX <= PlotSizeDiagonal2; XX++)
            {
                PosX = Pos + (ImgW * XX) + XX;
                if (ImgZ[PosX] < DrawZ)
                {
                    R0 = R;
                    G0 = G;
                    B0 = B;
                    CalcColor(R0, G0, B0, DrawX + XX, DrawY, DrawZ, LightAngle);
                    ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                }
            }
            for (XX = 0 - PlotSizeDiagonal3; XX <= PlotSizeDiagonal4; XX++)
            {
                PosX = Pos + (ImgW * XX) + XX - 1;
                if (ImgZ[PosX] < DrawZ)
                {
                    R0 = R;
                    G0 = G;
                    B0 = B;
                    CalcColor(R0, G0, B0, DrawX + XX, DrawY, DrawZ, LightAngle);
                    ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                }
            }
        }
        else
        {
            for (XX = 0 - PlotSizeDiagonal1; XX <= PlotSizeDiagonal2; XX++)
            {
                PosX = Pos + (ImgW * XX) + XX;
                if (ImgZ[PosX] <= DrawZ)
                {
                    R0 = R;
                    G0 = G;
                    B0 = B;
                    CalcColor(R0, G0, B0, DrawX + XX, DrawY, DrawZ, LightAngle);
                    ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                }
            }
            for (XX = 0 - PlotSizeDiagonal3; XX <= PlotSizeDiagonal4; XX++)
            {
                PosX = Pos + (ImgW * XX) + XX - 1;
                if (ImgZ[PosX] <= DrawZ)
                {
                    R0 = R;
                    G0 = G;
                    B0 = B;
                    CalcColor(R0, G0, B0, DrawX + XX, DrawY, DrawZ, LightAngle);
                    ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                }
            }
        }
    }
}

void IOGraphWin::DrawPointD2(int X, int Y, int Z, uchar R, uchar G, uchar B, double LightAngle)
{
    int DrawX = X + ZeroX;
    int DrawY = ZeroY - Y;
    int DrawZ = Z + ZeroZ;
    uchar R0, G0, B0;

    if ((DrawX >= BoundLL) && (DrawY >= BoundTL) && (DrawX <= BoundRL) && (DrawY <= BoundBL))
    {
        int Pos = (DrawY * ImgW) + DrawX;
        int XX, PosX, Pos4X;
        if (InverseDraw)
        {
            for (XX = 0 - PlotSizeDiagonal1; XX <= PlotSizeDiagonal2; XX++)
            {
                PosX = Pos + (ImgW * XX) - XX;
                if (ImgZ[PosX] < DrawZ)
                {
                    R0 = R;
                    G0 = G;
                    B0 = B;
                    CalcColor(R0, G0, B0, DrawX + XX, DrawY, DrawZ, LightAngle);
                    ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                }
            }
            for (XX = 0 - PlotSizeDiagonal3; XX <= PlotSizeDiagonal4; XX++)
            {
                PosX = Pos + (ImgW * XX) - XX + 1;
                if (ImgZ[PosX] < DrawZ)
                {
                    R0 = R;
                    G0 = G;
                    B0 = B;
                    CalcColor(R0, G0, B0, DrawX + XX, DrawY, DrawZ, LightAngle);
                    ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                }
            }
        }
        else
        {
            for (XX = 0 - PlotSizeDiagonal1; XX <= PlotSizeDiagonal2; XX++)
            {
                PosX = Pos + (ImgW * XX) - XX;
                if (ImgZ[PosX] <= DrawZ)
                {
                    R0 = R;
                    G0 = G;
                    B0 = B;
                    CalcColor(R0, G0, B0, DrawX + XX, DrawY, DrawZ, LightAngle);
                    ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                }
            }
            for (XX = 0 - PlotSizeDiagonal3; XX <= PlotSizeDiagonal4; XX++)
            {
                PosX = Pos + (ImgW * XX) - XX + 1;
                if (ImgZ[PosX] <= DrawZ)
                {
                    R0 = R;
                    G0 = G;
                    B0 = B;
                    CalcColor(R0, G0, B0, DrawX + XX, DrawY, DrawZ, LightAngle);
                    ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B0; ImgRaw[Pos4X + 1] = G0; ImgRaw[Pos4X + 2] = R0;
                }
            }
        }
    }
}

void IOGraphWin::DrawLine(int X0, int Y0, int Z0, int X1, int Y1, int Z1, uchar R, uchar G, uchar B, double LightAngle)
{
    int DrawX0_ = X0 + ZeroX;
    int DrawY0_ = ZeroY - Y0;
    int DrawX1_ = X1 + ZeroX;
    int DrawY1_ = ZeroY - Y1;

    if ((DrawX0_ < 0) && (DrawX1_ < 0)) { return; }
    if ((DrawY0_ < 0) && (DrawY1_ < 0)) { return; }
    if ((DrawX0_ > ImgW1) && (DrawX1_ > ImgW1)) { return; }
    if ((DrawY0_ > ImgH1) && (DrawY1_ > ImgH1)) { return; }

    int Temp;
    double dx0,  dy0,  dz0;
    double dx,   dy,   dz;
    double tempx,tempy,tempz;
    double x,    y,    z;
    double x1,   y1,   z1;
    double rob;
    x1 = X0;
    y1 = Y0;
    z1 = Z0;

    dx0 = X1 - X0;
    dy0 = Y1 - Y0;
    dz0 = Z1 - Z0;

    if (X0 > X1) { Temp = X0; X0 = X1; X1 = Temp; }
    if (Y0 > Y1) { Temp = Y0; Y0 = Y1; Y1 = Temp; }
    if (Z0 > Z1) { Temp = Z0; Z0 = Z1; Z1 = Temp; }
    dx = X1 - X0;
    dy = Y1 - Y0;
    dz = Z1 - Z0;

    int DrawLineMode = 0;
    if (dx > (dy * 1.7321))
    {
        DrawLineMode = 1;
        rob = dx;
    }
    else
    {
        if (dy > (dx * 1.7321))
        {
            DrawLineMode = 2;
            rob = dy;
        }
        else
        {
            if (dx > dy)
            {
                rob = dx;
            }
            else
            {
                rob = dy;
            }
            if ((dx0 * dy0) >= 0)
            {
                DrawLineMode = 3;
            }
            else
            {
                DrawLineMode = 4;
            }
        }
    }

    int rob_ = rob;
    (dx != 0) ? tempx = (dx0 / rob) : tempx = 0;
    (dy != 0) ? tempy = (dy0 / rob) : tempy = 0;
    (dz != 0) ? tempz = (dz0 / rob) : tempz = 0;

    switch (DrawLineMode)
    {
        case 1:
            while (rob_ != 0)
            {
                x1 = x1 + tempx;
                y1 = y1 + tempy;
                z1 = z1 + tempz;
                x = floor(x1 + 0.5);
                y = floor(y1 + 0.5);
                z = floor(z1 + 0.5);
                DrawPointV(x, y, z, R, G, B, LightAngle);
                rob_--;
            }
            return;
        case 2:
            while (rob_ != 0)
            {
                x1 = x1 + tempx;
                y1 = y1 + tempy;
                z1 = z1 + tempz;
                x = floor(x1 + 0.5);
                y = floor(y1 + 0.5);
                z = floor(z1 + 0.5);
                DrawPointH(x, y, z, R, G, B, LightAngle);
                rob_--;
            }
            return;
        case 3:
            while (rob_ != 0)
            {
                x1 = x1 + tempx;
                y1 = y1 + tempy;
                z1 = z1 + tempz;
                x = floor(x1 + 0.5);
                y = floor(y1 + 0.5);
                z = floor(z1 + 0.5);
                DrawPointD1(x, y, z, R, G, B, LightAngle);
                rob_--;
            }
            return;
        case 4:
            while (rob_ != 0)
            {
                x1 = x1 + tempx;
                y1 = y1 + tempy;
                z1 = z1 + tempz;
                x = floor(x1 + 0.5);
                y = floor(y1 + 0.5);
                z = floor(z1 + 0.5);
                DrawPointD2(x, y, z, R, G, B, LightAngle);
                rob_--;
            }
            return;
    }
}

void IOGraphWin::DrawLine0(int X0, int Y0, int Z0, int X1, int Y1, int Z1, int R0, int G0, int B0, int R1, int G1, int B1, double LightAngle)
{
    int DrawX0_ = X0 + ZeroX;
    int DrawY0_ = ZeroY - Y0;
    int DrawX1_ = X1 + ZeroX;
    int DrawY1_ = ZeroY - Y1;

    if ((DrawX0_ < 0) && (DrawX1_ < 0)) { return; }
    if ((DrawY0_ < 0) && (DrawY1_ < 0)) { return; }
    if ((DrawX0_ > ImgW1) && (DrawX1_ > ImgW1)) { return; }
    if ((DrawY0_ > ImgH1) && (DrawY1_ > ImgH1)) { return; }

    int Temp;
    double dx0,   dy0,   dz0,   dr0,   dg0,   db0;
    double dx,    dy,    dz,    dr,    dg,    db;
    double tempx, tempy, tempz, tempr, tempg, tempb;
    double x,     y,     z,     r,     g,     b;
    double x1,    y1,    z1,    r1,    g1,    b1;
    double rob;
    x1 = X0;
    y1 = Y0;
    z1 = Z0;
    r1 = R0;
    g1 = G0;
    b1 = B0;

    dx0 = X1 - X0;
    dy0 = Y1 - Y0;
    dz0 = Z1 - Z0;
    dr0 = R1 - R0;
    dg0 = G1 - G0;
    db0 = B1 - B0;

    if (X0 > X1) { Temp = X0; X0 = X1; X1 = Temp; }
    if (Y0 > Y1) { Temp = Y0; Y0 = Y1; Y1 = Temp; }
    if (Z0 > Z1) { Temp = Z0; Z0 = Z1; Z1 = Temp; }
    if (R0 > R1) { Temp = R0; R0 = R1; R1 = Temp; }
    if (G0 > G1) { Temp = G0; G0 = G1; G1 = Temp; }
    if (B0 > B1) { Temp = B0; B0 = B1; B1 = Temp; }
    dx = X1 - X0;
    dy = Y1 - Y0;
    dz = Z1 - Z0;
    dr = R1 - R0;
    dg = G1 - G0;
    db = B1 - B0;

    int DrawLineMode = 0;
    if (dx > (dy * 2))
    {
        DrawLineMode = 1;
        rob = dx;
    }
    else
    {
        if (dy > (dx * 2))
        {
            DrawLineMode = 2;
            rob = dy;
        }
        else
        {
            if (dx > dy)
            {
                rob = dx;
            }
            else
            {
                rob = dy;
            }
            if ((dx0 * dy0) >= 0)
            {
                DrawLineMode = 3;
            }
            else
            {
                DrawLineMode = 4;
            }
        }
    }

    int rob_ = rob;
    (dx != 0) ? tempx = (dx0 / rob) : tempx = 0;
    (dy != 0) ? tempy = (dy0 / rob) : tempy = 0;
    (dz != 0) ? tempz = (dz0 / rob) : tempz = 0;
    (dr != 0) ? tempr = (dr0 / rob) : tempr = 0;
    (dg != 0) ? tempg = (dg0 / rob) : tempg = 0;
    (db != 0) ? tempb = (db0 / rob) : tempb = 0;
    switch (DrawLineMode)
    {
        case 1:
            while (rob_ != 0)
            {
                x1 = x1 + tempx;
                y1 = y1 + tempy;
                z1 = z1 + tempz;
                r1 = r1 + tempr;
                g1 = g1 + tempg;
                b1 = b1 + tempb;
                x = floor(x1 + 0.5);
                y = floor(y1 + 0.5);
                z = floor(z1 + 0.5);
                r = floor(r1 + 0.5);
                g = floor(g1 + 0.5);
                b = floor(b1 + 0.5);
                DrawPointV(x, y, z, r, g, b, LightAngle);
                rob_--;
            }
            return;
        case 2:
            while (rob_ != 0)
            {
                x1 = x1 + tempx;
                y1 = y1 + tempy;
                z1 = z1 + tempz;
                r1 = r1 + tempr;
                g1 = g1 + tempg;
                b1 = b1 + tempb;
                x = floor(x1 + 0.5);
                y = floor(y1 + 0.5);
                z = floor(z1 + 0.5);
                r = floor(r1 + 0.5);
                g = floor(g1 + 0.5);
                b = floor(b1 + 0.5);
                DrawPointH(x, y, z, r, g, b, LightAngle);
                rob_--;
            }
            return;
        case 3:
            while (rob_ != 0)
            {
                x1 = x1 + tempx;
                y1 = y1 + tempy;
                z1 = z1 + tempz;
                r1 = r1 + tempr;
                g1 = g1 + tempg;
                b1 = b1 + tempb;
                x = floor(x1 + 0.5);
                y = floor(y1 + 0.5);
                z = floor(z1 + 0.5);
                r = floor(r1 + 0.5);
                g = floor(g1 + 0.5);
                b = floor(b1 + 0.5);
                DrawPointD1(x, y, z, r, g, b, LightAngle);
                rob_--;
            }
            return;
        case 4:
            while (rob_ != 0)
            {
                x1 = x1 + tempx;
                y1 = y1 + tempy;
                z1 = z1 + tempz;
                r1 = r1 + tempr;
                g1 = g1 + tempg;
                b1 = b1 + tempb;
                x = floor(x1 + 0.5);
                y = floor(y1 + 0.5);
                z = floor(z1 + 0.5);
                r = floor(r1 + 0.5);
                g = floor(g1 + 0.5);
                b = floor(b1 + 0.5);
                DrawPointD2(x, y, z, r, g, b, LightAngle);
                rob_--;
            }
            return;
    }
}

void IOGraphWin::CalcTriangleAngle(int X1, int Y1, int Z1, int X2, int Y2, int Z2, int X3, int Y3, int Z3, double &LightAngle)
{
    /*
          | Y1 Z1 1 |      | X1 Z1 1 |      | X1 Y1 1 |      | X1 Y1 Z1 |
      A = | Y2 Z2 1 |  B = | X2 Z2 1 |  C = | X2 Y2 1 |  D = | X2 Y2 Z2 |
          | Y3 Z3 1 |      | X3 Z3 1 |      | X3 Y3 1 |      | X3 Y3 Z3 |

          A*X - B*Y + C*Z - D = 0;
     */
    /*double A = Det3(Y1, Z1, 1, Y2, Z2, 1, Y3, Z3, 1);
    double B = 0 - Det3(X1, Z1, 1, X2, Z2, 1, X3, Z3, 1);
    double C = Det3(X1, Y1, 1, X2, Y2, 1, X3, Y3, 1);*/


    int Len12 = ((X1 - X2) * (X1 - X2)) + ((Y1 - Y2) * (Y1 - Y2)) + ((Z1 - Z2) * (Z1 - Z2));
    int Len23 = ((X2 - X3) * (X2 - X3)) + ((Y2 - Y3) * (Y2 - Y3)) + ((Z2 - Z3) * (Z2 - Z3));
    int Len31 = ((X3 - X1) * (X3 - X1)) + ((Y3 - Y1) * (Y3 - Y1)) + ((Z3 - Z1) * (Z3 - Z1));

    double Vec1X, Vec1Y, Vec1Z;
    double Vec2X, Vec2Y, Vec2Z;

    if ((Len12 <= Len23) && (Len12 <= Len31))
    {
        Vec1X = X1 - X3; Vec1Y = Y1 - Y3; Vec1Z = Z1 - Z3;
        Vec2X = X2 - X3; Vec2Y = Y2 - Y3; Vec2Z = Z2 - Z3;
    }
    else
    {
        if ((Len23 <= Len12) && (Len23 <= Len31))
        {
            Vec1X = X2 - X1; Vec1Y = Y2 - Y1; Vec1Z = Z2 - Z1;
            Vec2X = X3 - X1; Vec2Y = Y3 - Y1; Vec2Z = Z3 - Z1;
        }
        else
        {
            Vec1X = X3 - X2; Vec1Y = Y3 - Y2; Vec1Z = Z3 - Z2;
            Vec2X = X1 - X2; Vec2Y = Y1 - Y2; Vec2Z = Z1 - Z2;
        }
    }

    double TriX = Vec1Y * Vec2Z - Vec1Z * Vec2Y;
    double TriY = Vec1Z * Vec2X - Vec1X * Vec2Z;
    double TriZ = Vec1X * Vec2Y - Vec1Y * Vec2X;

    LightAngle = VectorAngle(TriX, TriY, TriZ, LightVectorX, LightVectorY, LightVectorZ);
    if (LightAngle > M_PI_2)
    {
        if (VectorAngle(TriX, TriY, TriZ, 0.0, 0.0, -1.0) < M_PI_2)
        {
            LightAngle = M_PI - LightAngle;
        }
    }
    else
    {
        if (VectorAngle(TriX, TriY, TriZ, 0.0, 0.0, 1.0) > M_PI_2)
        {
            LightAngle = M_PI - LightAngle;
        }
    }
}

void IOGraphWin::DrawHalfTriangle(int X0, int Y0, int Z0, int X1, int X2, int Y_, int Z1, int Z2, uchar R0, uchar G0, uchar B0, double LightAngle)
{
    int DrawX0 = X0 + ZeroX;
    int DrawY0 = ZeroY - Y0;
    int DrawX1 = X1 + ZeroX;
    int DrawX2 = X2 + ZeroX;
    int DrawY_ = ZeroY - Y_;

    int X, Y, Z, YI1, YI2, XI1, XI2, ZI1, ZI2;
    double DX1, DX2, DZ1, DZ2, DY, DYX, A1, A2, A1Z, A2Z, ZProp;
    int XDelta, YDelta, ZDelta;

    if (DrawX1 > DrawX2)
    {
        X = DrawX1;
        DrawX1 = DrawX2;
        DrawX2 = X;
        X = Z1;
        Z1 = Z2;
        Z2 = X;
    }

    DX1 = DrawX1 - DrawX0;
    DX2 = DrawX2 - DrawX0;
    DY = DrawY0 - DrawY_;
    DZ1 = Z1 - Z0;
    DZ2 = Z2 - Z0;
    A1 = DX1 / DY;
    A2 = DX2 / DY;
    A1Z = DZ1 / DY;
    A2Z = DZ2 / DY;

    if (DrawY_ < DrawY0)
    {
        YI1 = DrawY_;
        YI2 = DrawY0;
    }
    else
    {
        YI1 = DrawY0;
        YI2 = DrawY_;
    }

    YDelta = YI2 - YI1;
    if (YDelta < 1)
    {
        YDelta = 1;
    }
    for (Y = YI1; Y <= YI2; Y++)
    {
        DYX = (DrawY_ - Y);
        XI1 = round(DYX * A1) + DrawX1;
        XI2 = round(DYX * A2) + DrawX2;
        ZI1 = round(DYX * A1Z) + Z1;
        ZI2 = round(DYX * A2Z) + Z2;
        XDelta = XI2 - XI1;
        ZDelta = ZI2 - ZI1;
        ZProp = (double)ZDelta / (double)XDelta;
        if (XDelta < 1)
        {
            XDelta = 1;
        }
        if (InverseDraw)
        {
            for (X = XI1; X <= XI2; X++)
            {
                if ((X >= 0) && (Y >= 0) && (X <= ImgW1) && (Y <= ImgH1))
                {
                    Z = ZI1 + ((X - XI1) * ZProp);
                    int Pos = (Y * ImgW) + X;
                    int Pos4X;
                    uchar R0_ = R0;
                    uchar G0_ = G0;
                    uchar B0_ = B0;
                    CalcColor(R0_, G0_, B0_, X, Y, Z, LightAngle);
                    if (ImgZ[Pos] < Z) { ImgZ[Pos] = Z; Pos4X = Pos << 2; ImgRaw[Pos4X + 0] = B0_; ImgRaw[Pos4X + 1] = G0_; ImgRaw[Pos4X + 2] = R0_; }
                }
            }
        }
        else
        {
            for (X = XI1; X <= XI2; X++)
            {
                if ((X >= 0) && (Y >= 0) && (X <= ImgW1) && (Y <= ImgH1))
                {
                    Z = ZI1 + ((X - XI1) * ZProp);
                    int Pos = (Y * ImgW) + X;
                    int Pos4X;
                    uchar R0_ = R0;
                    uchar G0_ = G0;
                    uchar B0_ = B0;
                    CalcColor(R0_, G0_, B0_, X, Y, Z, LightAngle);
                    if (ImgZ[Pos] <= Z) { ImgZ[Pos] = Z; Pos4X = Pos << 2; ImgRaw[Pos4X + 0] = B0_; ImgRaw[Pos4X + 1] = G0_; ImgRaw[Pos4X + 2] = R0_; }
                }
            }
        }
    }
}

void IOGraphWin::DrawTriangle(int X1, int Y1, int Z1, int X2, int Y2, int Z2, int X3, int Y3, int Z3, uchar R0, uchar G0, uchar B0, double LightAngle)
{
    int XMin;
    int XMax;
    int YMin;
    int YMax;

    double F1, F2, F3, F3Z, F4, F4Z;

    int P0X_;
    int P0X__;
    int P0Y_;
    int P0Z_;
    int P0Z__;
    int P1X_;
    int P1Y_;
    int P1Z_;
    int P2X_;
    int P2Y_;
    int P2Z_;

    XMin = X1;
    XMax = X1;
    if (XMin > X2) { XMin = X2; }
    if (XMax < X2) { XMax = X2; }
    if (XMin > X3) { XMin = X3; }
    if (XMax < X3) { XMax = X3; }

    YMin = Y1;
    YMax = Y1;
    if (YMin > Y2) { YMin = Y2; }
    if (YMax < Y2) { YMax = Y2; }
    if (YMin > Y3) { YMin = Y3; }
    if (YMax < Y3) { YMax = Y3; }

    P0Y_ = YMin;

    if ((Y1 > YMin) && (Y1 < YMax))
    {
        P0X_ = X1; P0Y_ = Y1; P0Z_ = Z1;
        if (Y2 == YMin)
        {
            P1X_ = X2; P1Y_ = Y2; P1Z_ = Z2;
            P2X_ = X3; P2Y_ = Y3; P2Z_ = Z3;
        }
        else
        {
            P1X_ = X3; P1Y_ = Y3; P1Z_ = Z3;
            P2X_ = X2; P2Y_ = Y2; P2Z_ = Z2;
        }
    }
    else
    {
        if ((Y2 > YMin) && (Y2 < YMax))
        {
            P0X_ = X2; P0Y_ = Y2; P0Z_ = Z2;
            if (Y3 == YMin)
            {
                P1X_ = X3; P1Y_ = Y3; P1Z_ = Z3;
                P2X_ = X1; P2Y_ = Y1; P2Z_ = Z1;
            }
            else
            {
                P1X_ = X1; P1Y_ = Y1; P1Z_ = Z1;
                P2X_ = X3; P2Y_ = Y3; P2Z_ = Z3;
            }
        }
        else
        {
            if ((Y3 > YMin) && (Y3 < YMax))
            {
                P0X_ = X3; P0Y_ = Y3; P0Z_ = Z3;
                if (Y1 == YMin)
                {
                    P1X_ = X1; P1Y_ = Y1; P1Z_ = Z1;
                    P2X_ = X2; P2Y_ = Y2; P2Z_ = Z2;
                }
                else
                {
                    P1X_ = X2; P1Y_ = Y2; P1Z_ = Z2;
                    P2X_ = X1; P2Y_ = Y1; P2Z_ = Z1;
                }
            }
        }
    }



    if ((P0Y_ > YMin) && (P0Y_ < YMax))
    {
        F1 = P0Y_ - P1Y_;
        F2 = P2Y_ - P1Y_;

        F3 = P2X_ - P1X_;
        F3Z = P2Z_ - P1Z_;
        F4 = F3 * (F1 / F2);
        F4Z = F3Z * (F1 / F2);

        P0X__ = P1X_ + F4;
        P0Z__ = P1Z_ + F4Z;

        if (PlotSurface)
        {
            DrawHalfTriangle(P1X_, P1Y_, P1Z_, P0X_, P0X__, P0Y_, P0Z_, P0Z__, R0, G0, B0, LightAngle);
            DrawHalfTriangle(P2X_, P2Y_, P2Z_, P0X_, P0X__, P0Y_, P0Z_, P0Z__, R0, G0, B0, LightAngle);
        }
    }
    else
    {
        if (Y1 == Y2)
        {
            if (PlotSurface)
            {
                DrawHalfTriangle(X3, Y3, Z3, X1, X2, Y1, Z1, Z2, R0, G0, B0, LightAngle);
            }
        }
        if (Y2 == Y3)
        {
            if (PlotSurface)
            {
                DrawHalfTriangle(X1, Y1, Z1, X2, X3, Y2, Z2, Z3, R0, G0, B0, LightAngle);
            }
        }
        if (Y3 == Y1)
        {
            if (PlotSurface)
            {
                DrawHalfTriangle(X2, Y2, Z2, X3, X1, Y3, Z3, Z1, R0, G0, B0, LightAngle);
            }
        }
    }
}

void IOGraphWin::DrawHalfTriangle0(int X0, int Y0, int Z0, int X1, int X2, int Y_, int Z1, int Z2, uchar R0, uchar G0, uchar B0, uchar R1, uchar G1, uchar B1, uchar R2, uchar G2, uchar B2, double LightAngle)
{
    int ColorR0 = R0;
    int ColorG0 = G0;
    int ColorB0 = B0;
    int ColorR1 = R1;
    int ColorG1 = G1;
    int ColorB1 = B1;
    int ColorR2 = R2;
    int ColorG2 = G2;
    int ColorB2 = B2;

    int DrawX0 = X0 + ZeroX;
    int DrawY0 = ZeroY - Y0;
    int DrawX1 = X1 + ZeroX;
    int DrawX2 = X2 + ZeroX;
    int DrawY_ = ZeroY - Y_;

    int X, Y, Z, YI1, YI2, XI1, XI2, ZI1, ZI2;
    double DX1, DX2, DZ1, DZ2, DY, DYX, A1, A2, A1Z, A2Z, ZProp;
    int ColorR, ColorG, ColorB, YProp, YProp0, XProp, XProp0, XDelta, YDelta, ZDelta;
    int ColorRW1, ColorGW1, ColorBW1;
    int ColorRW2, ColorGW2, ColorBW2;

    if (DrawX1 > DrawX2)
    {
        X = DrawX1;
        DrawX1 = DrawX2;
        DrawX2 = X;
        X = Z1;
        Z1 = Z2;
        Z2 = X;
        X = ColorR1;
        ColorR1 = ColorR2;
        ColorR2 = X;
        X = ColorG1;
        ColorG1 = ColorG2;
        ColorG2 = X;
        X = ColorB1;
        ColorB1 = ColorB2;
        ColorB2 = X;
    }

    DX1 = DrawX1 - DrawX0;
    DX2 = DrawX2 - DrawX0;
    DZ1 = Z1 - Z0;
    DZ2 = Z2 - Z0;
    DY = DrawY0 - DrawY_;
    A1 = DX1 / DY;
    A2 = DX2 / DY;
    A1Z = DZ1 / DY;
    A2Z = DZ2 / DY;

    if (DrawY_ < DrawY0)
    {
        YI1 = DrawY_;
        YI2 = DrawY0;
    }
    else
    {
        YI1 = DrawY0;
        YI2 = DrawY_;
    }

    YDelta = YI2 - YI1;
    if (YDelta < 1)
    {
        YDelta = 1;
    }
    for (Y = YI1; Y <= YI2; Y++)
    {
        DYX = (DrawY_ - Y);
        XI1 = round(DYX * A1) + DrawX1;
        XI2 = round(DYX * A2) + DrawX2;
        ZI1 = round(DYX * A1Z) + Z1;
        ZI2 = round(DYX * A2Z) + Z2;

        if (DrawY_ < DrawY0)
        {
            YProp = (((Y - YI1) * 100) / (YDelta));
        }
        else
        {
            YProp = (((YI2 - Y) * 100) / (YDelta));
        }
        YProp0 = 100 - YProp;
        XDelta = XI2 - XI1;
        ZDelta = ZI2 - ZI1;
        ZProp = (double)ZDelta / (double)XDelta;
        if (XDelta < 1)
        {
            XDelta = 1;
        }
        if (InverseDraw)
        {
            for (X = XI1; X <= XI2; X++)
            {
                if ((X >= 0) && (Y >= 0) && (X <= ImgW1) && (Y <= ImgH1))
                {
                    Z = ZI1 + ((X - XI1) * ZProp);

                    XProp = ((X - XI1) * 100) / XDelta;
                    XProp0 = 100 - XProp;

                    ColorRW1 = ((ColorR1 * YProp0) + (ColorR0 * YProp)) / 100;
                    ColorGW1 = ((ColorG1 * YProp0) + (ColorG0 * YProp)) / 100;
                    ColorBW1 = ((ColorB1 * YProp0) + (ColorB0 * YProp)) / 100;

                    ColorRW2 = ((ColorR2 * YProp0) + (ColorR0 * YProp)) / 100;
                    ColorGW2 = ((ColorG2 * YProp0) + (ColorG0 * YProp)) / 100;
                    ColorBW2 = ((ColorB2 * YProp0) + (ColorB0 * YProp)) / 100;

                    ColorR = ((ColorRW1 * XProp0) + (ColorRW2) * XProp) / 100;
                    ColorG = ((ColorGW1 * XProp0) + (ColorGW2) * XProp) / 100;
                    ColorB = ((ColorBW1 * XProp0) + (ColorBW2) * XProp) / 100;

                    uchar ColorR_ = Range(ColorR, 0, 255);
                    uchar ColorG_ = Range(ColorG, 0, 255);
                    uchar ColorB_ = Range(ColorB, 0, 255);

                    int Pos = (Y * ImgW) + X;
                    int Pos4X;
                    CalcColor(ColorR_, ColorG_, ColorB_, X, Y, Z, LightAngle);
                    if (ImgZ[Pos] < Z) { ImgZ[Pos] = Z; Pos4X = Pos << 2; ImgRaw[Pos4X + 0] = ColorB_; ImgRaw[Pos4X + 1] = ColorG_; ImgRaw[Pos4X + 2] = ColorR_; }
                }
            }
        }
        else
        {
            for (X = XI1; X <= XI2; X++)
            {
                if ((X >= 0) && (Y >= 0) && (X <= ImgW1) && (Y <= ImgH1))
                {
                    Z = ZI1 + ((X - XI1) * ZProp);

                    XProp = ((X - XI1) * 100) / XDelta;
                    XProp0 = 100 - XProp;

                    ColorRW1 = ((ColorR1 * YProp0) + (ColorR0 * YProp)) / 100;
                    ColorGW1 = ((ColorG1 * YProp0) + (ColorG0 * YProp)) / 100;
                    ColorBW1 = ((ColorB1 * YProp0) + (ColorB0 * YProp)) / 100;

                    ColorRW2 = ((ColorR2 * YProp0) + (ColorR0 * YProp)) / 100;
                    ColorGW2 = ((ColorG2 * YProp0) + (ColorG0 * YProp)) / 100;
                    ColorBW2 = ((ColorB2 * YProp0) + (ColorB0 * YProp)) / 100;

                    ColorR = ((ColorRW1 * XProp0) + (ColorRW2) * XProp) / 100;
                    ColorG = ((ColorGW1 * XProp0) + (ColorGW2) * XProp) / 100;
                    ColorB = ((ColorBW1 * XProp0) + (ColorBW2) * XProp) / 100;

                    uchar ColorR_ = Range(ColorR, 0, 255);
                    uchar ColorG_ = Range(ColorG, 0, 255);
                    uchar ColorB_ = Range(ColorB, 0, 255);

                    int Pos = (Y * ImgW) + X;
                    int Pos4X;
                    CalcColor(ColorR_, ColorG_, ColorB_, X, Y, Z, LightAngle);
                    if (ImgZ[Pos] <= Z) { ImgZ[Pos] = Z; Pos4X = Pos << 2; ImgRaw[Pos4X + 0] = ColorB_; ImgRaw[Pos4X + 1] = ColorG_; ImgRaw[Pos4X + 2] = ColorR_; }
                }
            }
        }
    }
}

void IOGraphWin::DrawTriangle0(int X1, int Y1, int Z1, int X2, int Y2, int Z2, int X3, int Y3, int Z3, uchar R1, uchar G1, uchar B1, uchar R2, uchar G2, uchar B2, uchar R3, uchar G3, uchar B3, double LightAngle)
{
    int XMin;
    int XMax;
    int YMin;
    int YMax;

    double F1, F2, F3, F3Z, F4, F4Z, F5, F6;

    int P0X_;
    int P0X__;
    int P0Y_;
    int P0Z_;
    int P0Z__;
    int P1X_;
    int P1Y_;
    int P1Z_;
    int P2X_;
    int P2Y_;
    int P2Z_;

    int Color0R_ = 0;
    int Color0G_ = 0;
    int Color0B_ = 0;

    int Color1R_ = 0;
    int Color1G_ = 0;
    int Color1B_ = 0;

    int Color2R_ = 0;
    int Color2G_ = 0;
    int Color2B_ = 0;

    int Color3R_ = 0;
    int Color3G_ = 0;
    int Color3B_ = 0;

    if (LightMode == 2)
    {
        CalcTriangleAngle(X1, Y1, Z1, X2, Y2, Z2, X3, Y3, Z3, LightAngle);
    }

    XMin = X1;
    XMax = X1;
    if (XMin > X2) { XMin = X2; }
    if (XMax < X2) { XMax = X2; }
    if (XMin > X3) { XMin = X3; }
    if (XMax < X3) { XMax = X3; }

    YMin = Y1;
    YMax = Y1;
    if (YMin > Y2) { YMin = Y2; }
    if (YMax < Y2) { YMax = Y2; }
    if (YMin > Y3) { YMin = Y3; }
    if (YMax < Y3) { YMax = Y3; }

    P0Y_ = YMin;

    if ((Y1 > YMin) && (Y1 < YMax))
    {
        P0X_ = X1; P0Y_ = Y1; P0Z_ = Z1;
        if (Y2 == YMin)
        {
            P1X_ = X2; P1Y_ = Y2; P1Z_ = Z2;
            P2X_ = X3; P2Y_ = Y3; P2Z_ = Z3;
            Color1R_ = R2; Color1G_ = G2; Color1B_ = B2;
            Color2R_ = R1; Color2G_ = G1; Color2B_ = B1;
            Color3R_ = R3; Color3G_ = G3; Color3B_ = B3;
        }
        else
        {
            P1X_ = X3; P1Y_ = Y3; P1Z_ = Z3;
            P2X_ = X2; P2Y_ = Y2; P2Z_ = Z2;
            Color1R_ = R3; Color1G_ = G3; Color1B_ = B3;
            Color2R_ = R1; Color2G_ = G1; Color2B_ = B1;
            Color3R_ = R2; Color3G_ = G2; Color3B_ = B2;
        }
    }
    else
    {
        if ((Y2 > YMin) && (Y2 < YMax))
        {
            P0X_ = X2; P0Y_ = Y2; P0Z_ = Z2;
            if (Y3 == YMin)
            {
                P1X_ = X3; P1Y_ = Y3; P1Z_ = Z3;
                P2X_ = X1; P2Y_ = Y1; P2Z_ = Z1;
                Color1R_ = R3; Color1G_ = G3; Color1B_ = B3;
                Color2R_ = R2; Color2G_ = G2; Color2B_ = B2;
                Color3R_ = R1; Color3G_ = G1; Color3B_ = B1;
            }
            else
            {
                P1X_ = X1; P1Y_ = Y1; P1Z_ = Z1;
                P2X_ = X3; P2Y_ = Y3; P2Z_ = Z3;
                Color1R_ = R1; Color1G_ = G1; Color1B_ = B1;
                Color2R_ = R2; Color2G_ = G2; Color2B_ = B2;
                Color3R_ = R3; Color3G_ = G3; Color3B_ = B3;
            }
        }
        else
        {
            if ((Y3 > YMin) && (Y3 < YMax))
            {
                P0X_ = X3; P0Y_ = Y3; P0Z_ = Z3;
                if (Y1 == YMin)
                {
                    P1X_ = X1; P1Y_ = Y1; P1Z_ = Z1;
                    P2X_ = X2; P2Y_ = Y2; P2Z_ = Z2;
                    Color1R_ = R1; Color1G_ = G1; Color1B_ = B1;
                    Color2R_ = R3; Color2G_ = G3; Color2B_ = B3;
                    Color3R_ = R2; Color3G_ = G2; Color3B_ = B2;
                }
                else
                {
                    P1X_ = X2; P1Y_ = Y2; P1Z_ = Z2;
                    P2X_ = X1; P2Y_ = Y1; P2Z_ = Z1;
                    Color1R_ = R2; Color1G_ = G2; Color1B_ = B2;
                    Color2R_ = R3; Color2G_ = G3; Color2B_ = B3;
                    Color3R_ = R1; Color3G_ = G1; Color3B_ = B1;
                }
            }
        }
    }



    if ((P0Y_ > YMin) && (P0Y_ < YMax))
    {
        F1 = P0Y_ - P1Y_;
        F2 = P2Y_ - P1Y_;

        F3 = P2X_ - P1X_;
        F3Z = P2Z_ - P1Z_;
        F4 = F3 * (F1 / F2);
        F4Z = F3Z * (F1 / F2);
        F5 = (F1 / F2);
        F6 = 1.0 - (F1 / F2);

        P0X__ = P1X_ + F4;
        P0Z__ = P1Z_ + F4Z;

        if ((F5 > 0) && (F6 > 0))
        {
            Color0R_ = (Color1R_ * F6) + (Color3R_ * F5);
            Color0G_ = (Color1G_ * F6) + (Color3G_ * F5);
            Color0B_ = (Color1B_ * F6) + (Color3B_ * F5);
        }
        else
        {
            Color0R_ = 0;
            Color0G_ = 0;
            Color0B_ = 0;
        }

        if (PlotSurface)
        {
            DrawHalfTriangle0(P1X_, P1Y_, P1Z_, P0X_, P0X__, P0Y_, P0Z_, P0Z__, Color1R_, Color1G_, Color1B_, Color2R_, Color2G_, Color2B_, Color0R_, Color0G_, Color0B_, LightAngle);
            DrawHalfTriangle0(P2X_, P2Y_, P2Z_, P0X_, P0X__, P0Y_, P0Z_, P0Z__, Color3R_, Color3G_, Color3B_, Color2R_, Color2G_, Color2B_, Color0R_, Color0G_, Color0B_, LightAngle);
        }
    }
    else
    {
        if (Y1 == Y2)
        {
            if (PlotSurface)
            {
                DrawHalfTriangle0(X3, Y3, Z3, X1, X2, Y1, Z1, Z2, R3, G3, B3, R1, G1, B1, R2, G2, B2, LightAngle);
            }
        }
        if (Y2 == Y3)
        {
            if (PlotSurface)
            {
                DrawHalfTriangle0(X1, Y1, Z1, X2, X3, Y2, Z2, Z3, R1, G1, B1, R2, G2, B2, R3, G3, B3, LightAngle);
            }
        }
        if (Y3 == Y1)
        {
            if (PlotSurface)
            {
                DrawHalfTriangle0(X2, Y2, Z2, X3, X1, Y3, Z3, Z1, R2, G2, B2, R3, G3, B3, R1, G1, B1, LightAngle);
            }
        }
    }
}

void IOGraphWin::Refresh(bool ToFile, string FileName, int ScrW, int ScrH)
{
    if (!ToFile)
    {
        ScrW = ui->GraphScreen->width();
        ScrH = ui->GraphScreen->height();
    }
    if ((Core->IOGraph_[CurrentIO]->IsChanged()) || (CurrentIOX != CurrentIO) || (ScrW != ImgW) || (ScrH != ImgH))
    {
        if (CurrentIOX != CurrentIO)
        {
            RefreshNum = -1;
        }
        QImage * ImgX = NULL;
        CurrentIOX = CurrentIO;
        if ((ScrW > 0) && (ScrH > 0))
        {
            if ((ImgW != ScrW) || (ImgH != ScrH) || (Img == NULL) || (ToFile))
            {
                if (Img != NULL)
                {
                    ImgX = Img;
                }

                ImgW = ScrW;
                ImgH = ScrH;
                Img = NEW(QImage, QImage(ImgW, ImgH, QImage::Format_RGB32));
                ImgRaw = Img->bits();
                ImgZ = new int[ImgW * ImgH];
                ImgWH = ImgW * ImgH;
                for (int I = ((ImgWH << 2) - 1); I >= 0; I -= 4)
                {
                    ImgRaw[I] = 0;
                }

                RefreshNum = -1;
            }

            IOGraph * Gr = Core->IOGraph_[CurrentIO];

            int L = Gr->GraphDef.size();

            DimensionType X, Y, Z, X0, Y0, Z0, X00, Y00, Z00;
            int DrawX, DrawY, DrawZ, DrawX0, DrawY0, DrawZ0, DrawX00, DrawY00, DrawZ00;
            char DrawMode;
            double LightAngle = 0;

            if ((RefreshNum < 0) || (Core->IOGraph_[CurrentIO]->NeedRedraw))
            {
                ZeroX = (ImgW >> 1) - OffsetXRender;
                ZeroY = (ImgH >> 1) + OffsetYRender;
                ZeroZ = 0 - OffsetZRender;

                int P1 = 0;
                int P4 = 0;
                for (int Y = 0; Y < ImgH; Y++)
                {
                    for (int X = 0; X < ImgW; X++)
                    {
                        ImgZ[P1] = -2147483648;
                        P1++;
                        ImgRaw[P4 + 0] = BackColorB;
                        ImgRaw[P4 + 1] = BackColorG;
                        ImgRaw[P4 + 2] = BackColorR;
                        P4 += 4;
                    }
                }

                ImgW1 = ImgW - 1;
                ImgH1 = ImgH - 1;
                BoundLP = PlotSizeP + PlotSizeP2;
                BoundTP = PlotSizeP + PlotSizeP2;
                BoundRP = ImgW1 - PlotSizeP;
                BoundBP = ImgH1 - PlotSizeP;
                BoundLL = PlotSizeL + PlotSizeL2;
                BoundTL = PlotSizeL + PlotSizeL2;
                BoundRL = ImgW1 - PlotSizeL;
                BoundBL = ImgH1 - PlotSizeL;

                RefreshNum = 0;
            }

            if (Core->IOGraph_[CurrentIO]->NeedRedrawLast >= 0)
            {
                if (RefreshNum > Core->IOGraph_[CurrentIO]->NeedRedrawLast)
                {
                    RefreshNum = Core->IOGraph_[CurrentIO]->NeedRedrawLast;
                }
            }

            for (int I = RefreshNum; I < L; I++)
            {
                if (Gr->GraphDef[I].Draw)
                {
                    X = Gr->GraphDef[I].X - OffsetX;
                    Y = Gr->GraphDef[I].Y - OffsetY;
                    Z = Gr->GraphDef[I].Z - OffsetZ;
                    DrawX = round((X * DrawXtoX) + (Y * DrawYtoX) + (Z * DrawZtoX));
                    DrawY = round((X * DrawXtoY) + (Y * DrawYtoY) + (Z * DrawZtoY));
                    DrawZ = round((X * DrawXtoZ) + (Y * DrawYtoZ) + (Z * DrawZtoZ));

                    if (Gr->GraphDef[I].IsText)
                    {
                        if (PlotText && Core->GraphFont_->FontExists)
                        {
                            string Text = Gr->GraphDef[I].Text;
                            int TextLen = Gr->GraphDef[I].TextLen;
                            int III = 0;
                            int FontW = Core->GraphFont_->FontW;
                            int FontH = Core->GraphFont_->FontH;
                            bool TextB = Gr->GraphDef[I].TextB;
                            bool TextF = Gr->GraphDef[I].TextF;
                            for (int II = 0; II < TextLen; II++)
                            {
                                uchar * Glyph = Core->GraphFont_->FontGlyph[(uchar)Text[II]];
                                int P = 0;
                                int DrawXX = DrawX + III + Gr->GraphDef[I].TextX;
                                int DrawYY = DrawY - Gr->GraphDef[I].TextY;
                                for (int YY = 0; YY < FontH; YY++)
                                {
                                    for (int XX = 0; XX < FontW; XX++)
                                    {
                                        if (Glyph[P])
                                        {
                                            if (TextF)
                                            {
                                                DrawPointT(DrawXX + XX, DrawYY - YY, DrawZ, Gr->GraphDef[I].ColorR, Gr->GraphDef[I].ColorG, Gr->GraphDef[I].ColorB);
                                            }
                                        }
                                        else
                                        {
                                            if (TextB)
                                            {
                                                DrawPointT(DrawXX + XX, DrawYY - YY, DrawZ, Gr->GraphDef[I].ColorR0, Gr->GraphDef[I].ColorG0, Gr->GraphDef[I].ColorB0);
                                            }
                                        }
                                        P++;
                                    }
                                }
                                III += FontW;
                            }
                        }
                    }
                    else
                    {
                        DrawMode = Gr->GraphDef[I].DrawMode;

                        if (DrawMode != 0)
                        {
                            X0 = Gr->GraphDef[I].X0 - OffsetX;
                            Y0 = Gr->GraphDef[I].Y0 - OffsetY;
                            Z0 = Gr->GraphDef[I].Z0 - OffsetZ;
                            DrawX0 = round((X0 * DrawXtoX) + (Y0 * DrawYtoX) + (Z0 * DrawZtoX));
                            DrawY0 = round((X0 * DrawXtoY) + (Y0 * DrawYtoY) + (Z0 * DrawZtoY));
                            DrawZ0 = round((X0 * DrawXtoZ) + (Y0 * DrawYtoZ) + (Z0 * DrawZtoZ));

                            if ((DrawMode == 3) || (DrawMode == 4))
                            {
                                X00 = Gr->GraphDef[I].X00 - OffsetX;
                                Y00 = Gr->GraphDef[I].Y00 - OffsetY;
                                Z00 = Gr->GraphDef[I].Z00 - OffsetZ;
                                DrawX00 = round((X00 * DrawXtoX) + (Y00 * DrawYtoX) + (Z00 * DrawZtoX));
                                DrawY00 = round((X00 * DrawXtoY) + (Y00 * DrawYtoY) + (Z00 * DrawZtoY));
                                DrawZ00 = round((X00 * DrawXtoZ) + (Y00 * DrawYtoZ) + (Z00 * DrawZtoZ));

                                if (LightMode == 2)
                                {
                                    CalcTriangleAngle(DrawX00, DrawY00, DrawZ00, DrawX0, DrawY0, DrawZ0, DrawX, DrawY, DrawZ, LightAngle);
                                }


                                if (InverseDraw)
                                {
                                    if (PlotSizeP >= 0)
                                    {
                                        DrawPoint(DrawX, DrawY, DrawZ, Gr->GraphDef[I].ColorR, Gr->GraphDef[I].ColorG, Gr->GraphDef[I].ColorB, LightAngle);
                                        DrawPoint(DrawX0, DrawY0, DrawZ0, Gr->GraphDef[I].ColorR0, Gr->GraphDef[I].ColorG0, Gr->GraphDef[I].ColorB0, LightAngle);
                                        DrawPoint(DrawX00, DrawY00, DrawZ00, Gr->GraphDef[I].ColorR00, Gr->GraphDef[I].ColorG00, Gr->GraphDef[I].ColorB00, LightAngle);
                                    }
                                    if (PlotSizeL >= 0)
                                    {
                                        if (DrawMode == 3)
                                        {
                                            DrawLine(DrawX0, DrawY0, DrawZ0, DrawX, DrawY, DrawZ, Gr->GraphDef[I].ColorR, Gr->GraphDef[I].ColorG, Gr->GraphDef[I].ColorB, LightAngle);
                                            DrawLine(DrawX00, DrawY00, DrawZ00, DrawX0, DrawY0, DrawZ0, Gr->GraphDef[I].ColorR, Gr->GraphDef[I].ColorG, Gr->GraphDef[I].ColorB, LightAngle);
                                            DrawLine(DrawX, DrawY, DrawZ, DrawX00, DrawY00, DrawZ00, Gr->GraphDef[I].ColorR, Gr->GraphDef[I].ColorG, Gr->GraphDef[I].ColorB, LightAngle);
                                        }
                                        else
                                        {
                                            DrawLine0(DrawX0, DrawY0, DrawZ0, DrawX, DrawY, DrawZ, Gr->GraphDef[I].ColorR0, Gr->GraphDef[I].ColorG0, Gr->GraphDef[I].ColorB0, Gr->GraphDef[I].ColorR, Gr->GraphDef[I].ColorG, Gr->GraphDef[I].ColorB, LightAngle);
                                            DrawLine0(DrawX00, DrawY00, DrawZ00, DrawX0, DrawY0, DrawZ0, Gr->GraphDef[I].ColorR00, Gr->GraphDef[I].ColorG00, Gr->GraphDef[I].ColorB00, Gr->GraphDef[I].ColorR0, Gr->GraphDef[I].ColorG0, Gr->GraphDef[I].ColorB0, LightAngle);
                                            DrawLine0(DrawX, DrawY, DrawZ, DrawX00, DrawY00, DrawZ00, Gr->GraphDef[I].ColorR, Gr->GraphDef[I].ColorG, Gr->GraphDef[I].ColorB, Gr->GraphDef[I].ColorR00, Gr->GraphDef[I].ColorG00, Gr->GraphDef[I].ColorB00, LightAngle);
                                        }
                                    }
                                }
                                if (PlotSurface || (LightMode == 2))
                                {
                                    if (DrawMode == 3)
                                    {
                                        DrawTriangle(DrawX00, DrawY00, DrawZ00, DrawX0, DrawY0, DrawZ0, DrawX, DrawY, DrawZ, Gr->GraphDef[I].ColorR, Gr->GraphDef[I].ColorG, Gr->GraphDef[I].ColorB, LightAngle);
                                    }
                                    else
                                    {
                                        DrawTriangle0(DrawX00, DrawY00, DrawZ00, DrawX0, DrawY0, DrawZ0, DrawX, DrawY, DrawZ, Gr->GraphDef[I].ColorR00, Gr->GraphDef[I].ColorG00, Gr->GraphDef[I].ColorB00, Gr->GraphDef[I].ColorR0, Gr->GraphDef[I].ColorG0, Gr->GraphDef[I].ColorB0, Gr->GraphDef[I].ColorR, Gr->GraphDef[I].ColorG, Gr->GraphDef[I].ColorB, LightAngle);
                                    }
                                }
                                if (!InverseDraw)
                                {
                                    if (PlotSizeL >= 0)
                                    {
                                        if (DrawMode == 3)
                                        {
                                            DrawLine(DrawX0, DrawY0, DrawZ0, DrawX, DrawY, DrawZ, Gr->GraphDef[I].ColorR, Gr->GraphDef[I].ColorG, Gr->GraphDef[I].ColorB, LightAngle);
                                            DrawLine(DrawX00, DrawY00, DrawZ00, DrawX0, DrawY0, DrawZ0, Gr->GraphDef[I].ColorR, Gr->GraphDef[I].ColorG, Gr->GraphDef[I].ColorB, LightAngle);
                                            DrawLine(DrawX, DrawY, DrawZ, DrawX00, DrawY00, DrawZ00, Gr->GraphDef[I].ColorR, Gr->GraphDef[I].ColorG, Gr->GraphDef[I].ColorB, LightAngle);
                                        }
                                        else
                                        {
                                            DrawLine0(DrawX0, DrawY0, DrawZ0, DrawX, DrawY, DrawZ, Gr->GraphDef[I].ColorR0, Gr->GraphDef[I].ColorG0, Gr->GraphDef[I].ColorB0, Gr->GraphDef[I].ColorR, Gr->GraphDef[I].ColorG, Gr->GraphDef[I].ColorB, LightAngle);
                                            DrawLine0(DrawX00, DrawY00, DrawZ00, DrawX0, DrawY0, DrawZ0, Gr->GraphDef[I].ColorR00, Gr->GraphDef[I].ColorG00, Gr->GraphDef[I].ColorB00, Gr->GraphDef[I].ColorR0, Gr->GraphDef[I].ColorG0, Gr->GraphDef[I].ColorB0, LightAngle);
                                            DrawLine0(DrawX, DrawY, DrawZ, DrawX00, DrawY00, DrawZ00, Gr->GraphDef[I].ColorR, Gr->GraphDef[I].ColorG, Gr->GraphDef[I].ColorB, Gr->GraphDef[I].ColorR00, Gr->GraphDef[I].ColorG00, Gr->GraphDef[I].ColorB00, LightAngle);
                                        }
                                    }
                                    if (PlotSizeP >= 0)
                                    {
                                        DrawPoint(DrawX, DrawY, DrawZ, Gr->GraphDef[I].ColorR, Gr->GraphDef[I].ColorG, Gr->GraphDef[I].ColorB, LightAngle);
                                        DrawPoint(DrawX0, DrawY0, DrawZ0, Gr->GraphDef[I].ColorR0, Gr->GraphDef[I].ColorG0, Gr->GraphDef[I].ColorB0, LightAngle);
                                        DrawPoint(DrawX00, DrawY00, DrawZ00, Gr->GraphDef[I].ColorR00, Gr->GraphDef[I].ColorG00, Gr->GraphDef[I].ColorB00, LightAngle);
                                    }
                                }
                            }
                            else
                            {
                                DrawX00 = 0;
                                DrawY00 = 0;
                                DrawZ00 = 0;
                                if (LightMode == 2)
                                {
                                    double DrawX_ = DrawX - DrawX0;
                                    double DrawY_ = DrawY - DrawY0;
                                    double DrawZ_ = DrawZ - DrawZ0;
                                    LightAngle = VectorAngle(DrawX_, DrawY_, DrawZ_, LightVectorX, LightVectorY, LightVectorZ);
                                    if (LightAngle > M_PI_2)
                                    {
                                        LightAngle = M_PI - LightAngle;
                                    }
                                    LightAngle = M_PI_2 - LightAngle;
                                }

                                if (InverseDraw)
                                {
                                    if (PlotSizeP >= 0)
                                    {
                                        DrawPoint(DrawX, DrawY, DrawZ, Gr->GraphDef[I].ColorR, Gr->GraphDef[I].ColorG, Gr->GraphDef[I].ColorB, LightAngle);
                                        DrawPoint(DrawX0, DrawY0, DrawZ0, Gr->GraphDef[I].ColorR0, Gr->GraphDef[I].ColorG0, Gr->GraphDef[I].ColorB0, LightAngle);
                                    }
                                }
                                if (PlotSizeL >= 0)
                                {
                                    if (DrawMode == 1)
                                    {
                                        DrawLine(DrawX0, DrawY0, DrawZ0, DrawX, DrawY, DrawZ, Gr->GraphDef[I].ColorR, Gr->GraphDef[I].ColorG, Gr->GraphDef[I].ColorB, LightAngle);
                                    }
                                    else
                                    {
                                        DrawLine0(DrawX0, DrawY0, DrawZ0, DrawX, DrawY, DrawZ, Gr->GraphDef[I].ColorR0, Gr->GraphDef[I].ColorG0, Gr->GraphDef[I].ColorB0, Gr->GraphDef[I].ColorR, Gr->GraphDef[I].ColorG, Gr->GraphDef[I].ColorB, LightAngle);
                                    }
                                }
                                if (!InverseDraw)
                                {
                                    if (PlotSizeP >= 0)
                                    {
                                        DrawPoint(DrawX, DrawY, DrawZ, Gr->GraphDef[I].ColorR, Gr->GraphDef[I].ColorG, Gr->GraphDef[I].ColorB, LightAngle);
                                        DrawPoint(DrawX0, DrawY0, DrawZ0, Gr->GraphDef[I].ColorR0, Gr->GraphDef[I].ColorG0, Gr->GraphDef[I].ColorB0, LightAngle);
                                    }
                                }

                            }
                        }
                        else
                        {
                            if (PlotSizeP >= 0)
                            {
                                DrawPoint0(DrawX, DrawY, DrawZ, Gr->GraphDef[I].ColorR, Gr->GraphDef[I].ColorG, Gr->GraphDef[I].ColorB);
                            }
                        }

                    }
                }
            }
            RefreshNum = L;
            if (ToFile)
            {
                Img->save(Eden::ToQStr(FileName), "PNG");
                if (ui->GraphScreen->ImgX != ImgX)
                {
                    if (ImgX != NULL)
                    {
                        DEL(QImage, ImgX);
                    }
                }
            }
            else
            {
                ui->GraphScreen->ImgX = Img;
                ui->GraphScreen->repaint();
                if (ImgX != NULL)
                {
                    DEL(QImage, ImgX);
                }
            }
        }
        else
        {
            if (Img != NULL)
            {
                ui->GraphScreen->ImgX = NULL;
                ui->GraphScreen->repaint();
                DEL(QImage, Img);
                Img = NULL;
            }
        }
    }
    ui->GraphScreen->repaint();
}

void IOGraphWin::on_GraphClear_clicked()
{
    Core->IOGraph_[CurrentIO]->Clear();
}

void IOGraphWin::on_GraphColor_clicked()
{
    QColor C;
    C.setRgb(BackColorR, BackColorG, BackColorB);
    C = QColorDialog::getColor(C, this, "", 0);
    BackColorR = C.red();
    BackColorG = C.green();
    BackColorB = C.blue();
    BackColorRI = 255 - C.red();
    BackColorGI = 255 - C.green();
    BackColorBI = 255 - C.blue();
    RefreshNum = -1;
    RefreshParams(0);
    Refresh(false, "", 0, 0);
}

void IOGraphWin::on_GraphPlotSizeP_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_GraphPlotSizeL_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_GraphPlotSurface_toggled(bool checked) { RefreshParams(checked); }
void IOGraphWin::on_GraphPlotText_toggled(bool checked){ RefreshParams(checked); }
void IOGraphWin::on_GraphInv_toggled(bool checked) { RefreshParams(checked); }

void IOGraphWin::on_GrScaleXX_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_GrScaleXY_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_GrScaleXZ_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_GrScaleYX_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_GrScaleYY_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_GrScaleYZ_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_GrScaleZX_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_GrScaleZY_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_GrScaleZZ_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_GrScaleX_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_GrScaleY_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_GrScaleZ_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_GrOffset1X_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_GrOffset1Y_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_GrOffset1Z_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_GrOffset2X_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_GrOffset2Y_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_GrOffset2Z_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_Rot1Order_currentIndexChanged(int index) { RefreshParams(index); }
void IOGraphWin::on_Rot1X_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_Rot1Y_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_Rot1Z_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_Rot2Order_currentIndexChanged(int index) { RefreshParams(index); }
void IOGraphWin::on_Rot2X_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_Rot2Y_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_Rot2Z_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_LightMode_currentIndexChanged(int index) { RefreshParams(index); }
void IOGraphWin::on_LightAngleHT_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_LightAngleVT_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_LightDist0T_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_LightDist1T_valueChanged(int arg1) { RefreshParams(arg1); }
void IOGraphWin::on_LightDist2T_valueChanged(int arg1) { RefreshParams(arg1); }


void IOGraphWin::RefreshParams(int Dummy)
{
    if (!RefreshEnabled)
    {
        return;
    }
    RefreshEnabled = false;


    PlotText = ui->GraphPlotText->isChecked();
    InverseDraw = ui->GraphInv->isChecked();

    Dummy++;

    if (ui->GraphPlotSizeP->value() > 0)
    {
        PlotPointSquare = false;
        PlotSizeP = ui->GraphPlotSizeP->value() - 1;
    }
    else
    {
        PlotPointSquare = true;
        PlotSizeP = (0 - ui->GraphPlotSizeP->value()) - 1;
    }
    PlotSizeP2 = PlotSizeP & 1;
    PlotSizeP = PlotSizeP >> 1;
    if (ui->GraphPlotSizeL->value() > 0)
    {
        PlotSizeL = ui->GraphPlotSizeL->value() - 1;
        PlotLineCross = false;
    }
    else
    {
        PlotSizeL = (0 - ui->GraphPlotSizeL->value()) - 1;
        PlotLineCross = true;
    }
    PlotSizeL2 = PlotSizeL & 1;
    PlotSizeL = PlotSizeL >> 1;
    PlotSurface = ui->GraphPlotSurface->isChecked();
    if (PlotSizeP >= 0)
    {
        if (PlotPointSquare)
        {
            for (int YY = 0; YY <= MaxPlotWidth; YY++)
            {
                for (int XX = 0; XX <= MaxPlotWidth; XX++)
                {
                    Mask[XX][YY] = 1;
                }
            }
        }
        else
        {
            for (int YY = 0; YY <= MaxPlotWidth; YY++)
            {
                for (int XX = 0; XX <= MaxPlotWidth; XX++)
                {
                    Mask[XX][YY] = MaskAll[PlotSizeP][XX][YY];
                }
            }
        }
    }
    else
    {
        for (int YY = 0; YY <= MaxPlotWidth; YY++)
        {
            for (int XX = 0; XX <= MaxPlotWidth; XX++)
            {
                Mask[XX][YY] = 0;
            }
        }
    }


    int BoundX = (((double)((PlotSizeL + PlotSizeL + PlotSizeL2) << 1)) / sqrt(2));
    //BoundX = PlotSizeL + PlotSizeL + PlotSizeL2;
    int BoundXX = BoundX >> 1;
    PlotSizeDiagonal1 = (BoundXX >> 1) + (BoundXX & 1);
    PlotSizeDiagonal2 = BoundXX >> 1;
    PlotSizeDiagonal3 = PlotSizeDiagonal1 - 1;
    PlotSizeDiagonal4 = PlotSizeDiagonal2;
    if ((BoundXX & 1))
    {
        PlotSizeDiagonal4 += ((BoundX & 1));
    }
    else
    {
        PlotSizeDiagonal3 += ((BoundX & 1));
    }



    DrawXtoX = ui->GrScaleXX->value() * ui->GrScaleX->value(); DrawXtoX /= 1000000.0;
    DrawXtoY = ui->GrScaleXY->value() * ui->GrScaleX->value(); DrawXtoY /= 1000000.0;
    DrawXtoZ = ui->GrScaleXZ->value() * ui->GrScaleX->value(); DrawXtoZ /= 1000000.0;
    DrawYtoX = ui->GrScaleYX->value() * ui->GrScaleY->value(); DrawYtoX /= 1000000.0;
    DrawYtoY = ui->GrScaleYY->value() * ui->GrScaleY->value(); DrawYtoY /= 1000000.0;
    DrawYtoZ = ui->GrScaleYZ->value() * ui->GrScaleY->value(); DrawYtoZ /= 1000000.0;
    DrawZtoX = ui->GrScaleZX->value() * ui->GrScaleZ->value(); DrawZtoX /= 1000000.0;
    DrawZtoY = ui->GrScaleZY->value() * ui->GrScaleZ->value(); DrawZtoY /= 1000000.0;
    DrawZtoZ = ui->GrScaleZZ->value() * ui->GrScaleZ->value(); DrawZtoZ /= 1000000.0;

    while (ui->Rot1X->value() <= (0 - 180)) { ui->Rot1X->setValue(ui->Rot1X->value() + 360); }
    while (ui->Rot1X->value() > (0 + 180)) { ui->Rot1X->setValue(ui->Rot1X->value() - 360); }
    while (ui->Rot1Y->value() <= (0 - 180)) { ui->Rot1Y->setValue(ui->Rot1Y->value() + 360); }
    while (ui->Rot1Y->value() > (0 + 180)) { ui->Rot1Y->setValue(ui->Rot1Y->value() - 360); }
    while (ui->Rot1Z->value() <= (0 - 180)) { ui->Rot1Z->setValue(ui->Rot1Z->value() + 360); }
    while (ui->Rot1Z->value() > (0 + 180)) { ui->Rot1Z->setValue(ui->Rot1Z->value() - 360); }
    while (ui->Rot2X->value() <= (0 - 180)) { ui->Rot2X->setValue(ui->Rot2X->value() + 360); }
    while (ui->Rot2X->value() > (0 + 180)) { ui->Rot2X->setValue(ui->Rot2X->value() - 360); }
    while (ui->Rot2Y->value() <= (0 - 180)) { ui->Rot2Y->setValue(ui->Rot2Y->value() + 360); }
    while (ui->Rot2Y->value() > (0 + 180)) { ui->Rot2Y->setValue(ui->Rot2Y->value() - 360); }
    while (ui->Rot2Z->value() <= (0 - 180)) { ui->Rot2Z->setValue(ui->Rot2Z->value() + 360); }
    while (ui->Rot2Z->value() > (0 + 180)) { ui->Rot2Z->setValue(ui->Rot2Z->value() - 360); }
    while (ui->LightAngleHT->value() <= (0 - 180)) { ui->LightAngleHT->setValue(ui->LightAngleHT->value() + 360); }
    while (ui->LightAngleHT->value() > (0 + 180)) { ui->LightAngleHT->setValue(ui->LightAngleHT->value() - 360); }
    while (ui->LightAngleVT->value() <= (0 - 180)) { ui->LightAngleVT->setValue(ui->LightAngleVT->value() + 360); }
    while (ui->LightAngleVT->value() > (0 + 180)) { ui->LightAngleVT->setValue(ui->LightAngleVT->value() - 360); }

    DimensionType Val_;
    Val_ = ui->Rot1X->value(); Val_ = Val_ * M_PI / 180.0; Angle1X = Val_;
    Val_ = ui->Rot1Y->value(); Val_ = Val_ * M_PI / 180.0; Angle1Y = Val_;
    Val_ = ui->Rot1Z->value(); Val_ = Val_ * M_PI / 180.0; Angle1Z = Val_;
    Val_ = ui->Rot2X->value(); Val_ = Val_ * M_PI / 180.0; Angle2X = 0.0 - Val_;
    Val_ = ui->Rot2Y->value(); Val_ = Val_ * M_PI / 180.0; Angle2Y = 0.0 - Val_;
    Val_ = ui->Rot2Z->value(); Val_ = Val_ * M_PI / 180.0; Angle2Z = 0.0 - Val_;

    switch (ui->Rot1Order->currentIndex())
    {
        case 0:
            RotateScale(Angle1X, DrawYtoX, DrawZtoX);
            RotateScale(Angle1X, DrawYtoY, DrawZtoY);
            RotateScale(Angle1X, DrawYtoZ, DrawZtoZ);
            RotateScale(Angle1Y, DrawZtoX, DrawXtoX);
            RotateScale(Angle1Y, DrawZtoY, DrawXtoY);
            RotateScale(Angle1Y, DrawZtoZ, DrawXtoZ);
            RotateScale(Angle1Z, DrawXtoX, DrawYtoX);
            RotateScale(Angle1Z, DrawXtoY, DrawYtoY);
            RotateScale(Angle1Z, DrawXtoZ, DrawYtoZ);
            break;
        case 1:
            RotateScale(Angle1X, DrawYtoX, DrawZtoX);
            RotateScale(Angle1X, DrawYtoY, DrawZtoY);
            RotateScale(Angle1X, DrawYtoZ, DrawZtoZ);
            RotateScale(Angle1Z, DrawXtoX, DrawYtoX);
            RotateScale(Angle1Z, DrawXtoY, DrawYtoY);
            RotateScale(Angle1Z, DrawXtoZ, DrawYtoZ);
            RotateScale(Angle1Y, DrawZtoX, DrawXtoX);
            RotateScale(Angle1Y, DrawZtoY, DrawXtoY);
            RotateScale(Angle1Y, DrawZtoZ, DrawXtoZ);
            break;
        case 2:
            RotateScale(Angle1Y, DrawZtoX, DrawXtoX);
            RotateScale(Angle1Y, DrawZtoY, DrawXtoY);
            RotateScale(Angle1Y, DrawZtoZ, DrawXtoZ);
            RotateScale(Angle1Z, DrawXtoX, DrawYtoX);
            RotateScale(Angle1Z, DrawXtoY, DrawYtoY);
            RotateScale(Angle1Z, DrawXtoZ, DrawYtoZ);
            RotateScale(Angle1X, DrawYtoX, DrawZtoX);
            RotateScale(Angle1X, DrawYtoY, DrawZtoY);
            RotateScale(Angle1X, DrawYtoZ, DrawZtoZ);
            break;
        case 3:
            RotateScale(Angle1Y, DrawZtoX, DrawXtoX);
            RotateScale(Angle1Y, DrawZtoY, DrawXtoY);
            RotateScale(Angle1Y, DrawZtoZ, DrawXtoZ);
            RotateScale(Angle1X, DrawYtoX, DrawZtoX);
            RotateScale(Angle1X, DrawYtoY, DrawZtoY);
            RotateScale(Angle1X, DrawYtoZ, DrawZtoZ);
            RotateScale(Angle1Z, DrawXtoX, DrawYtoX);
            RotateScale(Angle1Z, DrawXtoY, DrawYtoY);
            RotateScale(Angle1Z, DrawXtoZ, DrawYtoZ);
            break;
        case 4:
            RotateScale(Angle1Z, DrawXtoX, DrawYtoX);
            RotateScale(Angle1Z, DrawXtoY, DrawYtoY);
            RotateScale(Angle1Z, DrawXtoZ, DrawYtoZ);
            RotateScale(Angle1X, DrawYtoX, DrawZtoX);
            RotateScale(Angle1X, DrawYtoY, DrawZtoY);
            RotateScale(Angle1X, DrawYtoZ, DrawZtoZ);
            RotateScale(Angle1Y, DrawZtoX, DrawXtoX);
            RotateScale(Angle1Y, DrawZtoY, DrawXtoY);
            RotateScale(Angle1Y, DrawZtoZ, DrawXtoZ);
            break;
        case 5:
            RotateScale(Angle1Z, DrawXtoX, DrawYtoX);
            RotateScale(Angle1Z, DrawXtoY, DrawYtoY);
            RotateScale(Angle1Z, DrawXtoZ, DrawYtoZ);
            RotateScale(Angle1Y, DrawZtoX, DrawXtoX);
            RotateScale(Angle1Y, DrawZtoY, DrawXtoY);
            RotateScale(Angle1Y, DrawZtoZ, DrawXtoZ);
            RotateScale(Angle1X, DrawYtoX, DrawZtoX);
            RotateScale(Angle1X, DrawYtoY, DrawZtoY);
            RotateScale(Angle1X, DrawYtoZ, DrawZtoZ);
            break;
    }

    switch (ui->Rot2Order->currentIndex())
    {
        case 0:
            RotateScale(Angle2X, DrawXtoY, DrawXtoZ);
            RotateScale(Angle2X, DrawYtoY, DrawYtoZ);
            RotateScale(Angle2X, DrawZtoY, DrawZtoZ);
            RotateScale(Angle2Y, DrawXtoZ, DrawXtoX);
            RotateScale(Angle2Y, DrawYtoZ, DrawYtoX);
            RotateScale(Angle2Y, DrawZtoZ, DrawZtoX);
            RotateScale(Angle2Z, DrawXtoX, DrawXtoY);
            RotateScale(Angle2Z, DrawYtoX, DrawYtoY);
            RotateScale(Angle2Z, DrawZtoX, DrawZtoY);
            break;
        case 1:
            RotateScale(Angle2X, DrawXtoY, DrawXtoZ);
            RotateScale(Angle2X, DrawYtoY, DrawYtoZ);
            RotateScale(Angle2X, DrawZtoY, DrawZtoZ);
            RotateScale(Angle2Z, DrawXtoX, DrawXtoY);
            RotateScale(Angle2Z, DrawYtoX, DrawYtoY);
            RotateScale(Angle2Z, DrawZtoX, DrawZtoY);
            RotateScale(Angle2Y, DrawXtoZ, DrawXtoX);
            RotateScale(Angle2Y, DrawYtoZ, DrawYtoX);
            RotateScale(Angle2Y, DrawZtoZ, DrawZtoX);
            break;
        case 2:
            RotateScale(Angle2Y, DrawXtoZ, DrawXtoX);
            RotateScale(Angle2Y, DrawYtoZ, DrawYtoX);
            RotateScale(Angle2Y, DrawZtoZ, DrawZtoX);
            RotateScale(Angle2Z, DrawXtoX, DrawXtoY);
            RotateScale(Angle2Z, DrawYtoX, DrawYtoY);
            RotateScale(Angle2Z, DrawZtoX, DrawZtoY);
            RotateScale(Angle2X, DrawXtoY, DrawXtoZ);
            RotateScale(Angle2X, DrawYtoY, DrawYtoZ);
            RotateScale(Angle2X, DrawZtoY, DrawZtoZ);
            break;
        case 3:
            RotateScale(Angle2Y, DrawXtoZ, DrawXtoX);
            RotateScale(Angle2Y, DrawYtoZ, DrawYtoX);
            RotateScale(Angle2Y, DrawZtoZ, DrawZtoX);
            RotateScale(Angle2X, DrawXtoY, DrawXtoZ);
            RotateScale(Angle2X, DrawYtoY, DrawYtoZ);
            RotateScale(Angle2X, DrawZtoY, DrawZtoZ);
            RotateScale(Angle2Z, DrawXtoX, DrawXtoY);
            RotateScale(Angle2Z, DrawYtoX, DrawYtoY);
            RotateScale(Angle2Z, DrawZtoX, DrawZtoY);
            break;
        case 4:
            RotateScale(Angle2Z, DrawXtoX, DrawXtoY);
            RotateScale(Angle2Z, DrawYtoX, DrawYtoY);
            RotateScale(Angle2Z, DrawZtoX, DrawZtoY);
            RotateScale(Angle2X, DrawXtoY, DrawXtoZ);
            RotateScale(Angle2X, DrawYtoY, DrawYtoZ);
            RotateScale(Angle2X, DrawZtoY, DrawZtoZ);
            RotateScale(Angle2Y, DrawXtoZ, DrawXtoX);
            RotateScale(Angle2Y, DrawYtoZ, DrawYtoX);
            RotateScale(Angle2Y, DrawZtoZ, DrawZtoX);
            break;
        case 5:
            RotateScale(Angle2Z, DrawXtoX, DrawXtoY);
            RotateScale(Angle2Z, DrawYtoX, DrawYtoY);
            RotateScale(Angle2Z, DrawZtoX, DrawZtoY);
            RotateScale(Angle2Y, DrawXtoZ, DrawXtoX);
            RotateScale(Angle2Y, DrawYtoZ, DrawYtoX);
            RotateScale(Angle2Y, DrawZtoZ, DrawZtoX);
            RotateScale(Angle2X, DrawXtoY, DrawXtoZ);
            RotateScale(Angle2X, DrawYtoY, DrawYtoZ);
            RotateScale(Angle2X, DrawZtoY, DrawZtoZ);
            break;
    }

    OffsetX = ui->GrOffset1X->value(); OffsetX /= 1000.0;
    OffsetY = ui->GrOffset1Y->value(); OffsetY /= 1000.0;
    OffsetZ = ui->GrOffset1Z->value(); OffsetZ /= 1000.0;

    OffsetXRender = ui->GrOffset2X->value();
    OffsetYRender = ui->GrOffset2Y->value();
    OffsetZRender = ui->GrOffset2Z->value();

    CalcColorInit();

    RefreshNum = -1;

    RefreshEnabled = true;
}

void IOGraphWin::RotateScale(double &Angle, double &ScaleX, double &ScaleY)
{
    DimensionType Dist = (ScaleX * ScaleX) + (ScaleY * ScaleY);
    DimensionType AngDiff = atan2f(ScaleY, ScaleX);
    Dist = sqrt(Dist);
    AngDiff += Angle;
    ScaleX = cosf(AngDiff) * Dist;
    ScaleY = sinf(AngDiff) * Dist;
}

void IOGraphWin::on_ValueStepT_valueChanged(int arg1)
{
    ui->GrScaleXX->setSingleStep(arg1);
    ui->GrScaleXY->setSingleStep(arg1);
    ui->GrScaleXZ->setSingleStep(arg1);
    ui->GrScaleYX->setSingleStep(arg1);
    ui->GrScaleYY->setSingleStep(arg1);
    ui->GrScaleYZ->setSingleStep(arg1);
    ui->GrScaleZX->setSingleStep(arg1);
    ui->GrScaleZY->setSingleStep(arg1);
    ui->GrScaleZZ->setSingleStep(arg1);
    ui->GrScaleX->setSingleStep(arg1);
    ui->GrScaleY->setSingleStep(arg1);
    ui->GrScaleZ->setSingleStep(arg1);
    ui->GrOffset1X->setSingleStep(arg1);
    ui->GrOffset1Y->setSingleStep(arg1);
    ui->GrOffset1Z->setSingleStep(arg1);
    ui->GrOffset2X->setSingleStep(arg1);
    ui->GrOffset2Y->setSingleStep(arg1);
    ui->GrOffset2Z->setSingleStep(arg1);
    ui->Rot1X->setSingleStep(arg1);
    ui->Rot1Y->setSingleStep(arg1);
    ui->Rot1Z->setSingleStep(arg1);
    ui->Rot2X->setSingleStep(arg1);
    ui->Rot2Y->setSingleStep(arg1);
    ui->Rot2Z->setSingleStep(arg1);
    ui->LightAngleHT->setSingleStep(arg1);
    ui->LightAngleVT->setSingleStep(arg1);
    ui->LightDist0T->setSingleStep(arg1);
    ui->LightDist1T->setSingleStep(arg1);
    ui->LightDist2T->setSingleStep(arg1);
}

void IOGraphWin::on_RenderFile_clicked()
{
    string FileName = "";
    QString FileNameX = QFileDialog::getSaveFileName(this, "Save image", Core->LastPath, "Files (*.png)");
    Core->SaveLastPath(FileNameX, false);
    if (!FileNameX.isEmpty())
    {
        FileName = Eden::ToStr(FileNameX);
    }

    if (FileName != "")
    {
        FileName = Eden::FileExtension(FileName, "png");
        int W = Eden::ToInt(Core->InputBox(this, "Image width", "Render image", Eden::ToStr(ui->GraphScreen->width())));
        int H = Eden::ToInt(Core->InputBox(this, "Image height", "Render image", Eden::ToStr(ui->GraphScreen->height())));
        Refresh(true, FileName, W, H);
    }
}

void IOGraphWin::on_ShowHide1_toggled(bool checked)
{
    ui->GraphClear->setVisible(checked);
    ui->GraphColor->setVisible(checked);
    ui->GraphPlotSizePL->setVisible(checked);
    ui->GraphPlotSizeP->setVisible(checked);
    ui->GraphPlotSizeLL->setVisible(checked);
    ui->GraphPlotSizeL->setVisible(checked);
    ui->GraphPlotSurface->setVisible(checked);
    ui->GraphPlotText->setVisible(checked);
    ui->GraphInv->setVisible(checked);
    ui->ValueStepL->setVisible(checked);
    ui->ValueStepT->setVisible(checked);
    ui->RenderFile->setVisible(checked);
    RefreshParams(0);
}

void IOGraphWin::on_ShowHide2_toggled(bool checked)
{
    ui->GrScaleXL->setVisible(checked);
    ui->GrScaleXX->setVisible(checked);
    ui->GrScaleXY->setVisible(checked);
    ui->GrScaleXZ->setVisible(checked);
    ui->GrScaleYL->setVisible(checked);
    ui->GrScaleYX->setVisible(checked);
    ui->GrScaleYY->setVisible(checked);
    ui->GrScaleYZ->setVisible(checked);
    ui->GrScaleZL->setVisible(checked);
    ui->GrScaleZX->setVisible(checked);
    ui->GrScaleZY->setVisible(checked);
    ui->GrScaleZZ->setVisible(checked);
    ui->GrScaleL->setVisible(checked);
    ui->GrScaleX->setVisible(checked);
    ui->GrScaleY->setVisible(checked);
    ui->GrScaleZ->setVisible(checked);
    ui->GrOffset1L->setVisible(checked);
    ui->GrOffset1X->setVisible(checked);
    ui->GrOffset1Y->setVisible(checked);
    ui->GrOffset1Z->setVisible(checked);
    ui->GrOffset2L->setVisible(checked);
    ui->GrOffset2X->setVisible(checked);
    ui->GrOffset2Y->setVisible(checked);
    ui->GrOffset2Z->setVisible(checked);
    RefreshParams(0);
}

void IOGraphWin::on_ShowHide3_toggled(bool checked)
{
    ui->Rot1L->setVisible(checked);
    ui->Rot1Order->setVisible(checked);
    ui->Rot1X->setVisible(checked);
    ui->Rot1Y->setVisible(checked);
    ui->Rot1Z->setVisible(checked);
    ui->Rot2L->setVisible(checked);
    ui->Rot2Order->setVisible(checked);
    ui->Rot2X->setVisible(checked);
    ui->Rot2Y->setVisible(checked);
    ui->Rot2Z->setVisible(checked);
    RefreshParams(0);
}

void IOGraphWin::on_ShowHide4_toggled(bool checked)
{
    ui->LightL->setVisible(checked);
    ui->LightMode->setVisible(checked);
    ui->LightAngleHL->setVisible(checked);
    ui->LightAngleHT->setVisible(checked);
    ui->LightAngleVL->setVisible(checked);
    ui->LightAngleVT->setVisible(checked);
    ui->LightDist0L->setVisible(checked);
    ui->LightDist0T->setVisible(checked);
    ui->LightDist1L->setVisible(checked);
    ui->LightDist1T->setVisible(checked);
    ui->LightDist2L->setVisible(checked);
    ui->LightDist2T->setVisible(checked);
    RefreshParams(0);
}
