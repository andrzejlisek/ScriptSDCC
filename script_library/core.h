// Header file "_.h" is generated during compilation.
// This file defines macros:
// "mem_swap" - address in data memory reserved for swap (256 bytes)
// "engine_mcs51" or "engine_z180" without value - depends on selected engine
// "mem_common" or "mem_separated" without value - depends on selected CODE/DATA memory type
#include "_.h"

// Integer number type one-word names
#define uchar unsigned char
#define schar signed char
#define uint unsigned int
#define sint signed int
#define ulong unsigned long
#define slong signed long
#define ushort unsigned short
#define sshort signed short
#define llong long long
#define ullong unsigned long long
#define sllong signed long long

// Arrays giving absolute data memory access
#define _char ((char volatile *)0)
#define _uchar ((uchar volatile *)0)
#define _schar ((schar volatile *)0)
#define _short ((short volatile *)0)
#define _ushort ((ushort volatile *)0)
#define _sshort ((sshort volatile *)0)
#define _int ((int volatile *)0)
#define _uint ((uint volatile *)0)
#define _sint ((sint volatile *)0)
#define _long ((long volatile *)0)
#define _ulong ((ulong volatile *)0)
#define _slong ((slong volatile *)0)

// Replacement for MCS51-specified keywords to "mem_near" and "mem_far", which can be used also on Z180
#ifdef engine_z180
 #define _near
 #define _far
#endif
#ifdef engine_mcs51
 #define _near __idata
 #define _far __xdata
#endif

// Union for every data type, which is used to set and get values in swap memory
_far __at (mem_swap + 8) union core_io_union
{
 schar RawS[8];
 uchar RawU[8];
 schar BufS1;
 uchar BufU1;
 sshort BufS2;
 ushort BufU2;
 sint BufS2x;
 uint BufU2x;
 slong BufS4;
 ulong BufU4;
 float BufF;
} core_io;

// Copys 8 bytes from one location to other location, used in swap memory
// Src - source address
// Dst - destination address
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

// Returns number of instructions from running or last prgcounter invocation
// div - Exponent of 10 to be a divider
// Retunrs: Number of instructions divided by 10^div
ulong prgcounter(uchar div)
{
 core_io.BufU1 = div;
 _uchar[mem_swap] = 252;
 return core_io.BufU4;
}

// Stops script and allows user to continue by clicking "Run" button
#define stop() _uchar[mem_swap] = 254;

// Stops script without continuation possibility
#define end() _uchar[mem_swap] = 253;

// Sets specified string to string buffer
// V - Pointer to null-terminated string
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

// Gets string buffer contents to specified string
// V - Pointer to null-terminated string
// S - Maximum string length, the longer string will be truncated
void string_get(char * V, ushort S)
{
 ushort I, N;
 _uchar[mem_swap] = 248;
 N = core_io.BufU2;
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
