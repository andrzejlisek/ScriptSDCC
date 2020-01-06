#include "graphfont.h"

GraphFont::GraphFont(string FileName)
{
    FontExists = false;
    QImage * Img = NEW(QImage, QImage(Eden::ToQStr(FileName)));
    int ImgW = Img->size().width();
    int ImgH = Img->size().height();

    for (int I = 0; I < 65536; I++)
    {
        FontGlyph[I] = NULL;
    }

    if ((ImgW > 8) && (ImgH > 0) && (((ImgW - 8) % 256) == 0))
    {
        int R, G, B, Pix;
        int LastPage = -1;
        int PageCount = 0;
        vector<int> Page;

        for (int I = 0; I < ImgH; I++)
        {
            int PageNum = 0;
            int PageNumDelta = 128;
            for (int II = 0; II < 8; II++)
            {
                R = Img->pixelColor(II, I).red();
                G = Img->pixelColor(II, I).green();
                B = Img->pixelColor(II, I).blue();
                Pix = R + G + B;
                if (Pix >= 384) { PageNum += PageNumDelta; }
                PageNumDelta = PageNumDelta >> 1;
            }
            if (LastPage != PageNum)
            {
                Page.push_back(PageNum);
                LastPage = PageNum;
                PageCount++;
            }
        }

        FontGlyphC = PageCount << 8;

        if ((ImgH % PageCount) == 0)
        {
            FontW = (ImgW - 8) / 256;
            FontH = ImgH / PageCount;

            for (int YY = 0; YY < PageCount; YY++)
            {
                int I = Page[YY] << 8;
                for (int XX = 0; XX < 256; XX++)
                {
                    FontGlyph[I] = NEWARR(uchar, uchar[FontW * FontH]);
                    int P = 0;
                    for (int Y = 0; Y < FontH; Y++)
                    {
                        for (int X = 0; X < FontW; X++)
                        {
                            int XXX = XX * FontW + 8;
                            int YYY = YY * FontH;
                            R = Img->pixelColor(XXX + X, YYY + Y).red();
                            G = Img->pixelColor(XXX + X, YYY + Y).green();
                            B = Img->pixelColor(XXX + X, YYY + Y).blue();
                            Pix = R + G + B;
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
                    I++;
                }
            }
            FontExists = true;
        }
    }
    DEL(QImage, Img);
}

GraphFont::~GraphFont()
{
    for (int I = 0; I < FontGlyphC; I++)
    {
        DELARR(uchar*, FontGlyph[I]);
    }
}
