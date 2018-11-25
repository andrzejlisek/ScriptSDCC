#define IOSpreadsheetCmd 32

// Gets cell value from spreadsheet
// N - Spreadsheet number
// R - Row number
// C - Colun number
// cell_get_char_v, cell_get_char_v - gets value from number
// cell_get_char_t, cell_get_char_t - gets value from 1-character string
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

// Gets cell value from spreadsheet as string
// N - Spreadsheet number
// R - Row number
// C - Colun number
// V - Pointer to string
// S - Maximum string length, the longer string value will be truncated
void cell_get_string(uchar N, uchar R, uchar C, char * V, ushort S)
{
 _uchar[mem_swap + 1] = R;
 _uchar[mem_swap + 2] = C;
 _uchar[mem_swap] = IOSpreadsheetCmd + 4 + N;
 string_get(V, S);
}

// Sets cell value to spreadsheet
// N - Spreadsheet number
// R - Row number
// C - Colun number
// cell_set_char_v, cell_set_char_v - sets value as number
// cell_set_char_t, cell_set_char_t - sets value as 1-character string
void cell_set_char_v( uchar N, uchar R, uchar C, char V)   { char V_ = V;   _uchar[mem_swap + 1] =  0; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; core_io.BufS1 = V_; _uchar[mem_swap] = IOSpreadsheetCmd + 8 + N; }
void cell_set_uchar_v(uchar N, uchar R, uchar C, uchar V)  { uchar V_ = V;  _uchar[mem_swap + 1] =  1; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; core_io.BufU1 = V_; _uchar[mem_swap] = IOSpreadsheetCmd + 8 + N; }
void cell_set_char_t( uchar N, uchar R, uchar C, char V)   { char V_ = V;   _uchar[mem_swap + 1] =  2; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; core_io.BufS1 = V_; _uchar[mem_swap] = IOSpreadsheetCmd + 8 + N; }
void cell_set_uchar_t(uchar N, uchar R, uchar C, uchar V)  { uchar V_ = V;  _uchar[mem_swap + 1] =  3; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; core_io.BufU1 = V_; _uchar[mem_swap] = IOSpreadsheetCmd + 8 + N; }
void cell_set_short(  uchar N, uchar R, uchar C, short V)  { short V_ = V;  _uchar[mem_swap + 1] =  4; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; core_io.BufS2 = V_; _uchar[mem_swap] = IOSpreadsheetCmd + 8 + N; }
void cell_set_ushort( uchar N, uchar R, uchar C, ushort V) { ushort V_ = V; _uchar[mem_swap + 1] =  5; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; core_io.BufU2 = V_; _uchar[mem_swap] = IOSpreadsheetCmd + 8 + N; }
void cell_set_int(    uchar N, uchar R, uchar C, int V)    { int V_ = V;    _uchar[mem_swap + 1] =  6; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; core_io.BufS2 = V_; _uchar[mem_swap] = IOSpreadsheetCmd + 8 + N; }
void cell_set_uint(   uchar N, uchar R, uchar C, uint V)   { uint V_ = V;   _uchar[mem_swap + 1] =  7; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; core_io.BufU2 = V_; _uchar[mem_swap] = IOSpreadsheetCmd + 8 + N; }
void cell_set_long(   uchar N, uchar R, uchar C, long V)   { long V_ = V;   _uchar[mem_swap + 1] =  8; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; core_io.BufS4 = V_; _uchar[mem_swap] = IOSpreadsheetCmd + 8 + N; }
void cell_set_ulong(  uchar N, uchar R, uchar C, ulong V)  { ulong V_ = V;  _uchar[mem_swap + 1] =  9; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; core_io.BufU4 = V_; _uchar[mem_swap] = IOSpreadsheetCmd + 8 + N; }
void cell_set_float(  uchar N, uchar R, uchar C, float V)  { float V_ = V;  _uchar[mem_swap + 1] = 10; _uchar[mem_swap + 2] = R; _uchar[mem_swap + 3] = C; core_io.BufF  = V_; _uchar[mem_swap] = IOSpreadsheetCmd + 8 + N; }

// Sets cell value to spreadsheet as string
// N - Spreadsheet number
// R - Row number
// C - Colun number
// V - Pointer to string
void cell_set_string(uchar N, uchar R, uchar C, char * V)
{
 string_set(V);
 _uchar[mem_swap + 1] = R;
 _uchar[mem_swap + 2] = C;
 _uchar[mem_swap] = IOSpreadsheetCmd + 12 + N;
}

// Clears spreadsheet
// N - Spreadsheet number
// R1 - First row
// C1 - First column
// R2 - Last row
// C2 - Last column
void sheet_clear(uchar N, uchar R1, uchar C1, uchar R2, uchar C2)
{
 _uchar[mem_swap + 1] = 0;
 _uchar[mem_swap + 2] = R1;
 _uchar[mem_swap + 3] = C1;
 _uchar[mem_swap + 4] = R2;
 _uchar[mem_swap + 5] = C2;
 _uchar[mem_swap] = IOSpreadsheetCmd + 16 + N;
}

// Clears whole spreadsheet
// N - Spreadsheet number
#define sheet_clear_whole(N) sheet_clear(N, 0, 0, 255, 255)

// Adds one row - moves values down
// N - Spreadsheet number
// R1 - First row
// C1 - First column
// R2 - Last row
// C2 - Last column
void sheet_rowadd(uchar N, uchar R1, uchar C1, uchar R2, uchar C2)
{
 _uchar[mem_swap + 1] = 1;
 _uchar[mem_swap + 2] = R1;
 _uchar[mem_swap + 3] = C1;
 _uchar[mem_swap + 4] = R2;
 _uchar[mem_swap + 5] = C2;
 _uchar[mem_swap] = IOSpreadsheetCmd + 16 + N;
}

// Removes one row - moves values up
// N - Spreadsheet number
// R1 - First row
// C1 - First column
// R2 - Last row
// C2 - Last column
void sheet_rowrem(uchar N, uchar R1, uchar C1, uchar R2, uchar C2)
{
 _uchar[mem_swap + 1] = 2;
 _uchar[mem_swap + 2] = R1;
 _uchar[mem_swap + 3] = C1;
 _uchar[mem_swap + 4] = R2;
 _uchar[mem_swap + 5] = C2;
 _uchar[mem_swap] = IOSpreadsheetCmd + 16 + N;
}

// Adds one column - moves values right
// N - Spreadsheet number
// R1 - First row
// C1 - First column
// R2 - Last row
// C2 - Last column
void sheet_coladd(uchar N, uchar R1, uchar C1, uchar R2, uchar C2)
{
 _uchar[mem_swap + 1] = 3;
 _uchar[mem_swap + 2] = R1;
 _uchar[mem_swap + 3] = C1;
 _uchar[mem_swap + 4] = R2;
 _uchar[mem_swap + 5] = C2;
 _uchar[mem_swap] = IOSpreadsheetCmd + 16 + N;
}

// Removes one column - moves values left
// N - Spreadsheet number
// R1 - First row
// C1 - First column
// R2 - Last row
// C2 - Last column
void sheet_colrem(uchar N, uchar R1, uchar C1, uchar R2, uchar C2)
{
 _uchar[mem_swap + 1] = 4;
 _uchar[mem_swap + 2] = R1;
 _uchar[mem_swap + 3] = C1;
 _uchar[mem_swap + 4] = R2;
 _uchar[mem_swap + 5] = C2;
 _uchar[mem_swap] = IOSpreadsheetCmd + 16 + N;
}
