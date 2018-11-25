#include "core.h"
#include "console.h"
#include <stdlib.h>
#include "bundle.h"

void main()
{
 long T = 100;
 while (T > 0)
 {
  print_char_t(0, '2');
  T = T - 1;
 }
 print_char_t(0, '\n');
 bundle_run(0);
 stop();
 T = 100;
 while (T > 0)
 {
  print_char_t(0, '4');
  T = T - 1;
 }
 print_char_t(0, '\n');
 bundle_run(0);
 stop();

 T = 50;
 while (T > 0)
 {
  print_char_t(0, '2');
  T = T - 1;
 }
 end();
}
