#ifndef GRAPHFONT_H
#define GRAPHFONT_H

#include <iostream>
#include <QImage>
#include "eden.h"

class GraphFont
{
public:
    GraphFont(string FileName);
    ~GraphFont();
    bool FontExists;
    int FontW = 0;
    int FontH = 0;
    uchar * FontGlyph[65536];
    int FontGlyphC = 0;
};

#endif // GRAPHFONT_H
