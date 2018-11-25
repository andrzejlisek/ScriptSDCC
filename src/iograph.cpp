#include "iograph.h"

IOGraph::IOGraph(GraphFont * GraphFont__)
{
    GraphFont_ = GraphFont__;
    Clear();
}

IOGraph::~IOGraph()
{

}

void IOGraph::Clear()
{
    GraphDef.clear();

    TextColorR = 255;
    TextColorG = 255;
    TextColorB = 255;

    TextColorR0 = 0;
    TextColorG0 = 0;
    TextColorB0 = 0;

    TextFore = true;
    TextBack = false;

    Text_X = 0;
    Text_Y = 0;
    Text_Z = 0;
    Text_X0 = 0;
    Text_Y0 = 0;

    for (int I = 0; I < 256; I++)
    {
        ThreadS[I] = 0;
    }
    CurrentThread = 0;
    Changed = true;
    NeedRedraw = true;
}

void IOGraph::Plot(double X, double Y, double Z, uchar R, uchar G, uchar B)
{
    PlotDef PD;
    PD.X = X;
    PD.Y = Y;
    PD.Z = Z;
    PD.ColorR = R;
    PD.ColorG = G;
    PD.ColorB = B;
    if (CurrentThread == 0)
    {
        PD.DrawMode = 0;
        GraphDef.push_back(PD);
    }
    else
    {
        if (ThreadS[CurrentThread] == 0)
        {
            PD.DrawMode = 0;
            GraphDef.push_back(PD);
            ThreadS[CurrentThread] = 1;
        }
        else
        {
            PD.X0 = ThreadX[CurrentThread];
            PD.Y0 = ThreadY[CurrentThread];
            PD.Z0 = ThreadZ[CurrentThread];
            PD.ColorR0 = ThreadR[CurrentThread];
            PD.ColorG0 = ThreadG[CurrentThread];
            PD.ColorB0 = ThreadB[CurrentThread];
            PD.DrawMode = ((PD.ColorR0 == R) && (PD.ColorG0 == G) && (PD.ColorB0 == B)) ? 1 : 2;
            GraphDef.push_back(PD);
        }
        ThreadX[CurrentThread] = X;
        ThreadY[CurrentThread] = Y;
        ThreadZ[CurrentThread] = Z;
        ThreadR[CurrentThread] = R;
        ThreadG[CurrentThread] = G;
        ThreadB[CurrentThread] = B;
    }
    Changed = true;
}

void IOGraph::Line(int L)
{
    int I = GraphDef.size();
    int I0 = I - 1;
    if (I <= L)
    {
        return;
    }
    while (L > 0)
    {
        I--;
        I0--;
        GraphDef[I].X0 = GraphDef[I0].X;
        GraphDef[I].Y0 = GraphDef[I0].Y;
        GraphDef[I].Z0 = GraphDef[I0].Z;
        GraphDef[I].ColorR0 = GraphDef[I0].ColorR;
        GraphDef[I].ColorG0 = GraphDef[I0].ColorG;
        GraphDef[I].ColorB0 = GraphDef[I0].ColorB;
        if ((GraphDef[I].ColorR != GraphDef[I0].ColorR) || (GraphDef[I].ColorG != GraphDef[I0].ColorG) || (GraphDef[I].ColorB != GraphDef[I0].ColorB))
        {
            GraphDef[I].DrawMode = 2;
        }
        else
        {
            GraphDef[I].DrawMode = 1;
        }
        L--;
    }
    if (NeedRedrawLast < 0)
    {
        NeedRedrawLast = I0;
    }
}

void IOGraph::Triangle()
{
    int I = GraphDef.size();
    if (I < 3)
    {
        return;
    }
    I--;
    int I0 = I - 1;
    if (GraphDef[I0].DrawMode < 3)
    {
        GraphDef[I0].Draw = false;
    }
    GraphDef[I].X0 = GraphDef[I0].X;
    GraphDef[I].Y0 = GraphDef[I0].Y;
    GraphDef[I].Z0 = GraphDef[I0].Z;
    GraphDef[I].ColorR0 = GraphDef[I0].ColorR;
    GraphDef[I].ColorG0 = GraphDef[I0].ColorG;
    GraphDef[I].ColorB0 = GraphDef[I0].ColorB;
    I0--;
    if (GraphDef[I0].DrawMode < 3)
    {
        GraphDef[I0].Draw = false;
    }
    GraphDef[I].X00 = GraphDef[I0].X;
    GraphDef[I].Y00 = GraphDef[I0].Y;
    GraphDef[I].Z00 = GraphDef[I0].Z;
    GraphDef[I].ColorR00 = GraphDef[I0].ColorR;
    GraphDef[I].ColorG00 = GraphDef[I0].ColorG;
    GraphDef[I].ColorB00 = GraphDef[I0].ColorB;
    GraphDef[I].DrawMode = 3;
    PlotDef PD = GraphDef[I];
    if ((PD.ColorR != PD.ColorR0) || (PD.ColorR0 != PD.ColorR00) || (PD.ColorR00 != PD.ColorR))
    {
        GraphDef[I].DrawMode = 4;
    }
    else
    {
        if ((PD.ColorG != PD.ColorG0) || (PD.ColorG0 != PD.ColorG00) || (PD.ColorG00 != PD.ColorG))
        {
            GraphDef[I].DrawMode = 4;
        }
        else
        {
            if ((PD.ColorB != PD.ColorB0) || (PD.ColorB0 != PD.ColorB00) || (PD.ColorB00 != PD.ColorB))
            {
                GraphDef[I].DrawMode = 4;
            }
        }
    }
    if (NeedRedrawLast < 0)
    {
        NeedRedrawLast = I0;
    }
}

bool IOGraph::IsChanged()
{
    return Changed;
}

void IOGraph::ThreadSet(uchar Thr)
{
    CurrentThread = Thr;
}

void IOGraph::ThreadReset()
{
    ThreadS[CurrentThread] = 0;
}

void IOGraph::TextSet(double X, double Y, double Z, int X0, int Y0)
{
    Text_X = X;
    Text_Y = Y;
    Text_Z = Z;
    Text_X0 = X0;
    Text_Y0 = Y0;
}

void IOGraph::TextPrint(string Text)
{
    PlotDef PD;
    PD.X = Text_X;
    PD.Y = Text_Y;
    PD.Z = Text_Z;
    PD.ColorR = TextColorR;
    PD.ColorG = TextColorG;
    PD.ColorB = TextColorB;
    PD.ColorR0 = TextColorR0;
    PD.ColorG0 = TextColorG0;
    PD.ColorB0 = TextColorB0;
    PD.TextB = TextBack;
    PD.TextF = TextFore;
    PD.DrawMode = 0;
    PD.IsText = true;
    PD.Text = Text;
    PD.TextLen = PD.Text.size();
    PD.TextX = Text_X0 - (PD.TextLen * GraphFont_->FontW / 2);
    PD.TextY = 0 - Text_Y0 - (GraphFont_->FontH / 2);
    GraphDef.push_back(PD);
}
