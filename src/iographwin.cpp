#include "iographwin.h"
#include "ui_iographwin.h"

IOGraphWin::IOGraphWin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IOGraphWin)
{
    BackColorR = 0;
    BackColorG = 0;
    BackColorB = 0;

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
    RefreshParams();
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
    Refresh();
}

void IOGraphWin::on_Show1_clicked()
{
    setWindowTitle("Graph 1");
    CurrentIOX = -1;
    CurrentIO = 1;
    Refresh();
}

void IOGraphWin::on_Show2_clicked()
{
    setWindowTitle("Graph 2");
    CurrentIOX = -1;
    CurrentIO = 2;
    Refresh();
}

void IOGraphWin::on_Show3_clicked()
{
    setWindowTitle("Graph 3");
    CurrentIOX = -1;
    CurrentIO = 3;
    Refresh();
}

void IOGraphWin::DrawPoint(int X, int Y, int Z, uchar R, uchar G, uchar B)
{
    int DrawX = X + ZeroX;
    int DrawY = ZeroY - Y;
    int DrawZ = Z + ZeroZ;

    if ((DrawX >= BoundLP) && (DrawY >= BoundTP) && (DrawX <= BoundRP) && (DrawY <= BoundBP))
    {
        int Pos = (DrawY * ImgW) + DrawX;
        int XX, YY, PosX, Pos4X;
        for (YY = 0; YY <= PlotSizeP; YY++)
        {
            for (XX = 0; XX <= PlotSizeP; XX++)
            {
                if (Mask[XX][YY])
                {
                    PosX = Pos + (ImgW * (YY)) + XX;
                    if (ImgZ[PosX] <= DrawZ) { ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B; ImgRaw[Pos4X + 1] = G; ImgRaw[Pos4X + 2] = R; }

                    PosX = Pos + (ImgW * (YY)) - XX - PlotSizeP2;
                    if (ImgZ[PosX] <= DrawZ) { ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B; ImgRaw[Pos4X + 1] = G; ImgRaw[Pos4X + 2] = R; }

                    PosX = Pos - (ImgW * (YY + PlotSizeP2)) + XX;
                    if (ImgZ[PosX] <= DrawZ) { ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B; ImgRaw[Pos4X + 1] = G; ImgRaw[Pos4X + 2] = R; }

                    PosX = Pos - (ImgW * (YY + PlotSizeP2)) - XX - PlotSizeP2;
                    if (ImgZ[PosX] <= DrawZ) { ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B; ImgRaw[Pos4X + 1] = G; ImgRaw[Pos4X + 2] = R; }
                }
            }
        }
    }
}

void IOGraphWin::DrawPointH(int X, int Y, int Z, uchar R, uchar G, uchar B)
{
    int DrawX = X + ZeroX;
    int DrawY = ZeroY - Y;
    int DrawZ = Z + ZeroZ;

    if ((DrawX >= BoundLL) && (DrawY >= 0) && (DrawX <= BoundRL) && (DrawY <= ImgH1))
    {
        int Pos = (DrawY * ImgW) + DrawX;
        int XX, PosX, Pos4X;
        for (XX = 0 - PlotSizeL - PlotSizeL2; XX <= PlotSizeL; XX++)
        {
            PosX = Pos + XX;
            if (ImgZ[PosX] <= DrawZ) { ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B; ImgRaw[Pos4X + 1] = G; ImgRaw[Pos4X + 2] = R; }
        }
    }
}

void IOGraphWin::DrawPointV(int X, int Y, int Z, uchar R, uchar G, uchar B)
{
    int DrawX = X + ZeroX;
    int DrawY = ZeroY - Y;
    int DrawZ = Z + ZeroZ;

    if ((DrawX >= 0) && (DrawY >= BoundTL) && (DrawX <= ImgW1) && (DrawY <= BoundBL))
    {
        int Pos = (DrawY * ImgW) + DrawX;
        int YY, PosX, Pos4X;
        for (YY = 0 - PlotSizeL - PlotSizeL2; YY <= PlotSizeL; YY++)
        {
            PosX = Pos + (ImgW * YY);
            if (ImgZ[PosX] <= DrawZ) { ImgZ[PosX] = DrawZ; Pos4X = PosX << 2; ImgRaw[Pos4X + 0] = B; ImgRaw[Pos4X + 1] = G; ImgRaw[Pos4X + 2] = R; }
        }
    }
}

