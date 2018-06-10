#include "core.h"
#include "graph.h"

void draw(float X1, float Y1, float X2, float Y2, char T)
{
 float XS1 = X1 + (X2 - X1) * 0.5;
 float YS1 = Y1 + (Y2 - Y1) * 0.5;
 float DX = (X2 - X1) * 0.1;
 float DY = (Y2 - Y1) * 0.1;
 if (T == 0)
 {
  graph_thread_reset(0);
  graph_plot_float(0, X1 + DX, Y1 + DY, 0, 255, 255, 255);
  graph_plot_float(0, X1 + DX, Y2 - DY, 0, 255, 255, 255);
  graph_plot_float(0, X2 - DX, Y2 - DY, 0, 255, 255, 255);
  graph_plot_float(0, X2 - DX, Y1 + DY, 0, 255, 255, 255);
  graph_plot_float(0, X1 + DX, Y1 + DY, 0, 255, 255, 255);
 }
 else
 {
  draw(X1, Y1, XS1, YS1, T - 1);
  draw(XS1, Y1, X2, YS1, T - 1);
  draw(X1, YS1, XS1, Y2, T - 1);
  draw(XS1, YS1, X2, Y2, T - 1);
 }
}

void main()
{
 graph_clear(0);
 graph_thread(0, 1);
 draw(-100, -100, 100, 100, 6);
 end();
}
