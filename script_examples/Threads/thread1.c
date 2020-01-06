#include "core.h"
#include "console.h"
#include <stdlib.h>
#include "bundle.h"

void main()
{
 long T = 100;
 int L = 100;
 while (T > 0)
 {
  console_print_uchar_t(0, '1');
  T = T - 1;
 }
 console_print_line(0);
 bundle_run(1);
 stop();
 T = 100;
 while (T > 0)
 {
  console_print_uchar_t(0, '3');
  T = T - 1;
 }
 console_print_line(0);
 bundle_run(1);
 stop();

 console_print_line(0);
 bundle_run(1);
 while (L > 0)
 {
  T = 50;
  while (T > 0)
  {
   console_print_uchar_t(0, '/');
   T = T - 1;
  }
  console_print_line(0);
  L--;
 }

 while (bundle_status(1) == 1)
 {
 }
 console_print_line(0);

 T = 100;
 while (T > 0)
 {
  console_print_uchar_t(0, '4');
  T = T - 1;
 }
 console_print_line(0);
 bundle_run(1);
 stop();
 T = 100;
 while (T > 0)
 {
  console_print_uchar_t(0, '2');
  T = T - 1;
 }
 console_print_line(0);
 bundle_run(1);

 end();
}
