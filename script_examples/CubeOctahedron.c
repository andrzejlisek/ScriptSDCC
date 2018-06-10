#include "core.h"
#include "graph.h"

void main()
{
 int H = 100;
 int HX = (H * 141) / 100;

 graph_clear(0);
 graph_thread(0, 1);

 graph_thread_reset(0);
 graph_plot_int(0, 0 - H, 0 - H, 0 - H,   0,   0,   0);
 graph_plot_int(0, 0 + H, 0 - H, 0 - H, 255,   0,   0);
 graph_plot_int(0, 0 + H, 0 + H, 0 - H, 255, 255,   0);
 graph_plot_int(0, 0 - H, 0 + H, 0 - H,   0, 255,   0);
 graph_plot_int(0, 0 - H, 0 - H, 0 - H,   0,   0,   0);

 graph_plot_int(0, 0 - H, 0 - H, 0 + H,   0,   0, 255);
 graph_plot_int(0, 0 + H, 0 - H, 0 + H, 255,   0, 255);
 graph_plot_int(0, 0 + H, 0 + H, 0 + H, 255, 255, 255);
 graph_plot_int(0, 0 - H, 0 + H, 0 + H,   0, 255, 255);
 graph_plot_int(0, 0 - H, 0 - H, 0 + H,   0,   0, 255);

 graph_thread_reset(0);
 graph_plot_int(0, 0 + H, 0 - H, 0 - H, 255,   0,   0);
 graph_plot_int(0, 0 + H, 0 - H, 0 + H, 255,   0, 255);

 graph_thread_reset(0);
 graph_plot_int(0, 0 + H, 0 + H, 0 - H, 255, 255,   0);
 graph_plot_int(0, 0 + H, 0 + H, 0 + H, 255, 255, 255);

 graph_thread_reset(0);
 graph_plot_int(0, 0 - H, 0 + H, 0 - H,   0, 255,   0);
 graph_plot_int(0, 0 - H, 0 + H, 0 + H,   0, 255, 255);


 graph_clear(1);
 graph_thread(1, 1);
 graph_thread_reset(1);
 graph_plot_int(1, 0 - H, 0 - H, 0,      255,   0,   0);
 graph_plot_int(1, 0 + H, 0 - H, 0,        0, 255,   0);
 graph_plot_int(1, 0 + H, 0 + H, 0,      255,   0,   0);
 graph_plot_int(1, 0 - H, 0 + H, 0,        0, 255,   0);
 graph_plot_int(1, 0,     0,     0 + HX,   0,   0, 255);
 graph_plot_int(1, 0 + H, 0 + H, 0,      255,   0,   0);
 graph_plot_int(1, 0,     0,     0 - HX,   0,   0, 255);
 graph_plot_int(1, 0 - H, 0 + H, 0,        0, 255,   0);
 graph_plot_int(1, 0 - H, 0 - H, 0,      255,   0,   0);
 graph_plot_int(1, 0,     0,     0 + HX,   0,   0, 255);
 graph_plot_int(1, 0 + H, 0 - H, 0,        0, 255,   0);
 graph_plot_int(1, 0,     0,     0 - HX,   0,   0, 255);
 graph_plot_int(1, 0 - H, 0 - H, 0,      255,   0,   0);

 end();
}
