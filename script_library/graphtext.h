#define IOGraphTextCmd 80

// Sets text color
// N - Graph number
// DispBackFore - 0 => Transparent background and foreground
//                1 => Transparent background, opaque foreground
//                2 => Opaque background, transparent foreground
//                3 => Opaque background and foreground
// ColorBackR, ColorBackG, ColorBackB - Background color
// ColorForeR, ColorForeG, ColorForeB - Foreground color
void graph_text_color(uchar N, uchar DispBackFore, uchar ColorBackR, uchar ColorBackG, uchar ColorBackB, uchar ColorForeR, uchar ColorForeG, uchar ColorForeB)
{
 _uchar[mem_swap + 1] = DispBackFore;
 _uchar[mem_swap + 2] = ColorBackR;
 _uchar[mem_swap + 3] = ColorBackG;
 _uchar[mem_swap + 4] = ColorBackB;
 _uchar[mem_swap + 5] = ColorForeR;
 _uchar[mem_swap + 6] = ColorForeG;
 _uchar[mem_swap + 7] = ColorForeB;
 _uchar[mem_swap] = IOGraphTextCmd + 0 + N;
}

// Allows to simplify define colors when olny background or only foreground color is needed
// N - Graph number
// R, G, B - Text color
#define graph_text_color_f(N, R, G, B) graph_text_color(N, 1, 0, 0, 0, R, G, B)
#define graph_text_color_b(N, R, G, B) graph_text_color(N, 2, R, G, B, 0, 0, 0)

// Macros used by all graph_text_xy functions
#define __graph_text_xy_common1(T) T = X; core_value_copy(mem_swap + 0x08, mem_swap + 0x10);
#define __graph_text_xy_common2(T) T = Y; core_value_copy(mem_swap + 0x08, mem_swap + 0x18);
#define __graph_text_xy_common3(T) T = Z; core_value_copy(mem_swap + 0x08, mem_swap + 0x20);
#define __graph_text_xy_common4(T) _uchar[mem_swap + 1] = T;
#define __graph_text_xy_common5 core_io.BufS2 = X0; core_value_copy(mem_swap + 0x08, mem_swap + 0x28);
#define __graph_text_xy_common6 core_io.BufS2 = Y0; core_value_copy(mem_swap + 0x08, mem_swap + 0x30);
#define __graph_text_xy_common7 _uchar[mem_swap] = IOGraphTextCmd + 4 + N;

// Sets text print coordinates
// N - Graph number
// X, Y, Z - Text base point coordinates
// X0, Y0 - Offset on screen between middle point of text and base point
void graph_text_xy_char(  uchar N,   char X,   char Y,   char Z, short X0, short Y0) { __graph_text_xy_common1(core_io.BufS1)  __graph_text_xy_common2(core_io.BufS1)  __graph_text_xy_common3(core_io.BufS1)  __graph_text_xy_common4(0)  __graph_text_xy_common5 __graph_text_xy_common6 __graph_text_xy_common7 }
void graph_text_xy_uchar( uchar N,  uchar X,  uchar Y,  uchar Z, short X0, short Y0) { __graph_text_xy_common1(core_io.BufU1)  __graph_text_xy_common2(core_io.BufU1)  __graph_text_xy_common3(core_io.BufU1)  __graph_text_xy_common4(1)  __graph_text_xy_common5 __graph_text_xy_common6 __graph_text_xy_common7 }
void graph_text_xy_short( uchar N,  short X,  short Y,  short Z, short X0, short Y0) { __graph_text_xy_common1(core_io.BufS2)  __graph_text_xy_common2(core_io.BufS2)  __graph_text_xy_common3(core_io.BufS2)  __graph_text_xy_common4(4)  __graph_text_xy_common5 __graph_text_xy_common6 __graph_text_xy_common7 }
void graph_text_xy_ushort(uchar N, ushort X, ushort Y, ushort Z, short X0, short Y0) { __graph_text_xy_common1(core_io.BufU2)  __graph_text_xy_common2(core_io.BufU2)  __graph_text_xy_common3(core_io.BufU2)  __graph_text_xy_common4(5)  __graph_text_xy_common5 __graph_text_xy_common6 __graph_text_xy_common7 }
void graph_text_xy_int(   uchar N,    int X,    int Y,    int Z, short X0, short Y0) { __graph_text_xy_common1(core_io.BufS2x) __graph_text_xy_common2(core_io.BufS2x) __graph_text_xy_common3(core_io.BufS2x) __graph_text_xy_common4(6)  __graph_text_xy_common5 __graph_text_xy_common6 __graph_text_xy_common7 }
void graph_text_xy_uint(  uchar N,   uint X,   uint Y,   uint Z, short X0, short Y0) { __graph_text_xy_common1(core_io.BufU2x) __graph_text_xy_common2(core_io.BufU2x) __graph_text_xy_common3(core_io.BufU2x) __graph_text_xy_common4(7)  __graph_text_xy_common5 __graph_text_xy_common6 __graph_text_xy_common7 }
void graph_text_xy_long(  uchar N,   long X,   long Y,   long Z, short X0, short Y0) { __graph_text_xy_common1(core_io.BufS4)  __graph_text_xy_common2(core_io.BufS4)  __graph_text_xy_common3(core_io.BufS4)  __graph_text_xy_common4(8)  __graph_text_xy_common5 __graph_text_xy_common6 __graph_text_xy_common7 }
void graph_text_xy_ulong( uchar N,  ulong X,  ulong Y,  ulong Z, short X0, short Y0) { __graph_text_xy_common1(core_io.BufU4)  __graph_text_xy_common2(core_io.BufU4)  __graph_text_xy_common3(core_io.BufU4)  __graph_text_xy_common4(9)  __graph_text_xy_common5 __graph_text_xy_common6 __graph_text_xy_common7 }
void graph_text_xy_float( uchar N,  float X,  float Y,  float Z, short X0, short Y0) { __graph_text_xy_common1(core_io.BufF)   __graph_text_xy_common2(core_io.BufF)   __graph_text_xy_common3(core_io.BufF)   __graph_text_xy_common4(10) __graph_text_xy_common5 __graph_text_xy_common6 __graph_text_xy_common7 }

