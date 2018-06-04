#ifndef IOGRAPH_H
#define IOGRAPH_H

#include <iostream>
#include "eden.h"
#include <queue>

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

        char DrawLine = 0;
    };

    vector<PlotDef> GraphDef;

    IOGraph();
    ~IOGraph();
    bool NeedRedraw = false;
    void Clear();
    void Plot(DimensionType X, DimensionType Y, DimensionType Z, uchar R, uchar G, uchar B);
    bool IsChanged();
    void ThreadSet(uchar Thr);
    void ThreadReset();
private:
    bool Changed = false;
    uchar CurrentThread = 0;
    DimensionType ThreadX[256];
    DimensionType ThreadY[256];
    DimensionType ThreadZ[256];
    uchar ThreadR[256];
    uchar ThreadG[256];
    uchar ThreadB[256];
    uchar ThreadS[256];
};

#endif // IOGRAPH_H
