#ifndef IOGRAPH_H
#define IOGRAPH_H

#include <iostream>
#include "eden.h"
#include <queue>
#include "graphfont.h"
#include <mutex>

using namespace std;

#define DimensionType double

class IOGraph
{
public:
    struct PlotDef
    {
        DimensionType X = 0;
        DimensionType Y = 0;
        DimensionType Z = 0;
        uchar ColorR = 0;
        uchar ColorG = 0;
        uchar ColorB = 0;

        DimensionType X0 = 0;
        DimensionType Y0 = 0;
        DimensionType Z0 = 0;
        uchar ColorR0 = 0;
        uchar ColorG0 = 0;
        uchar ColorB0 = 0;

        DimensionType X00 = 0;
        DimensionType Y00 = 0;
        DimensionType Z00 = 0;
        uchar ColorR00 = 0;
        uchar ColorG00 = 0;
        uchar ColorB00 = 0;

        // 0 - Point
        // 1 - Solid color line
        // 2 - Gradient line
        // 3 - Solid color triangle
        // 4 - Gradient triangle
        char DrawMode = 0;

        bool IsText = false;
        string Text = "";
        int TextLen = 0;
        int TextX = 0;
        int TextY = 0;

        bool TextF = true;
        bool TextB = false;

        bool Draw = true;
    };

    uchar TextColorR = 255;
    uchar TextColorG = 255;
    uchar TextColorB = 255;

    uchar TextColorR0 = 0;
    uchar TextColorG0 = 0;
    uchar TextColorB0 = 0;

    bool TextFore = true;
    bool TextBack = false;

    vector<PlotDef> GraphDef;

    GraphFont * GraphFont_;
    IOGraph(GraphFont * GraphFont__);
    ~IOGraph();
    bool NeedRedraw = false;
    int NeedRedrawLast = -1;
    void Clear();
    void Plot(DimensionType X, DimensionType Y, DimensionType Z, uchar R, uchar G, uchar B);
    void Line(int L);
    void Triangle();
    bool IsChanged();
    void ThreadSet(uchar Thr);
    void ThreadReset();
    void TextSet(double X, double Y, double Z, int X0, int Y0);
    void TextPrint(string Text);
private:
    mutex MTX;
    bool Changed = false;
    uchar CurrentThread = 0;
    DimensionType ThreadX[256];
    DimensionType ThreadY[256];
    DimensionType ThreadZ[256];
    uchar ThreadR[256];
    uchar ThreadG[256];
    uchar ThreadB[256];
    uchar ThreadS[256];

    double Text_X = 0;
    double Text_Y = 0;
    double Text_Z = 0;
    int Text_X0 = 0;
    int Text_Y0 = 0;
};

#endif // IOGRAPH_H