void IOGraphWin::DrawLine(int X0, int Y0, int Z0, int X1, int Y1, int Z1, uchar R, uchar G, uchar B)
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
    if (dx > dy)
    {
        rob = dx;

        (dx != 0) ? tempx = (dx0 / rob) : tempx = 0;
        (dy != 0) ? tempy = (dy0 / rob) : tempy = 0;
        (dz != 0) ? tempz = (dz0 / rob) : tempz = 0;
        while (rob != 0)
        {
            x1 = x1 + tempx;
            y1 = y1 + tempy;
            z1 = z1 + tempz;
            x = floor(x1 + 0.5);
            y = floor(y1 + 0.5);
            z = floor(z1 + 0.5);
            DrawPointV(x, y, z, R, G, B);
            rob--;
        }
    }
    else
    {
        rob = dy;

        (dx != 0) ? tempx = (dx0 / rob) : tempx = 0;
        (dy != 0) ? tempy = (dy0 / rob) : tempy = 0;
        (dz != 0) ? tempz = (dz0 / rob) : tempz = 0;
        while (rob != 0)
        {
            x1 = x1 + tempx;
            y1 = y1 + tempy;
            z1 = z1 + tempz;
            x = floor(x1 + 0.5);
            y = floor(y1 + 0.5);
            z = floor(z1 + 0.5);
            DrawPointH(x, y, z, R, G, B);
            rob--;
        }
    }
}

void IOGraphWin::DrawLine0(int X0, int Y0, int Z0, int X1, int Y1, int Z1, int R0, int G0, int B0, int R1, int G1, int B1)
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
    if (dx > dy)
    {
        rob = dx;

        (dx != 0) ? tempx = (dx0 / rob) : tempx = 0;
        (dy != 0) ? tempy = (dy0 / rob) : tempy = 0;
        (dz != 0) ? tempz = (dz0 / rob) : tempz = 0;
        (dr != 0) ? tempr = (dr0 / rob) : tempr = 0;
        (dg != 0) ? tempg = (dg0 / rob) : tempg = 0;
        (db != 0) ? tempb = (db0 / rob) : tempb = 0;
        while (rob != 0)
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
            DrawPointV(x, y, z, r, g, b);
            rob--;
        }
    }
    else
    {
        rob = dy;

        (dx != 0) ? tempx = (dx0 / rob) : tempx = 0;
        (dy != 0) ? tempy = (dy0 / rob) : tempy = 0;
        (dz != 0) ? tempz = (dz0 / rob) : tempz = 0;
        (dr != 0) ? tempr = (dr0 / rob) : tempr = 0;
        (dg != 0) ? tempg = (dg0 / rob) : tempg = 0;
        (db != 0) ? tempb = (db0 / rob) : tempb = 0;
        while (rob != 0)
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
            DrawPointH(x, y, z, r, g, b);
            rob--;
        }
    }
}


