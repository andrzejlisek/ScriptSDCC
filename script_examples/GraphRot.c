#include "core.h"
#include "spreadsheet.h"
#include "graph.h"
#include "graphtext.h"
#include "mathx.h"
#include <math.h>

void TextToArray(uchar * A, char * S)
{
 uchar I = 0;
 while (S[I] != 0)
 {
  A[I] = S[I];
  I++;
 }
 A[I] = 0;
}

void main()
{
 float Angle;
 float Radius = 100;
 float Distance = 150;


 float ScaleX1;
 float ScaleY1;
 float ScaleX2;
 float ScaleY2;

 uchar ColorR_ = 128;
 uchar ColorG_ = 128;
 uchar ColorB_ = 128;
 uchar ColorR[25];
 uchar ColorG[25];
 uchar ColorB[25];

 int I;

 uchar Label[6][10];

 TextToArray(Label[0], "RED");
 TextToArray(Label[1], "YELLOW");
 TextToArray(Label[2], "GREEN");
 TextToArray(Label[3], "CYAN");
 TextToArray(Label[4], "BLUE");
 TextToArray(Label[5], "MAGENTA");

 ColorR[0]  = 255; ColorG[0]  =   0; ColorB[0]  =   0;
 ColorR[1]  = 255; ColorG[1]  =  64; ColorB[1]  =   0;
 ColorR[2]  = 255; ColorG[2]  = 128; ColorB[2]  =   0;
 ColorR[3]  = 255; ColorG[3]  = 192; ColorB[3]  =   0;
 ColorR[4]  = 255; ColorG[4]  = 255; ColorB[4]  =   0;
 ColorR[5]  = 192; ColorG[5]  = 255; ColorB[5]  =   0;
 ColorR[6]  = 128; ColorG[6]  = 255; ColorB[6]  =   0;
 ColorR[7]  =  64; ColorG[7]  = 255; ColorB[7]  =   0;
 ColorR[8]  =   0; ColorG[8]  = 255; ColorB[8]  =   0;
 ColorR[9]  =   0; ColorG[9]  = 255; ColorB[9]  =  64;
 ColorR[10] =   0; ColorG[10] = 255; ColorB[10] = 128;
 ColorR[11] =   0; ColorG[11] = 255; ColorB[11] = 192;
 ColorR[12] =   0; ColorG[12] = 255; ColorB[12] = 255;
 ColorR[13] =   0; ColorG[13] = 192; ColorB[13] = 255;
 ColorR[14] =   0; ColorG[14] = 128; ColorB[14] = 255;
 ColorR[15] =   0; ColorG[15] =  64; ColorB[15] = 255;
 ColorR[16] =   0; ColorG[16] =   0; ColorB[16] = 255;
 ColorR[17] =  64; ColorG[17] =   0; ColorB[17] = 255;
 ColorR[18] = 128; ColorG[18] =   0; ColorB[18] = 255;
 ColorR[19] = 192; ColorG[19] =   0; ColorB[19] = 255;
 ColorR[20] = 255; ColorG[20] =   0; ColorB[20] = 255;
 ColorR[21] = 255; ColorG[21] =   0; ColorB[21] = 192;
 ColorR[22] = 255; ColorG[22] =   0; ColorB[22] = 128;
 ColorR[23] = 255; ColorG[23] =   0; ColorB[23] =  64;
 ColorR[24] = 255; ColorG[24] =   0; ColorB[24] =   0;


 sheet_clear_whole(0);

 cell_set_string(0, 0, 0, "Radius:");
 cell_set_string(0, 1, 0, "Label distance:");
 cell_set_string(0, 2, 0, "Middle point:");

 cell_set_sint(0, 0, 1, Radius);
 cell_set_sint(0, 1, 1, Distance);
 cell_set_sint(0, 2, 1, ColorR_);
 cell_set_sint(0, 2, 2, ColorG_);
 cell_set_sint(0, 2, 3, ColorB_);

 while(1)
 {
  graph_clear(0);

  Radius = cell_get_sint(0, 0, 1);
  Distance = cell_get_sint(0, 1, 1);
  ColorR_ = cell_get_sint(0, 2, 1);
  ColorG_ = cell_get_sint(0, 2, 2);
  ColorB_ = cell_get_sint(0, 2, 3);


  for (I = 0; I < 24; I++)
  {
   Angle = I * 15.0 * angle_deg_rad;
   ScaleX1 = cosf(Angle) * Radius;
   ScaleY1 = sinf(Angle) * Radius;

   Angle = (I + 1) * 15.0 * angle_deg_rad;
   ScaleX2 = cosf(Angle) * Radius;
   ScaleY2 = sinf(Angle) * Radius;

   graph_plot_float(0, ScaleX1, ScaleY1, 0, ColorR[I], ColorG[I], ColorB[I]);
   graph_plot_float(0, ScaleX2, ScaleY2, 0, ColorR[I + 1], ColorG[I + 1], ColorB[I + 1]);
   graph_plot_float(0, 0, 0, 0, ColorR_, ColorG_, ColorB_);
   graph_triangle(0);
  }

  for (I = 0; I < 6; I++)
  {
   Angle = I * 60.0 * angle_deg_rad;
   ScaleX1 = cosf(Angle) * Distance;
   ScaleY1 = sinf(Angle) * Distance;

   graph_text_color_f(0, ColorR[I * 4], ColorG[I * 4], ColorB[I * 4]);
   graph_text_xy_float(0, ScaleX1, ScaleY1, 0, 0, 0);
   graph_text_string(0, Label[I]);
  }

  stop();
 }
}
