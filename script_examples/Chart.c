#include "core.h"
#include "spreadsheet.h"
#include "graph.h"
#include "math.h"
#include "mathx.h"
#include "stdlib.h"

void FillCells(uchar N, uchar R, uchar C, char * Text)
{
 uchar L = 0;
 uchar P;
 char * Temp;
 while (Text[L] != 0)
 {
  L++;
  if (Text[L] == Text[0])
  {
   C++;
  }
 }
 Temp = malloc(L + 1);
 P = L;
 Temp[L] = 0;
 while (P > 0)
 {
  P--;
  Temp[P] = Text[P];
 }
 P = L - 1;
 while (P > 0)
 {
  P--;
  if (Temp[P] == Temp[0])
  {
   cell_set_string(N, R, C, Temp + P + 1);
   Temp[P] = '\0';
   C--;
  }
 }
 free(Temp);
}

void main()
{
 long Xmin = -200;
 long Xmax = 200;
 long Ymin = -200;
 long Ymax = 200;
 long X_, Y_, Xstep, Ystep;
 int X__, Y__, X___;
 float X1, Y1, X2, Y2, X3, Y3;
 float XXX_1, XXY_1, XYY_1, YYY_1, XX_1, XY_1, YY_1, X_1, Y_1, C_1;
 float XXX_2, XXY_2, XYY_2, YYY_2, XX_2, XY_2, YY_2, X_2, Y_2, C_2;
 float Val, Val_1, Val_2;
 float ToleranceP, ToleranceN;

 uchar Color1R;
 uchar Color1G;
 uchar Color1B;

 uchar Color0R;
 uchar Color0G;
 uchar Color0B;

 uchar ColorXR;
 uchar ColorXG;
 uchar ColorXB;

 Xstep = 2;
 Ystep = 2;

 sheet_clear_whole(1);

 FillCells(1,  0, 0, "|Line|XXX|XXY|XYY|YYY|XX|XY|YY|X|Y|Const");
 FillCells(1,  1, 0, "|Numerator|0|0|0|0|0|0|0|1|-1|0");
 FillCells(1,  2, 0, "|Denominator|0|0|0|0|0|0|0|0|0|1");
 FillCells(1,  3, 0, "|Tolerance|2");

 FillCells(1,  5, 0, "|Parabola|XXX|XXY|XYY|YYY|XX|XY|YY|X|Y|Const");
 FillCells(1,  6, 0, "|Numerator|0|0|0|0|1|0|0|0|-10|0");
 FillCells(1,  7, 0, "|Denominator|0|0|0|0|0|0|0|0|0|1");
 FillCells(1,  8, 0, "|Tolerance|20");

 FillCells(1, 10, 0, "|Hyperbola|XXX|XXY|XYY|YYY|XX|XY|YY|X|Y|Const");
 FillCells(1, 11, 0, "|Numerator|0|0|0|0|0|-1|0|0|0|1000");
 FillCells(1, 12, 0, "|Denominator|0|0|0|0|0|0|0|1|1|0");
 FillCells(1, 13, 0, "|Tolerance|2");

 FillCells(1, 15, 0, "|Circle|XXX|XXY|XYY|YYY|XX|XY|YY|X|Y|Const");
 FillCells(1, 16, 0, "|Numerator|0|0|0|0|1|0|1|0|0|-10000");
 FillCells(1, 17, 0, "|Denominator|0|0|0|0|0|0|0|0|0|1");
 FillCells(1, 18, 0, "|Tolerance|400");

 sheet_clear_whole(0);
 cell_set_string(0, 1, 0, "Numerator");
 cell_set_string(0, 2, 0, "Denominator");

 cell_set_string(0, 0, 1, "XXX");
 cell_set_string(0, 0, 2, "XXY");
 cell_set_string(0, 0, 3, "XYY");
 cell_set_string(0, 0, 4, "YYY");
 cell_set_string(0, 0, 5, "XX");
 cell_set_string(0, 0, 6, "XY");
 cell_set_string(0, 0, 7, "YY");
 cell_set_string(0, 0, 8, "X");
 cell_set_string(0, 0, 9, "Y");
 cell_set_string(0, 0, 10, "Const");

 cell_set_string(0, 1, 1, "0");
 cell_set_string(0, 1, 2, "0");
 cell_set_string(0, 1, 3, "0");
 cell_set_string(0, 1, 4, "0");
 cell_set_string(0, 1, 5, "0");
 cell_set_string(0, 1, 6, "-1");
 cell_set_string(0, 1, 7, "0");
 cell_set_string(0, 1, 8, "0");
 cell_set_string(0, 1, 9, "0");
 cell_set_string(0, 1, 10, "1000");

 cell_set_string(0, 2, 1, "0");
 cell_set_string(0, 2, 2, "0");
 cell_set_string(0, 2, 3, "0");
 cell_set_string(0, 2, 4, "0");
 cell_set_string(0, 2, 5, "0");
 cell_set_string(0, 2, 6, "0");
 cell_set_string(0, 2, 7, "0");
 cell_set_string(0, 2, 8, "1");
 cell_set_string(0, 2, 9, "1");
 cell_set_string(0, 2, 10, "0");

 cell_set_string(0, 4, 0, "X range:");
 cell_set_string(0, 4, 1, "-200");
 cell_set_string(0, 4, 2, "200");

 cell_set_string(0, 4, 3, "Y range:");
 cell_set_string(0, 4, 4, "-200");
 cell_set_string(0, 4, 5, "200");

 cell_set_string(0, 5, 4, "Tolerance");
 cell_set_string(0, 5, 5, "2");

 cell_set_string(0, 5, 0, "X step:");
 cell_set_string(0, 5, 1, "2");
 cell_set_string(0, 5, 2, "Y step:");
 cell_set_string(0, 5, 3, "2");


 cell_set_string(0, 7, 0, "True");
 cell_set_string(0, 8, 0, "False");
 cell_set_string(0, 9, 0, "Div 0");

 cell_set_uchar_v(0, 7, 1, 255);
 cell_set_uchar_v(0, 7, 2, 255);
 cell_set_uchar_v(0, 7, 3, 255);

 cell_set_uchar_v(0, 8, 1, 64);
 cell_set_uchar_v(0, 8, 2, 64);
 cell_set_uchar_v(0, 8, 3, 64);

 cell_set_uchar_v(0, 9, 1, 0);
 cell_set_uchar_v(0, 9, 2, 0);
 cell_set_uchar_v(0, 9, 3, 0);


 while(1)
 {
  stop();

  Xmin = cell_get_long(0, 4, 1);
  Xmax = cell_get_long(0, 4, 2);
  Ymin = cell_get_long(0, 4, 4);
  Ymax = cell_get_long(0, 4, 5);
  Xstep = cell_get_long(0, 5, 1);
  Ystep = cell_get_long(0, 5, 3);

  Color1R = cell_get_uchar_v(0, 7, 1);
  Color1G = cell_get_uchar_v(0, 7, 2);
  Color1B = cell_get_uchar_v(0, 7, 3);

  Color0R = cell_get_uchar_v(0, 8, 1);
  Color0G = cell_get_uchar_v(0, 8, 2);
  Color0B = cell_get_uchar_v(0, 8, 3);

  ColorXR = cell_get_uchar_v(0, 9, 1);
  ColorXG = cell_get_uchar_v(0, 9, 2);
  ColorXB = cell_get_uchar_v(0, 9, 3);

  ToleranceP = cell_get_float(0, 5, 5);
  ToleranceN = 0 - ToleranceP;

  XXX_1 = cell_get_float(0, 1, 1);
  XXY_1 = cell_get_float(0, 1, 2);
  XYY_1 = cell_get_float(0, 1, 3);
  YYY_1 = cell_get_float(0, 1, 4);
  XX_1  = cell_get_float(0, 1, 5);
  XY_1  = cell_get_float(0, 1, 6);
  YY_1  = cell_get_float(0, 1, 7);
  X_1   = cell_get_float(0, 1, 8);
  Y_1   = cell_get_float(0, 1, 9);
  C_1   = cell_get_float(0, 1, 10);

  XXX_2 = cell_get_float(0, 2, 1);
  XXY_2 = cell_get_float(0, 2, 2);
  XYY_2 = cell_get_float(0, 2, 3);
  YYY_2 = cell_get_float(0, 2, 4);
  XX_2  = cell_get_float(0, 2, 5);
  XY_2  = cell_get_float(0, 2, 6);
  YY_2  = cell_get_float(0, 2, 7);
  X_2   = cell_get_float(0, 2, 8);
  Y_2   = cell_get_float(0, 2, 9);
  C_2   = cell_get_float(0, 2, 10);

  graph_clear(0);
  Y__ = 0 - ((Ymax - Ymin) / (Ystep << 1));
  X___= 0 - ((Xmax - Xmin) / (Xstep << 1));
  for (Y_ = Ymin; Y_ <= Ymax; Y_ += Ystep)
  {
   Y1 = Y_;
   Y2 = Y1 * Y1;
   Y3 = Y1 * Y1 * Y1;
   X__ = X___;
   for (X_ = Xmin; X_ <= Xmax; X_ += Xstep)
   {
    X1 = X_;
    X2 = X1 * X1;
    X3 = X1 * X1 * X1;
    Val_2 = XXX_2*X3 + XXY_2*X2*Y1 + XYY_2*X1*Y2 + YYY_2*Y3 + XX_2*X2 + XY_2*X1*Y1 + YY_2*Y2 + X_2*X1 + Y_2*Y1 + C_2;
    if (Val_2 != 0.0)
    {
     Val_1 = XXX_1*X3 + XXY_1*X2*Y1 + XYY_1*X1*Y2 + YYY_1*Y3 + XX_1*X2 + XY_1*X1*Y1 + YY_1*Y2 + X_1*X1 + Y_1*Y1 + C_1;
     Val = Val_1 / Val_2;
     if ((Val >= ToleranceN) && (Val <= ToleranceP))
     {
      graph_plot_int(0, X__, Y__, 0, Color1R, Color1G, Color1B);
     }
     else
     {
      graph_plot_int(0, X__, Y__, 0, Color0R, Color0G, Color0B);
     }
    }
    else
    {
     graph_plot_int(0, X__, Y__, 0, ColorXR, ColorXG, ColorXB);
    }
    X__++;
   }
   Y__++;
  }
 }
}