void IOGraphWin::Refresh()
{
    int ScrW = ui->GraphScreen->width();
    int ScrH = ui->GraphScreen->height();
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
            if ((ImgW != ScrW) || (ImgH != ScrH) || (Img == NULL))
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

            DimensionType X, Y, Z, X0, Y0, Z0;
            int DrawX, DrawY, DrawZ, DrawX0, DrawY0, DrawZ0;
            char LineMode;

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

            for (int I = RefreshNum; I < L; I++)
            {
                X = Gr->GraphDef[I].X - OffsetX;
                Y = Gr->GraphDef[I].Y - OffsetY;
                Z = Gr->GraphDef[I].Z - OffsetZ;
                DrawX = round((X * DrawXtoX) + (Y * DrawYtoX) + (Z * DrawZtoX));
                DrawY = round((X * DrawXtoY) + (Y * DrawYtoY) + (Z * DrawZtoY));
                DrawZ = round((X * DrawXtoZ) + (Y * DrawYtoZ) + (Z * DrawZtoZ));

                LineMode = Gr->GraphDef[I].DrawLine;

                if (LineMode != 0)
                {
                    X0 = Gr->GraphDef[I].X0 - OffsetX;
                    Y0 = Gr->GraphDef[I].Y0 - OffsetY;
                    Z0 = Gr->GraphDef[I].Z0 - OffsetZ;
                    DrawX0 = round((X0 * DrawXtoX) + (Y0 * DrawYtoX) + (Z0 * DrawZtoX));
                    DrawY0 = round((X0 * DrawXtoY) + (Y0 * DrawYtoY) + (Z0 * DrawZtoY));
                    DrawZ0 = round((X0 * DrawXtoZ) + (Y0 * DrawYtoZ) + (Z0 * DrawZtoZ));

                    if (PlotSizeL >= 0)
                    {
                        if (LineMode == 1)
                        {
                            DrawLine(DrawX0, DrawY0, DrawZ0, DrawX, DrawY, DrawZ, Gr->GraphDef[I].ColorR, Gr->GraphDef[I].ColorG, Gr->GraphDef[I].ColorB);
                        }
                        else
                        {
                            DrawLine0(DrawX0, DrawY0, DrawZ0, DrawX, DrawY, DrawZ, Gr->GraphDef[I].ColorR0, Gr->GraphDef[I].ColorG0, Gr->GraphDef[I].ColorB0, Gr->GraphDef[I].ColorR, Gr->GraphDef[I].ColorG, Gr->GraphDef[I].ColorB);
                        }
                    }
                    if (PlotSizeP >= 0)
                    {
                        DrawPoint(DrawX0, DrawY0, DrawZ0, Gr->GraphDef[I].ColorR0, Gr->GraphDef[I].ColorG0, Gr->GraphDef[I].ColorB0);
                    }
                }
                if (PlotSizeP >= 0)
                {
                    DrawPoint(DrawX, DrawY, DrawZ, Gr->GraphDef[I].ColorR, Gr->GraphDef[I].ColorG, Gr->GraphDef[I].ColorB);
                }
            }
            RefreshNum = L;
            ui->GraphScreen->ImgX = Img;
            ui->GraphScreen->repaint();
            if (ImgX != NULL)
            {
                DEL(QImage, ImgX);
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
    RefreshNum = -1;
    Refresh();
}

void IOGraphWin::on_GraphPlotSizeP_valueChanged(int arg1) { arg1++; arg1--; RefreshParams(); }
void IOGraphWin::on_GraphPlotSizeL_valueChanged(int arg1) { arg1++; arg1--; RefreshParams(); }
void IOGraphWin::on_GrScaleXX_textChanged(const QString &arg1) { RefreshParams(); }
void IOGraphWin::on_GrScaleXY_textChanged(const QString &arg1) { RefreshParams(); }
void IOGraphWin::on_GrScaleXZ_textChanged(const QString &arg1) { RefreshParams(); }
void IOGraphWin::on_GrScaleYX_textChanged(const QString &arg1) { RefreshParams(); }
void IOGraphWin::on_GrScaleYY_textChanged(const QString &arg1) { RefreshParams(); }
void IOGraphWin::on_GrScaleYZ_textChanged(const QString &arg1) { RefreshParams(); }
void IOGraphWin::on_GrScaleZX_textChanged(const QString &arg1) { RefreshParams(); }
void IOGraphWin::on_GrScaleZY_textChanged(const QString &arg1) { RefreshParams(); }
void IOGraphWin::on_GrScaleZZ_textChanged(const QString &arg1) { RefreshParams(); }
void IOGraphWin::on_GrScaleX_textChanged(const QString &arg1) { RefreshParams(); }
void IOGraphWin::on_GrScaleY_textChanged(const QString &arg1) { RefreshParams(); }
void IOGraphWin::on_GrScaleZ_textChanged(const QString &arg1) { RefreshParams(); }
void IOGraphWin::on_GrOffset1X_textChanged(const QString &arg1) { RefreshParams(); }
void IOGraphWin::on_GrOffset1Y_textChanged(const QString &arg1) { RefreshParams(); }
void IOGraphWin::on_GrOffset1Z_textChanged(const QString &arg1) { RefreshParams(); }
void IOGraphWin::on_GrOffset2X_textChanged(const QString &arg1) { RefreshParams(); }
void IOGraphWin::on_GrOffset2Y_textChanged(const QString &arg1) { RefreshParams(); }
void IOGraphWin::on_GrOffset2Z_textChanged(const QString &arg1) { RefreshParams(); }


void IOGraphWin::RefreshParams()
{
    PlotSizeP = ui->GraphPlotSizeP->value() - 1;
    PlotSizeP2 = PlotSizeP & 1;
    PlotSizeP = PlotSizeP >> 1;
    PlotSizeL = ui->GraphPlotSizeL->value() - 1;
    PlotSizeL2 = PlotSizeL & 1;
    PlotSizeL = PlotSizeL >> 1;
    if (PlotSizeP >= 0)
    {
        for (int YY = 0; YY <= MaxPlotWidth; YY++)
        {
            for (int XX = 0; XX <= MaxPlotWidth; XX++)
            {
                Mask[XX][YY] = MaskAll[PlotSizeP][XX][YY];
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

    DrawXtoX = (Eden::ToLong(ui->GrScaleXX->text()) * Eden::ToLong(ui->GrScaleX->text()));
    DrawXtoX /= 1000000.0;
    DrawXtoY = (Eden::ToLong(ui->GrScaleXY->text()) * Eden::ToLong(ui->GrScaleX->text()));
    DrawXtoY /= 1000000.0;
    DrawXtoZ = (Eden::ToLong(ui->GrScaleXZ->text()) * Eden::ToLong(ui->GrScaleX->text()));
    DrawXtoZ /= 1000000.0;
    DrawYtoX = (Eden::ToLong(ui->GrScaleYX->text()) * Eden::ToLong(ui->GrScaleY->text()));
    DrawYtoX /= 1000000.0;
    DrawYtoY = (Eden::ToLong(ui->GrScaleYY->text()) * Eden::ToLong(ui->GrScaleY->text()));
    DrawYtoY /= 1000000.0;
    DrawYtoZ = (Eden::ToLong(ui->GrScaleYZ->text()) * Eden::ToLong(ui->GrScaleY->text()));
    DrawYtoZ /= 1000000.0;
    DrawZtoX = (Eden::ToLong(ui->GrScaleZX->text()) * Eden::ToLong(ui->GrScaleZ->text()));
    DrawZtoX /= 1000000.0;
    DrawZtoY = (Eden::ToLong(ui->GrScaleZY->text()) * Eden::ToLong(ui->GrScaleZ->text()));
    DrawZtoY /= 1000000.0;
    DrawZtoZ = (Eden::ToLong(ui->GrScaleZZ->text()) * Eden::ToLong(ui->GrScaleZ->text()));
    DrawZtoZ /= 1000000.0;

    OffsetX = Eden::ToLong(ui->GrOffset1X->text());
    OffsetX /= 1000000.0;
    OffsetY = Eden::ToLong(ui->GrOffset1Y->text());
    OffsetY /= 1000000.0;
    OffsetZ = Eden::ToLong(ui->GrOffset1Z->text());
    OffsetZ /= 1000000.0;

    OffsetXRender = Eden::ToLong(ui->GrOffset2X->text());
    OffsetYRender = Eden::ToLong(ui->GrOffset2Y->text());
    OffsetZRender = Eden::ToLong(ui->GrOffset2Z->text());

    RefreshNum = -1;
}
