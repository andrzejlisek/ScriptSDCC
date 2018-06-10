#define IOSpreadsheetCmd 32

char   cell_get_char_v( uchar N, uchar R, uchar C) { _uchar[mem_swap + 1] =  0; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; _uchar[mem_swap] = IOSpreadsheetCmd + 0 + N; return core_io.BufS1; }
uchar  cell_get_uchar_v(uchar N, uchar R, uchar C) { _uchar[mem_swap + 1] =  1; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; _uchar[mem_swap] = IOSpreadsheetCmd + 0 + N; return core_io.BufU1; }
char   cell_get_char_t( uchar N, uchar R, uchar C) { _uchar[mem_swap + 1] =  2; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; _uchar[mem_swap] = IOSpreadsheetCmd + 0 + N; return core_io.BufS1; }
uchar  cell_get_uchar_t(uchar N, uchar R, uchar C) { _uchar[mem_swap + 1] =  3; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; _uchar[mem_swap] = IOSpreadsheetCmd + 0 + N; return core_io.BufU1; }
short  cell_get_short(  uchar N, uchar R, uchar C) { _uchar[mem_swap + 1] =  4; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; _uchar[mem_swap] = IOSpreadsheetCmd + 0 + N; return core_io.BufS2; }
ushort cell_get_ushort( uchar N, uchar R, uchar C) { _uchar[mem_swap + 1] =  5; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; _uchar[mem_swap] = IOSpreadsheetCmd + 0 + N; return core_io.BufU2; }
int    cell_get_int(    uchar N, uchar R, uchar C) { _uchar[mem_swap + 1] =  6; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; _uchar[mem_swap] = IOSpreadsheetCmd + 0 + N; return core_io.BufS2; }
uint   cell_get_uint(   uchar N, uchar R, uchar C) { _uchar[mem_swap + 1] =  7; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; _uchar[mem_swap] = IOSpreadsheetCmd + 0 + N; return core_io.BufU2; }
long   cell_get_long(   uchar N, uchar R, uchar C) { _uchar[mem_swap + 1] =  8; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; _uchar[mem_swap] = IOSpreadsheetCmd + 0 + N; return core_io.BufS4; }
ulong  cell_get_ulong(  uchar N, uchar R, uchar C) { _uchar[mem_swap + 1] =  9; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; _uchar[mem_swap] = IOSpreadsheetCmd + 0 + N; return core_io.BufU4; }
float  cell_get_float(  uchar N, uchar R, uchar C) { _uchar[mem_swap + 1] = 10; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; _uchar[mem_swap] = IOSpreadsheetCmd + 0 + N; return core_io.BufF;  }

void cell_get_string(uchar N, uchar R, uchar C, char * V, uchar S)
{
 _uchar[mem_swap + 1] = R;
 _uchar[mem_swap + 2] = C;
 _uchar[mem_swap] = IOSpreadsheetCmd + 4 + N;
 string_get(V, S);
}

