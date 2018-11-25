#include "core.h"
#include "graph.h"
#include <math.h>
#include "mathx.h"

void main()
{
 int H = 100;
 int I, II;
 int X11, Y11, X21, Y21, X31, Y31, X41, Y41, X51, Y51;
 int X12, Y12, X22, Y22, X32, Y32, X42, Y42, X52, Y52;
 float Ang1, Ang2, Ang3, Ang4;
 uchar C00R, C00G, C00B;
 uchar C11R, C11G, C11B;
 uchar C21R, C21G, C21B;
 uchar C31R, C31G, C31B;
 uchar C41R, C41G, C41B;
 uchar C51R, C51G, C51B;
 uchar C61R, C61G, C61B;
 uchar C12R, C12G, C12B;
 uchar C22R, C22G, C22B;
 uchar C32R, C32G, C32B;
 uchar C42R, C42G, C42B;
 uchar C52R, C52G, C52B;
 uchar C62R, C62G, C62B;
 uchar C__R, C__G, C__B;

 graph_clear(0);
 graph_clear(1);
 graph_clear(2);

 graph_plot_int(0, 0 - H, 0 - H, 0 - H,   0,   0,   0);
 graph_plot_int(0, 0 + H, 0 - H, 0 - H, 255,   0,   0);
 graph_plot_int(0, 0 - H, 0 + H, 0 - H,   0, 255,   0);
 graph_triangle(0);
 graph_plot_int(0, 0 + H, 0 + H, 0 - H, 255, 255,   0);
 graph_triangle(0);

 graph_plot_int(0, 0 - H, 0 - H, 0 - H,   0,   0,   0);
 graph_plot_int(0, 0 - H, 0 - H, 0 + H,   0,   0, 255);
 graph_plot_int(0, 0 - H, 0 + H, 0 - H,   0, 255,   0);
 graph_triangle(0);
 graph_plot_int(0, 0 - H, 0 + H, 0 + H,   0, 255, 255);
 graph_triangle(0);

 graph_plot_int(0, 0 - H, 0 - H, 0 - H,   0,   0,   0);
 graph_plot_int(0, 0 - H, 0 - H, 0 + H,   0,   0, 255);
 graph_plot_int(0, 0 + H, 0 - H, 0 - H, 255,   0,   0);
 graph_triangle(0);
 graph_plot_int(0, 0 + H, 0 - H, 0 + H, 255,   0, 255);
 graph_triangle(0);

 graph_plot_int(0, 0 + H, 0 + H, 0 + H, 255, 255, 255);
 graph_plot_int(0, 0 - H, 0 + H, 0 + H,   0, 255, 255);
 graph_plot_int(0, 0 + H, 0 - H, 0 + H, 255,   0, 255);
 graph_triangle(0);
 graph_plot_int(0, 0 - H, 0 - H, 0 + H,   0,   0, 255);
 graph_triangle(0);

 graph_plot_int(0, 0 + H, 0 + H, 0 + H, 255, 255, 255);
 graph_plot_int(0, 0 + H, 0 + H, 0 - H, 255, 255,   0);
 graph_plot_int(0, 0 + H, 0 - H, 0 + H, 255,   0, 255);
 graph_triangle(0);
 graph_plot_int(0, 0 + H, 0 - H, 0 - H, 255,   0,   0);
 graph_triangle(0);

 graph_plot_int(0, 0 + H, 0 + H, 0 + H, 255, 255, 255);
 graph_plot_int(0, 0 + H, 0 + H, 0 - H, 255, 255,   0);
 graph_plot_int(0, 0 - H, 0 + H, 0 + H,   0, 255, 255);
 graph_triangle(0);
 graph_plot_int(0, 0 - H, 0 + H, 0 - H,   0, 255,   0);
 graph_triangle(0);



 graph_plot_int(1, 0,     0,     0 + H,   0,   0, 255);
 graph_plot_int(1, 0 + H, 0,     0,     255,   0,   0);
 graph_plot_int(1, 0,     0 + H, 0,       0, 255,   0);
 graph_triangle(1);
 graph_plot_int(1, 0,     0,     0 - H,   0,   0, 255);
 graph_triangle(1);

 graph_plot_int(1, 0,     0,     0 + H,   0,   0, 255);
 graph_plot_int(1, 0 - H, 0,     0,     255,   0,   0);
 graph_plot_int(1, 0,     0 + H, 0,       0, 255,   0);
 graph_triangle(1);
 graph_plot_int(1, 0,     0,     0 - H,   0,   0, 255);
 graph_triangle(1);

 graph_plot_int(1, 0,     0,     0 + H,   0,   0, 255);
 graph_plot_int(1, 0 + H, 0,     0,     255,   0,   0);
 graph_plot_int(1, 0,     0 - H, 0,       0, 255,   0);
 graph_triangle(1);
 graph_plot_int(1, 0,     0,     0 - H,   0,   0, 255);
 graph_triangle(1);

 graph_plot_int(1, 0,     0,     0 + H,   0,   0, 255);
 graph_plot_int(1, 0 - H, 0,     0,     255,   0,   0);
 graph_plot_int(1, 0,     0 - H, 0,       0, 255,   0);
 graph_triangle(1);
 graph_plot_int(1, 0,     0,     0 - H,   0,   0, 255);
 graph_triangle(1);


 C__R = 128;
 C__G = 128;
 C__B = 128;

 C61R = C__R; C61G = C__G; C61B = C__B;
 C51R = C__R; C51G = C__G; C51B = C__B;
 C41R = C__R; C41G = C__G; C41B = C__B;
 C31R = C__R; C31G = C__G; C31B = C__B;
 C21R = C__R; C21G = C__G; C21B = C__B;
 C11R = C__R; C11G = C__G; C11B = C__B;
 C00R = C__R; C00G = C__G; C00B = C__B;
 C12R = C__R; C12G = C__G; C12B = C__B;
 C22R = C__R; C22G = C__G; C22B = C__B;
 C32R = C__R; C32G = C__G; C32B = C__B;
 C42R = C__R; C42G = C__G; C42B = C__B;
 C52R = C__R; C52G = C__G; C52B = C__B;
 C62R = C__R; C62G = C__G; C62B = C__B;

 C61R = 255; C61G = 255; C61B = 255;
 C51R =   0; C51G = 255; C51B = 255;
 C41R =   0; C41G =   0; C41B = 255;
 C31R =   0; C31G =   0; C31B =   0;
 C21R = 255; C21G =   0; C21B =   0;
 C11R = 255; C11G = 255; C11B =   0;
 C00R = 255; C00G = 255; C00B = 255;
 C12R =   0; C12G = 255; C12B = 255;
 C22R =   0; C22G =   0; C22B = 255;
 C32R =   0; C32G =   0; C32B =   0;
 C42R = 255; C42G =   0; C42B =   0;
 C52R = 255; C52G = 255; C52B =   0;
 C62R = 255; C62G = 255; C62B = 255;

 C61R = 128; C61G =   0; C61B =   0;
 C51R = 255; C51G =   0; C51B =   0;
 C41R = 255; C41G = 128; C41B =   0;
 C31R = 255; C31G = 255; C31B =   0;
 C21R = 128; C21G = 255; C21B =   0;
 C11R =   0; C11G = 255; C11B =   0;
 C00R =   0; C00G = 255; C00B = 128;
 C12R =   0; C12G = 255; C12B = 255;
 C22R =   0; C22G = 128; C22B = 255;
 C32R =   0; C32G =   0; C32B = 255;
 C42R = 128; C42G =   0; C42B = 255;
 C52R = 255; C52G =   0; C52B = 255;
 C62R = 128; C62G =   0; C62B = 128;


 X11 = cosf( 15.0 * angle_deg_rad) * H;
 Y11 = sinf( 15.0 * angle_deg_rad) * H;
 X21 = cosf( 30.0 * angle_deg_rad) * H;
 Y21 = sinf( 30.0 * angle_deg_rad) * H;
 X31 = cosf( 45.0 * angle_deg_rad) * H;
 Y31 = sinf( 45.0 * angle_deg_rad) * H;
 X41 = cosf( 60.0 * angle_deg_rad) * H;
 Y41 = sinf( 60.0 * angle_deg_rad) * H;
 X51 = cosf( 75.0 * angle_deg_rad) * H;
 Y51 = sinf( 75.0 * angle_deg_rad) * H;

 X12 = cosf(-15.0 * angle_deg_rad) * H;
 Y12 = sinf(-15.0 * angle_deg_rad) * H;
 X22 = cosf(-30.0 * angle_deg_rad) * H;
 Y22 = sinf(-30.0 * angle_deg_rad) * H;
 X32 = cosf(-45.0 * angle_deg_rad) * H;
 Y32 = sinf(-45.0 * angle_deg_rad) * H;
 X42 = cosf(-60.0 * angle_deg_rad) * H;
 Y42 = sinf(-60.0 * angle_deg_rad) * H;
 X52 = cosf(-75.0 * angle_deg_rad) * H;
 Y52 = sinf(-75.0 * angle_deg_rad) * H;

 II = 10;
 graph_clear(2);
 for (I = 0; I < 360; I += (II + II))
 {
  Ang1 = (I + 0 * II) * angle_deg_rad;
  Ang2 = (I + 1 * II) * angle_deg_rad;
  Ang3 = (I + 2 * II) * angle_deg_rad;
  Ang4 = (I + 3 * II) * angle_deg_rad;

  graph_plot_int(2,                0,   H,                0, C61R, C61G, C61B);
  graph_plot_int(2, cosf(Ang1) * X51, Y51, sinf(Ang1) * X51, C51R, C51G, C51B);
  graph_plot_int(2, cosf(Ang3) * X51, Y51, sinf(Ang3) * X51, C51R, C51G, C51B);
  graph_triangle(2);

  graph_plot_int(2,                0,  -H,                0, C62R, C62G, C62B);
  graph_plot_int(2, cosf(Ang1) * X52, Y52, sinf(Ang1) * X52, C52R, C52G, C52B);
  graph_plot_int(2, cosf(Ang3) * X52, Y52, sinf(Ang3) * X52, C52R, C52G, C52B);
  graph_triangle(2);

  graph_plot_int(2, cosf(Ang1) * X51, Y51, sinf(Ang1) * X51, C51R, C51G, C51B);
  graph_plot_int(2, cosf(Ang3) * X51, Y51, sinf(Ang3) * X51, C51R, C51G, C51B);
  graph_plot_int(2, cosf(Ang2) * X41, Y41, sinf(Ang2) * X41, C41R, C41G, C41B);
  graph_triangle(2);
  graph_plot_int(2, cosf(Ang4) * X41, Y41, sinf(Ang4) * X41, C41R, C41G, C41B);
  graph_triangle(2);

  graph_plot_int(2, cosf(Ang1) * X52, Y52, sinf(Ang1) * X52, C52R, C52G, C52B);
  graph_plot_int(2, cosf(Ang3) * X52, Y52, sinf(Ang3) * X52, C52R, C52G, C52B);
  graph_plot_int(2, cosf(Ang2) * X42, Y42, sinf(Ang2) * X42, C42R, C42G, C42B);
  graph_triangle(2);
  graph_plot_int(2, cosf(Ang4) * X42, Y42, sinf(Ang4) * X42, C42R, C42G, C42B);
  graph_triangle(2);

  graph_plot_int(2, cosf(Ang4) * X41, Y41, sinf(Ang4) * X41, C41R, C41G, C41B);
  graph_plot_int(2, cosf(Ang2) * X41, Y41, sinf(Ang2) * X41, C41R, C41G, C41B);
  graph_plot_int(2, cosf(Ang3) * X31, Y31, sinf(Ang3) * X31, C31R, C31G, C31B);
  graph_triangle(2);
  graph_plot_int(2, cosf(Ang1) * X31, Y31, sinf(Ang1) * X31, C31R, C31G, C31B);
  graph_triangle(2);

  graph_plot_int(2, cosf(Ang4) * X42, Y42, sinf(Ang4) * X42, C42R, C42G, C42B);
  graph_plot_int(2, cosf(Ang2) * X42, Y42, sinf(Ang2) * X42, C42R, C42G, C42B);
  graph_plot_int(2, cosf(Ang3) * X32, Y32, sinf(Ang3) * X32, C32R, C32G, C32B);
  graph_triangle(2);
  graph_plot_int(2, cosf(Ang1) * X32, Y32, sinf(Ang1) * X32, C32R, C32G, C32B);
  graph_triangle(2);

  graph_plot_int(2, cosf(Ang1) * X31, Y31, sinf(Ang1) * X31, C31R, C31G, C31B);
  graph_plot_int(2, cosf(Ang3) * X31, Y31, sinf(Ang3) * X31, C31R, C31G, C31B);
  graph_plot_int(2, cosf(Ang2) * X21, Y21, sinf(Ang2) * X21, C21R, C21G, C21B);
  graph_triangle(2);
  graph_plot_int(2, cosf(Ang4) * X21, Y21, sinf(Ang4) * X21, C21R, C21G, C21B);
  graph_triangle(2);

  graph_plot_int(2, cosf(Ang1) * X32, Y32, sinf(Ang1) * X32, C32R, C32G, C32B);
  graph_plot_int(2, cosf(Ang3) * X32, Y32, sinf(Ang3) * X32, C32R, C32G, C32B);
  graph_plot_int(2, cosf(Ang2) * X22, Y22, sinf(Ang2) * X22, C22R, C22G, C22B);
  graph_triangle(2);
  graph_plot_int(2, cosf(Ang4) * X22, Y22, sinf(Ang4) * X22, C22R, C22G, C22B);
  graph_triangle(2);

  graph_plot_int(2, cosf(Ang4) * X21, Y21, sinf(Ang4) * X21, C21R, C21G, C21B);
  graph_plot_int(2, cosf(Ang2) * X21, Y21, sinf(Ang2) * X21, C21R, C21G, C21B);
  graph_plot_int(2, cosf(Ang3) * X11, Y11, sinf(Ang3) * X11, C11R, C11G, C11B);
  graph_triangle(2);
  graph_plot_int(2, cosf(Ang1) * X11, Y11, sinf(Ang1) * X11, C11R, C11G, C11B);
  graph_triangle(2);

  graph_plot_int(2, cosf(Ang4) * X22, Y22, sinf(Ang4) * X22, C22R, C22G, C22B);
  graph_plot_int(2, cosf(Ang2) * X22, Y22, sinf(Ang2) * X22, C22R, C22G, C22B);
  graph_plot_int(2, cosf(Ang3) * X12, Y12, sinf(Ang3) * X12, C12R, C12G, C12B);
  graph_triangle(2);
  graph_plot_int(2, cosf(Ang1) * X12, Y12, sinf(Ang1) * X12, C12R, C12G, C12B);
  graph_triangle(2);

  graph_plot_int(2, cosf(Ang1) * X11, Y11, sinf(Ang1) * X11, C11R, C11G, C11B);
  graph_plot_int(2, cosf(Ang3) * X11, Y11, sinf(Ang3) * X11, C11R, C11G, C11B);
  graph_plot_int(2, cosf(Ang2) *   H,   0, sinf(Ang2) *   H, C00R, C00G, C00B);
  graph_triangle(2);
  graph_plot_int(2, cosf(Ang4) *   H,   0, sinf(Ang4) *   H, C00R, C00G, C00B);
  graph_triangle(2);

  graph_plot_int(2, cosf(Ang1) * X12, Y12, sinf(Ang1) * X12, C12R, C12G, C12B);
  graph_plot_int(2, cosf(Ang3) * X12, Y12, sinf(Ang3) * X12, C12R, C12G, C12B);
  graph_plot_int(2, cosf(Ang2) *   H,   0, sinf(Ang2) *   H, C00R, C00G, C00B);
  graph_triangle(2);
  graph_plot_int(2, cosf(Ang4) *   H,   0, sinf(Ang4) *   H, C00R, C00G, C00B);
  graph_triangle(2);
 }

 end();
}
