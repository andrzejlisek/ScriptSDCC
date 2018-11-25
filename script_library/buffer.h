// Clears buffer
// N - Number of buffer
void inline buffer_clear(uchar N)
{
 _uchar[mem_swap + 1] = N;
 _uchar[mem_swap] = 243;
}

// Copys data from buffer to memory
// N - Number of buffer
// BufAddr - Address in buffer
// ProgAddr - Address in program data memory
// Size - Number of byte
// Returns: 1 if adresses and size are valid, otherwise 0 (in this case data is not copied)
uchar buffer_get(uchar N, ushort BufAddr, ushort ProgAddr, ushort Size)
{
 core_io.BufU2 = BufAddr;
 core_value_copy(mem_swap + 0x08, mem_swap + 0x10);
 core_io.BufU2 = ProgAddr;
 core_value_copy(mem_swap + 0x08, mem_swap + 0x18);
 core_io.BufU2 = Size;
 core_value_copy(mem_swap + 0x08, mem_swap + 0x20);
 _uchar[mem_swap + 1] = N;
 _uchar[mem_swap] = 244;
 return _uchar[mem_swap + 1];
}

// Copys data from memory to buffer
// N - Number of buffer
// BufAddr - Address in buffer
// ProgAddr - Address in program data memory
// Size - Number of byte
// Returns: 1 if adresses and size are valid, otherwise 0 (in this case data is not copied)
uchar buffer_set(uchar N, ushort BufAddr, ushort ProgAddr, ushort Size)
{
 core_io.BufU2 = BufAddr;
 core_value_copy(mem_swap + 0x08, mem_swap + 0x10);
 core_io.BufU2 = ProgAddr;
 core_value_copy(mem_swap + 0x08, mem_swap + 0x18);
 core_io.BufU2 = Size;
 core_value_copy(mem_swap + 0x08, mem_swap + 0x20);
 _uchar[mem_swap + 1] = N;
 _uchar[mem_swap] = 245;
 return _uchar[mem_swap + 1];
}
