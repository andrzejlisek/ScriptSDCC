// Opens file to read or write
// N - File number
// FileName - File name
// Returns: 0 - File not opened
//          1 - File opened
uchar inline file_open(uchar N, uchar * FileName)
{
 string_set(FileName);
 _uchar[mem_swap + 1] = N;
 _uchar[mem_swap + 2] = 3;
 _uchar[mem_swap] = 236;
 return _uchar[mem_swap + 1];
}

// Opens file to read only
// N - File number
// FileName - File name
// Returns: 0 - File not opened
//          1 - File opened
uchar inline file_open_read(uchar N, uchar * FileName)
{
 string_set(FileName);
 _uchar[mem_swap + 1] = N;
 _uchar[mem_swap + 2] = 2;
 _uchar[mem_swap] = 236;
 return _uchar[mem_swap + 1];
}

// Opens file to write only
// N - File number
// FileName - File name
// Returns: 0 - File not opened
//          1 - File opened
uchar inline file_open_write(uchar N, uchar * FileName)
{
 string_set(FileName);
 _uchar[mem_swap + 1] = N;
 _uchar[mem_swap + 2] = 1;
 _uchar[mem_swap] = 236;
 return _uchar[mem_swap + 1];
}

// Closes opened file
// N - File number
uchar inline file_close(uchar N)
{
 _uchar[mem_swap + 1] = N;
 _uchar[mem_swap] = 237;
 return _uchar[mem_swap + 1];
}

// Gets file size
// N - File number
ulong inline file_size(uchar N)
{
 _uchar[mem_swap + 1] = N;
 _uchar[mem_swap] = 238;
 return core_io.BufU4;
}

// Sets file pointer to specified position
// N - File number
// Pos - New file position
void inline file_setpos(uchar N, ulong Pos)
{
 core_io.BufU4 = Pos;
 _uchar[mem_swap + 1] = N;
 _uchar[mem_swap] = 239;
}

// Returns current position of file pointer
// N - File number
ulong inline file_getpos(uchar N)
{
 _uchar[mem_swap + 1] = N;
 _uchar[mem_swap] = 240;
 return core_io.BufU4;
}

// Copys file contents to buffer bemory
// N - File number
// BufN - Buffer memory number
// BufAddr - Address in buffer memory
// Size - Size of data
// Returns: 0 - Data not read
//          1 - Data read
uchar inline file_read(uchar N, uchar BufN, ushort BufAddr, ushort Size)
{
 _uchar[mem_swap + 1] = N;
 _uchar[mem_swap + 2] = BufN;
 _uchar[mem_swap + 3] = BufAddr >> 8;
 _uchar[mem_swap + 4] = BufAddr & 255;
 _uchar[mem_swap + 5] = Size >> 8;
 _uchar[mem_swap + 6] = Size & 255;
 _uchar[mem_swap] = 241;
 return _uchar[mem_swap + 1];
}

// Copys buffer bemory to file
// N - File number
// BufN - Buffer memory number
// BufAddr - Address in buffer memory
// Size - Size of data
// Returns: 0 - Data not wrote
//          1 - Data wrote
uchar inline file_write(uchar N, uchar BufN, ushort BufAddr, ushort Size)
{
 _uchar[mem_swap + 1] = N;
 _uchar[mem_swap + 2] = BufN;
 _uchar[mem_swap + 3] = BufAddr >> 8;
 _uchar[mem_swap + 4] = BufAddr & 255;
 _uchar[mem_swap + 5] = Size >> 8;
 _uchar[mem_swap + 6] = Size & 255;
 _uchar[mem_swap] = 242;
 return _uchar[mem_swap + 1];
}
