#include "iograph.h"

IOGraph::IOGraph()
{
    Clear();
}

IOGraph::~IOGraph()
{

}

void IOGraph::Clear()
{
    GraphDef.clear();
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
        PD.DrawLine = 0;
        GraphDef.push_back(PD);
    }
    else
    {
        if (ThreadS[CurrentThread] == 0)
        {
            PD.DrawLine = 0;
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
            PD.DrawLine = ((PD.ColorR0 == R) && (PD.ColorG0 == G) && (PD.ColorB0 == B)) ? 1 : 2;
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
