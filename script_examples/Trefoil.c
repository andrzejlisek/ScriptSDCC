#include "core.h"
#include "graph.h"
#include "mathx.h"
#include <math.h>

void main()
{
 int I;
 float Radius = 50.0;
 float T1, T2, T3, X, Y, Z;

 graph_clear(0);
 graph_clear(1);
 graph_thread(0, 1);
 graph_thread(1, 1);

 for (I = 0; I <= 360; I++)
 {
  T3 = I;
  T1 = (T3 * M_PI_) / 180.0;
  T2 = (T3 * M_PI_) / 90.0;
  T3 = (T3 * M_PI_) / 60.0;

  X = sinf(T1) + 2.0 * sinf(T2);
  Y = cosf(T1) - 2.0 * cosf(T2);
  Z = 0.0 - sinf(T3);
  graph_plot_float(0, X * Radius, Y * Radius, Z * Radius, 255, 255, 255);

  X = (2.0 + cosf(T3)) * cosf(T2);
  Y = (2.0 + cosf(T3)) * sinf(T2);
  Z = sinf(T3);
  graph_plot_float(1, X * Radius, Y * Radius, Z * Radius, 255, 255, 255);

 } 
 end();
}
