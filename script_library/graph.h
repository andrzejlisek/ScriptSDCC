#define IOGraphCmd 60

// Clears whole graphics
// N - Graph number
void inline graph_clear(uchar N)
{
 _uchar[mem_swap] = IOGraphCmd + 0 + N;
}

// Sets polyline thread
// N - Graph number
// T - Thread number
void inline graph_thread(uchar N, uchar T)
{
 _uchar[mem_swap + 1] = T;
 _uchar[mem_swap] = IOGraphCmd + 4 + N;
}

// Resets current polyline thread
// N - Graph number
void inline graph_thread_reset(uchar N)
{
 _uchar[mem_swap] = IOGraphCmd + 8 + N;
}

// Macros used by all graph_plot functions
#define __graph_plot_common1(T) T = X; core_value_copy(mem_swap + 0x08, mem_swap + 0x10);
#define __graph_plot_common2(T) T = Y; core_value_copy(mem_swap + 0x08, mem_swap + 0x18);
#define __graph_plot_common3(T) T = Z; core_value_copy(mem_swap + 0x08, mem_swap + 0x20);
#define __graph_plot_common4(T) _uchar[mem_swap + 1] = T;
#define __graph_plot_common5 _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = G; _uchar[mem_swap + 4] = B;
#define __graph_plot_common6 _uchar[mem_swap] = IOGraphCmd + 12 + N;

// Sets points on screen
// N - Graph number
// X, Y, Z - Point coordinates
// R, G, B - Point color
void graph_plot_char(  uchar N,   char X,   char Y,   char Z, uchar R, uchar G, uchar B) { __graph_plot_common1(core_io.BufS1)  __graph_plot_common2(core_io.BufS1)  __graph_plot_common3(core_io.BufS1)  __graph_plot_common4(0)  __graph_plot_common5 __graph_plot_common6 }
void graph_plot_uchar( uchar N,  uchar X,  uchar Y,  uchar Z, uchar R, uchar G, uchar B) { __graph_plot_common1(core_io.BufU1)  __graph_plot_common2(core_io.BufU1)  __graph_plot_common3(core_io.BufU1)  __graph_plot_common4(1)  __graph_plot_common5 __graph_plot_common6 }
void graph_plot_short( uchar N,  short X,  short Y,  short Z, uchar R, uchar G, uchar B) { __graph_plot_common1(core_io.BufS2)  __graph_plot_common2(core_io.BufS2)  __graph_plot_common3(core_io.BufS2)  __graph_plot_common4(4)  __graph_plot_common5 __graph_plot_common6 }
void graph_plot_ushort(uchar N, ushort X, ushort Y, ushort Z, uchar R, uchar G, uchar B) { __graph_plot_common1(core_io.BufU2)  __graph_plot_common2(core_io.BufU2)  __graph_plot_common3(core_io.BufU2)  __graph_plot_common4(5)  __graph_plot_common5 __graph_plot_common6 }
void graph_plot_int(   uchar N,    int X,    int Y,    int Z, uchar R, uchar G, uchar B) { __graph_plot_common1(core_io.BufS2x) __graph_plot_common2(core_io.BufS2x) __graph_plot_common3(core_io.BufS2x) __graph_plot_common4(6)  __graph_plot_common5 __graph_plot_common6 }
void graph_plot_uint(  uchar N,   uint X,   uint Y,   uint Z, uchar R, uchar G, uchar B) { __graph_plot_common1(core_io.BufU2x) __graph_plot_common2(core_io.BufU2x) __graph_plot_common3(core_io.BufU2x) __graph_plot_common4(7)  __graph_plot_common5 __graph_plot_common6 }
void graph_plot_long(  uchar N,   long X,   long Y,   long Z, uchar R, uchar G, uchar B) { __graph_plot_common1(core_io.BufS4)  __graph_plot_common2(core_io.BufS4)  __graph_plot_common3(core_io.BufS4)  __graph_plot_common4(8)  __graph_plot_common5 __graph_plot_common6 }
void graph_plot_ulong( uchar N,  ulong X,  ulong Y,  ulong Z, uchar R, uchar G, uchar B) { __graph_plot_common1(core_io.BufU4)  __graph_plot_common2(core_io.BufU4)  __graph_plot_common3(core_io.BufU4)  __graph_plot_common4(9)  __graph_plot_common5 __graph_plot_common6 }
void graph_plot_float( uchar N,  float X,  float Y,  float Z, uchar R, uchar G, uchar B) { __graph_plot_common1(core_io.BufF)   __graph_plot_common2(core_io.BufF)   __graph_plot_common3(core_io.BufF)   __graph_plot_common4(10) __graph_plot_common5 __graph_plot_common6 }

// Converts last three points to triangle
// N - Graph number
void inline graph_triangle(uchar N)
{
 _uchar[mem_swap + 1] = 0;
 _uchar[mem_swap] = IOGraphCmd + 16 + N;
}

// Converts last points to polyline
// N - Graph number
// L - Number of last points to convert
void inline graph_line(uchar N, uchar L)
{
 _uchar[mem_swap + 1] = L;
 _uchar[mem_swap] = IOGraphCmd + 16 + N;
}