// Paints value as text
// N - Graph number
// V - Value to paint
// graph_text_char_v, graph_text_uchar_v - paints value as number
// graph_text_char_t, graph_text_uchar_t - paints value as 1-character text
void graph_text_char_v( uchar N, char V)   { char V_ = V;   _uchar[mem_swap + 1] =  0; core_io.BufS1 = V_; _uchar[mem_swap] = IOGraphTextCmd + 8 + N; }
void graph_text_uchar_v(uchar N, uchar V)  { uchar V_ = V;  _uchar[mem_swap + 1] =  1; core_io.BufU1 = V_; _uchar[mem_swap] = IOGraphTextCmd + 8 + N; }
void graph_text_char_t( uchar N, char V)   { char V_ = V;   _uchar[mem_swap + 1] =  2; core_io.BufS1 = V_; _uchar[mem_swap] = IOGraphTextCmd + 8 + N; }
void graph_text_uchar_t(uchar N, uchar V)  { uchar V_ = V;  _uchar[mem_swap + 1] =  3; core_io.BufU1 = V_; _uchar[mem_swap] = IOGraphTextCmd + 8 + N; }
void graph_text_short(  uchar N, short V)  { short V_ = V;  _uchar[mem_swap + 1] =  4; core_io.BufS2 = V_; _uchar[mem_swap] = IOGraphTextCmd + 8 + N; }
void graph_text_ushort( uchar N, ushort V) { ushort V_ = V; _uchar[mem_swap + 1] =  5; core_io.BufU2 = V_; _uchar[mem_swap] = IOGraphTextCmd + 8 + N; }
void graph_text_int(    uchar N, int V)    { int V_ = V;    _uchar[mem_swap + 1] =  6; core_io.BufS2 = V_; _uchar[mem_swap] = IOGraphTextCmd + 8 + N; }
void graph_text_uint(   uchar N, uint V)   { uint V_ = V;   _uchar[mem_swap + 1] =  7; core_io.BufU2 = V_; _uchar[mem_swap] = IOGraphTextCmd + 8 + N; }
void graph_text_long(   uchar N, long V)   { long V_ = V;   _uchar[mem_swap + 1] =  8; core_io.BufS4 = V_; _uchar[mem_swap] = IOGraphTextCmd + 8 + N; }
void graph_text_ulong(  uchar N, ulong V)  { ulong V_ = V;  _uchar[mem_swap + 1] =  9; core_io.BufU4 = V_; _uchar[mem_swap] = IOGraphTextCmd + 8 + N; }
void graph_text_float(  uchar N, float V)  { float V_ = V;  _uchar[mem_swap + 1] = 10; core_io.BufF  = V_; _uchar[mem_swap] = IOGraphTextCmd + 8 + N; }

// Paints specified string
// N - Graph number
// V - Null-terminated string to paint
void graph_text_string(uchar N, char * V)
{
 string_set(V);
 _uchar[mem_swap] = IOGraphTextCmd + 12 + N;
}
