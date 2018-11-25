#include "graphfont.h"

GraphFont::GraphFont(string FileName)
{
    FontExists = false;
    QImage * Img = NEW(QImage, QImage(Eden::ToQStr(FileName)));
    int ImgW = Img->size().width();
    int ImgH = Img->size().height();
    if ((ImgW > 0) && (ImgH > 0) && ((ImgW % 256) == 0))
    {
        FontW = ImgW / 256;
        FontH = ImgH;

        for (int I = 0; I < 256; I++)
        {
            FontGlyph[I] = NEWARR(uchar, uchar[FontW * FontH]);
            int P = 0;
            for (int Y = 0; Y < FontH; Y++)
            {
                for (int X = 0; X < FontW; X++)
                {
                    int II = I * FontW;
                    int R = Img->pixelColor(II + X, Y).red();
                    int G = Img->pixelColor(II + X, Y).green();
                    int B = Img->pixelColor(II + X, Y).blue();
                    int Pix = R + G + B;
                    if (Pix >= 384)
                    {
                        FontGlyph[I][P] = 1;
                    }
                    else
                    {
                        FontGlyph[I][P] = 0;
                    }
                    P++;
                }
            }
        }
        FontExists = true;
    }
    DEL(QImage, Img);
}

GraphFont::~GraphFont()
{
    for (int I = 0; I < 256; I++)
    {
        DELARR(uchar, FontGlyph[I]);
    }
}
