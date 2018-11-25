#include "core.h"
#include "console.h"
#include <stdlib.h>
#include "bundle.h"

void main()
{
 long T = 100;
 while (T > 0)
 {
  print_char_t(0, '1');
  T = T - 1;
 }
 print_char_t(0, '\n');
 bundle_run(1);
 stop();
 T = 100;
 while (T > 0)
 {
  print_char_t(0, '3');
  T = T - 1;
 }
 print_char_t(0, '\n');
 bundle_run(1);
 stop();

 print_char_t(0, '\n');
 bundle_run(1);
 T = 50;
 while (T > 0)
 {
  print_char_t(0, '1');
  T = T - 1;
 }
 end();
}