void cell_set_char_v( uchar N, uchar R, uchar C, char V)   { _uchar[mem_swap + 1] =  0; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; core_io.BufS1 = V; _uchar[mem_swap] = IOSpreadsheetCmd + 8 + N; }
void cell_set_uchar_v(uchar N, uchar R, uchar C, uchar V)  { _uchar[mem_swap + 1] =  1; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; core_io.BufU1 = V; _uchar[mem_swap] = IOSpreadsheetCmd + 8 + N; }
void cell_set_char_t( uchar N, uchar R, uchar C, char V)   { _uchar[mem_swap + 1] =  2; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; core_io.BufS1 = V; _uchar[mem_swap] = IOSpreadsheetCmd + 8 + N; }
void cell_set_uchar_t(uchar N, uchar R, uchar C, uchar V)  { _uchar[mem_swap + 1] =  3; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; core_io.BufU1 = V; _uchar[mem_swap] = IOSpreadsheetCmd + 8 + N; }
void cell_set_short(  uchar N, uchar R, uchar C, short V)  { _uchar[mem_swap + 1] =  4; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; core_io.BufS2 = V; _uchar[mem_swap] = IOSpreadsheetCmd + 8 + N; }
void cell_set_ushort( uchar N, uchar R, uchar C, ushort V) { _uchar[mem_swap + 1] =  5; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; core_io.BufU2 = V; _uchar[mem_swap] = IOSpreadsheetCmd + 8 + N; }
void cell_set_int(    uchar N, uchar R, uchar C, int V)    { _uchar[mem_swap + 1] =  6; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; core_io.BufS2 = V; _uchar[mem_swap] = IOSpreadsheetCmd + 8 + N; }
void cell_set_uint(   uchar N, uchar R, uchar C, uint V)   { _uchar[mem_swap + 1] =  7; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; core_io.BufU2 = V; _uchar[mem_swap] = IOSpreadsheetCmd + 8 + N; }
void cell_set_long(   uchar N, uchar R, uchar C, long V)   { _uchar[mem_swap + 1] =  8; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; core_io.BufS4 = V; _uchar[mem_swap] = IOSpreadsheetCmd + 8 + N; }
void cell_set_ulong(  uchar N, uchar R, uchar C, ulong V)  { _uchar[mem_swap + 1] =  9; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; core_io.BufU4 = V; _uchar[mem_swap] = IOSpreadsheetCmd + 8 + N; }
void cell_set_float(  uchar N, uchar R, uchar C, float V)  { _uchar[mem_swap + 1] = 10; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; core_io.BufF  = V; _uchar[mem_swap] = IOSpreadsheetCmd + 8 + N; }

void cell_set_string(uchar N, uchar R, uchar C, char * V)
{
 string_set(V);
 _uchar[mem_swap + 1] = R;
 _uchar[mem_swap + 2] = C;
 _uchar[mem_swap] = IOSpreadsheetCmd + 12 + N;
}

void sheet_clear(uchar N, uchar R1, uchar C1, uchar R2, uchar C2)
{
 _uchar[mem_swap + 1] = 0;
 _uchar[mem_swap + 2] = R1;
 _uchar[mem_swap + 3] = C1;
 _uchar[mem_swap + 4] = R2;
 _uchar[mem_swap + 5] = C2;
 _uchar[mem_swap] = IOSpreadsheetCmd + 16 + N;
}

void sheet_rowadd(uchar N, uchar R1, uchar C1, uchar R2, uchar C2)
{
 _uchar[mem_swap + 1] = 1;
 _uchar[mem_swap + 2] = R1;
 _uchar[mem_swap + 3] = C1;
 _uchar[mem_swap + 4] = R2;
 _uchar[mem_swap + 5] = C2;
 _uchar[mem_swap] = IOSpreadsheetCmd + 16 + N;
}

void sheet_rowrem(uchar N, uchar R1, uchar C1, uchar R2, uchar C2)
{
 _uchar[mem_swap + 1] = 2;
 _uchar[mem_swap + 2] = R1;
 _uchar[mem_swap + 3] = C1;
 _uchar[mem_swap + 4] = R2;
 _uchar[mem_swap + 5] = C2;
 _uchar[mem_swap] = IOSpreadsheetCmd + 16 + N;
}

void sheet_coladd(uchar N, uchar R1, uchar C1, uchar R2, uchar C2)
{
 _uchar[mem_swap + 1] = 3;
 _uchar[mem_swap + 2] = R1;
 _uchar[mem_swap + 3] = C1;
 _uchar[mem_swap + 4] = R2;
 _uchar[mem_swap + 5] = C2;
 _uchar[mem_swap] = IOSpreadsheetCmd + 16 + N;
}

void sheet_colrem(uchar N, uchar R1, uchar C1, uchar R2, uchar C2)
{
 _uchar[mem_swap + 1] = 4;
 _uchar[mem_swap + 2] = R1;
 _uchar[mem_swap + 3] = C1;
 _uchar[mem_swap + 4] = R2;
 _uchar[mem_swap + 5] = C2;
 _uchar[mem_swap] = IOSpreadsheetCmd + 16 + N;
}
