#define IOConsoleCmd 4

// Clears console
// N - Console number
void inline console_clear(uchar N)
{
 _uchar[mem_swap] = IOConsoleCmd + 0 + N;
}

// Prints value
// N - Console number
// V - Value
// print_char_v, print_uchar_v - prints value as number
// print_char_t, print_uchar_t - prints value as 1-character text
void print_char_v( uchar N, char   V) { core_io.BufS1  = V; _uchar[mem_swap + 1] =  0; _uchar[mem_swap] = IOConsoleCmd + 4 + N; }
void print_uchar_v(uchar N, uchar  V) { core_io.BufU1  = V; _uchar[mem_swap + 1] =  1; _uchar[mem_swap] = IOConsoleCmd + 4 + N; }
void print_char_t( uchar N, char   V) { core_io.BufS1  = V; _uchar[mem_swap + 1] =  2; _uchar[mem_swap] = IOConsoleCmd + 4 + N; }
void print_uchar_t(uchar N, uchar  V) { core_io.BufU1  = V; _uchar[mem_swap + 1] =  3; _uchar[mem_swap] = IOConsoleCmd + 4 + N; }
void print_short(  uchar N, short  V) { core_io.BufS2  = V; _uchar[mem_swap + 1] =  4; _uchar[mem_swap] = IOConsoleCmd + 4 + N; }
void print_ushort( uchar N, ushort V) { core_io.BufU2  = V; _uchar[mem_swap + 1] =  5; _uchar[mem_swap] = IOConsoleCmd + 4 + N; }
void print_int(    uchar N, int    V) { core_io.BufS2x = V; _uchar[mem_swap + 1] =  6; _uchar[mem_swap] = IOConsoleCmd + 4 + N; }
void print_uint(   uchar N, uint   V) { core_io.BufU2x = V; _uchar[mem_swap + 1] =  7; _uchar[mem_swap] = IOConsoleCmd + 4 + N; }
void print_long(   uchar N, long   V) { core_io.BufS4  = V; _uchar[mem_swap + 1] =  8; _uchar[mem_swap] = IOConsoleCmd + 4 + N; }
void print_ulong(  uchar N, ulong  V) { core_io.BufU4  = V; _uchar[mem_swap + 1] =  9; _uchar[mem_swap] = IOConsoleCmd + 4 + N; }
void print_float(  uchar N, float  V) { core_io.BufF   = V; _uchar[mem_swap + 1] = 10; _uchar[mem_swap] = IOConsoleCmd + 4 + N; }

// Prints specified string
// N - Console number
// V - Null-terminated string to print
void print_string(uchar N, char * V)
{
 uint I = 0;
 while (V[I] != 0)
 {
  print_char_t(N, V[I]);
  I++;
 }
}

// Prints end-of-line character
// N - Console number
void inline print_line(uchar N)
{
 print_char_t(N, '\n');
}

// Sets float number format
// N - Console number
// V - Positive value:
//     Negative value:
void inline print_float_format(uchar N, char V)
{
 core_io.BufS1 = V;
 _uchar[mem_swap] = IOConsoleCmd + 8 + N;
}

// Gets one character from console input buffer as unsigned value
// Waits while buffer is empty
// N - Console number
char input_char(uchar N)
{
 char V = 0;
 while (V == 0)
 {
  _uchar[mem_swap] = IOConsoleCmd + 12 + N;
  V = core_io.BufS1;
 }
 return V;
}

// Gets one character from console input buffer as signed value
// Waits while buffer is empty
// N - Console number
uchar input_uchar(uchar N)
{
 uchar V = 0;
 while (V == 0)
 {
  _uchar[mem_swap] = IOConsoleCmd + 16 + N;
  V = core_io.BufU1;
 }
 return V;
}

// Clears console input buffer
// N - Console number
void inline input_flush(uchar N)
{
 _uchar[mem_swap] = IOConsoleCmd + 20 + N;
}
