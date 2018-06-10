#include "_.h"

#define _uchar ((unsigned char volatile *)0)
#define _char ((char volatile *)0)
#define _uint ((unsigned int volatile *)0)
#define _int ((int volatile *)0)
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
#define ushort unsigned short
#define llong long long
#define ullong unsigned long long

#ifdef engine_z180
 #define mem_near
 #define mem_far
#endif
#ifdef engine_mcs51
 #define mem_near __idata
 #define mem_far __xdata
#endif


mem_far __at (mem_swap + 8) union core_io_union
{
 char RawS[8];
 uchar RawU[8];
 char BufS1;
 uchar BufU1;
 short BufS2;
 ushort BufU2;
 int BufS2x;
 uint BufU2x;
 long BufS4;
 ulong BufU4;
 float BufF;
} core_io;


void core_value_copy(uint Src, uint Dst)
{
 _uchar[Dst + 0] = _uchar[Src + 0];
 _uchar[Dst + 1] = _uchar[Src + 1];
 _uchar[Dst + 2] = _uchar[Src + 2];
 _uchar[Dst + 3] = _uchar[Src + 3];
 _uchar[Dst + 4] = _uchar[Src + 4];
 _uchar[Dst + 5] = _uchar[Src + 5];
 _uchar[Dst + 6] = _uchar[Src + 6];
 _uchar[Dst + 7] = _uchar[Src + 7];
}

ulong prgcounter(uchar div)
{
 core_io.BufU1 = div;
 _uchar[mem_swap] = 252;
 return core_io.BufU4;
}

void stop()
{
 _uchar[mem_swap] = 254;
}

void end()
{
 _uchar[mem_swap] = 253;
}

void string_set(char * V)
{
 uchar I = 0;
 _uchar[mem_swap] = 250;
 while (V[I] != 0)
 {
  core_io.BufS1 = V[I];
  _uchar[mem_swap] = 251;
  I++;
 }
}

void string_get(char * V, uchar S)
{
 uchar I, N;
 _uchar[mem_swap] = 248;
 N = core_io.BufU1;
 if (N > S)
 {
  N = S;
 }
 for (I = 0; I < N; I++)
 {
  _uchar[mem_swap] = 249;
  V[I] = core_io.BufS1;
 }
 V[N] = 0;
}
