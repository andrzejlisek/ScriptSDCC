#include "core.h"
#include "console.h"
#include <math.h>
#include <stdlib.h>

ulong T1[100];
ulong T2[100];
int I, J;
int Size = 31;

void main()
{
 console_clear(0);
 console_clear(1);
 

 T1[0] = 1;
 T1[1] = 1;

 for (I = -1; I < Size; I++)
 {
  for (J = 0; J < I; J++)
  {
   T2[J] = T1[J] + T1[J + 1];
  }

  T1[0] = 1;
  T1[I + 1] = 1;
  for (J = 0; J < I; J++)
  {
   T1[J + 1] = T2[J];
  }

  for (J = 0; J <= (Size - I); J++)
  {
   console_print_schar_t(1, ' ');
   console_print_string(0, "      ");
  }
  for (J = 0; J <= (I + 1); J++)
  {
   console_print_string(0, "  ");
   if (T1[J] < 1000000000) { console_print_schar_t(0, ' '); }
   if (T1[J] < 100000000)  { console_print_schar_t(0, ' '); }
   if (T1[J] < 10000000)   { console_print_schar_t(0, ' '); }
   if (T1[J] < 1000000)    { console_print_schar_t(0, ' '); }
   if (T1[J] < 100000)     { console_print_schar_t(0, ' '); }
   if (T1[J] < 10000)      { console_print_schar_t(0, ' '); }
   if (T1[J] < 1000)       { console_print_schar_t(0, ' '); }
   if (T1[J] < 100)        { console_print_schar_t(0, ' '); }
   if (T1[J] < 10)         { console_print_schar_t(0, ' '); }
   
   console_print_ulong(0, T1[J]);
   if ((T1[J] % 2) == 0)
   {
    console_print_string(1, "  ");
   }
   else
   {
    console_print_string(1, "[]");
   }
  }

  console_print_line(0);
  console_print_line(1);
 }
 end();
}

