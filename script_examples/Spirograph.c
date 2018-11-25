#include "core.h"
#include <math.h>
#include <stdlib.h>
#include "spreadsheet.h"
#include "graph.h"
#include "mathx.h"

void main()
{
 uint I, DeltaPeriod;
 float PI0;
 float R;
 float r;
 float t;
 float h;
 float Factor;
 float X;
 float Y;

 int Delta = 200;
 int Period = 28;
 int Size = 100;
 float DeltaF, SizeF;
 R = 150;
 r = 56;
 h = r * 1.0;

 sheet_clear_whole(0);

 cell_set_string(0, 0, 0, "Delta");
 cell_set_string(0, 1, 0, "Period");
 cell_set_string(0, 2, 0, "Radius 1");
 cell_set_string(0, 3, 0, "Radius 2");
 cell_set_string(0, 4, 0, "Size");
 cell_set_int(0, 0, 1, Delta);
 cell_set_int(0, 1, 1, Period);
 cell_set_int(0, 2, 1, R);
 cell_set_int(0, 3, 1, r);
 cell_set_int(0, 4, 1, Size);

 while (1)
 {
  graph_clear(0);

  Delta = cell_get_int(0, 0, 1);
  Period = cell_get_int(0, 1, 1);
  R = cell_get_int(0, 2, 1);
  r = cell_get_int(0, 3, 1);
  Size = cell_get_int(0, 4, 1);

  graph_thread(0, 1);

  DeltaF = Delta;
  SizeF = Size;
  Factor = SizeF / R;

  PI0 = M_PI_ / (DeltaF / 2.0);
  DeltaPeriod = Delta * Period;
  for (I = 0; I < DeltaPeriod; I++)
  {
   t = I;
   t = t * PI0;
   X = (R - r) * cosf(t) + h * cosf(((R - r) * t) / r);
   Y = (R - r) * sinf(t) + h * sinf(((R - r) * t) / r);
   graph_plot_float(0, X * Factor, Y * Factor, 0, 255, 255, 255);
  }
  stop();
 }
}
