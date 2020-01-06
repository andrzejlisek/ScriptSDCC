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
// console_print_schar_v, console_print_uchar_v - prints value as number
// console_print_schar_t, console_print_uchar_t - prints value as 1-character text
void console_print_schar_v(uchar N, schar  V) { core_io.BufS1  = V; _uchar[mem_swap + 1] =  0; _uchar[mem_swap] = IOConsoleCmd + 4 + N; }
void console_print_uchar_v(uchar N, uchar  V) { core_io.BufU1  = V; _uchar[mem_swap + 1] =  1; _uchar[mem_swap] = IOConsoleCmd + 4 + N; }
void console_print_schar_t(uchar N, schar  V) { core_io.BufS1  = V; _uchar[mem_swap + 1] =  2; _uchar[mem_swap] = IOConsoleCmd + 4 + N; }
void console_print_uchar_t(uchar N, uchar  V) { core_io.BufU1  = V; _uchar[mem_swap + 1] =  3; _uchar[mem_swap] = IOConsoleCmd + 4 + N; }
void console_print_sshort( uchar N, sshort V) { core_io.BufS2  = V; _uchar[mem_swap + 1] =  4; _uchar[mem_swap] = IOConsoleCmd + 4 + N; }
void console_print_ushort( uchar N, ushort V) { core_io.BufU2  = V; _uchar[mem_swap + 1] =  5; _uchar[mem_swap] = IOConsoleCmd + 4 + N; }
void console_print_sint(   uchar N, sint   V) { core_io.BufS2x = V; _uchar[mem_swap + 1] =  6; _uchar[mem_swap] = IOConsoleCmd + 4 + N; }
void console_print_uint(   uchar N, uint   V) { core_io.BufU2x = V; _uchar[mem_swap + 1] =  7; _uchar[mem_swap] = IOConsoleCmd + 4 + N; }
void console_print_slong(  uchar N, slong  V) { core_io.BufS4  = V; _uchar[mem_swap + 1] =  8; _uchar[mem_swap] = IOConsoleCmd + 4 + N; }
void console_print_ulong(  uchar N, ulong  V) { core_io.BufU4  = V; _uchar[mem_swap + 1] =  9; _uchar[mem_swap] = IOConsoleCmd + 4 + N; }
void console_print_float(  uchar N, float  V) { core_io.BufF   = V; _uchar[mem_swap + 1] = 10; _uchar[mem_swap] = IOConsoleCmd + 4 + N; }

// Prints specified string
// N - Console number
// V - Null-terminated string to print
void console_print_string(uchar N, char * V)
{
 uint I = 0;
 while (V[I] != 0)
 {
  console_print_uchar_t(N, V[I]);
  I++;
 }
}

// Prints end-of-line character
// N - Console number
void inline console_print_line(uchar N)
{
 console_print_uchar_t(N, '\n');
}

// Sets float number format
// N - Console number
// V - Positive value:
//     Negative value:
void inline console_print_float_format(uchar N, char V)
{
 core_io.BufS1 = V;
 _uchar[mem_swap] = IOConsoleCmd + 8 + N;
}

// Gets one character from console input buffer as signed value
// Waits while buffer is empty
// N - Console number
schar console_input_schar(uchar N)
{
 schar V = 0;
 while (V == 0)
 {
  _uchar[mem_swap] = IOConsoleCmd + 12 + N;
  V = core_io.BufS1;
 }
 return V;
}

// Gets one character from console input buffer as unsigned value
// Waits while buffer is empty
// N - Console number
uchar console_input_uchar(uchar N)
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
void inline console_input_flush(uchar N)
{
 _uchar[mem_swap] = IOConsoleCmd + 20 + N;
}
