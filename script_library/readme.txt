These heares files are the ScriptSDCC API, which is used to write script in C language and compile it using SDCC external software.

The detailed description are in comments in the header files.

core.h - the mandatory file, which defines the standard types, macros and methods
 uchar - "unsigned char" type
 schar - "signed char" type
 uint - "unsigned int" type
 sint - "signed int" type
 ulong - "unsigned long" type
 slong - "signed long" type
 ushort - "unsigned short" type
 sshort - "signed short" type
 llong - "long long" type
 ullong - "unsigned long long" type
 sllong - "signed long long" type
 ulong prgcounter(uchar div) - Returns number of instructions from running or last prgcounter invocation
 void stop() - Stops script and allows user to continue by clicking "Run" button
 void end() - Stops script without continuation possibility
 void string_set(char * V) - Sets specified string to string buffer
 void string_get(char * V, ushort S) - Gets string buffer contents to specified string

console.h - Console input/output
 void console_clear(uchar N) - Clears console
 void console_print_schar_v(uchar N, schar V) - Prints schar value as number
 void console_print_uchar_v(uchar N, uchar V) - Prints schar value as number
 void console_print_schar_t(uchar N, schar V) - Prints uchar value as 1-character string
 void console_print_uchar_t(uchar N, uchar V) - Prints uchar value as 1-character string
 void console_print_sshort(uchar N, sshort V) - Prints sshort value
 void console_print_ushort(uchar N, ushort V) - Prints ushort value
 void console_print_sint(uchar N, sint V) - Prints sint value
 void console_print_uint(uchar N, uint V) - Prints uint value
 void console_print_slong(uchar N, slong V) - Prints slong value
 void console_print_ulong(uchar N, ulong V) - Prints ulong value
 void console_print_float(uchar N, float V) - Prints float value
 void console_print_string(uchar N, char * V) - Prints specified string
 void inline console_print_line(uchar N) - Prints end-of-line character
 void inline console_print_float_format(uchar N, char V) - Sets float number format
 schar console_input_schar(uchar N) - Gets one character from console input buffer as signed value
 uchar console_input_uchar(uchar N) - Gets one character from console input buffer as unsigned value
 void inline console_input_flush(uchar N) - Clears console input buffer

spreadsheet.h - Spreadsheet input/output
 schar cell_get_schar_v(uchar N, uchar R, uchar C) - Gets cell number value from spreadsheet
 uchar cell_get_uchar_v(uchar N, uchar R, uchar C) - Gets cell number value from spreadsheet
 schar cell_get_schar_t(uchar N, uchar R, uchar C) - Gets cell character value from spreadsheet
 uchar cell_get_uchar_t(uchar N, uchar R, uchar C) - Gets cell character value from spreadsheet
 sshort cell_get_sshort(uchar N, uchar R, uchar C) - Gets cell value from spreadsheet
 ushort cell_get_ushort(uchar N, uchar R, uchar C) - Gets cell value from spreadsheet
 sint cell_get_sint(uchar N, uchar R, uchar C) - Gets cell value from spreadsheet
 uint cell_get_uint(uchar N, uchar R, uchar C) - Gets cell value from spreadsheet
 slong cell_get_slong(uchar N, uchar R, uchar C) - Gets cell value from spreadsheet
 ulong cell_get_ulong(uchar N, uchar R, uchar C) - Gets cell value from spreadsheet
 float cell_get_float(uchar N, uchar R, uchar C) - Gets cell value from spreadsheet
 void cell_get_string(uchar N, uchar R, uchar C, char * V, ushort S) - Gets cell value from spreadsheet as string
 void cell_set_schar_v(uchar N, uchar R, uchar C, schar V) - Sets cell value to spreadsheet as number
 void cell_set_uchar_v(uchar N, uchar R, uchar C, uchar V) - Sets cell value to spreadsheet as number
 void cell_set_schar_t(uchar N, uchar R, uchar C, schar V) - Sets cell value to spreadsheet as 1-character string
 void cell_set_uchar_t(uchar N, uchar R, uchar C, uchar V) - Sets cell value to spreadsheet as 1-character string
 void cell_set_sshort( uchar N, uchar R, uchar C, sshort V) - Sets cell value to spreadsheet
 void cell_set_ushort( uchar N, uchar R, uchar C, ushort V) - Sets cell value to spreadsheet
 void cell_set_sint(uchar N, uchar R, uchar C, sint V) - Sets cell value to spreadsheet
 void cell_set_uint(uchar N, uchar R, uchar C, uint V) - Sets cell value to spreadsheet
 void cell_set_slong(uchar N, uchar R, uchar C, slong V) - Sets cell value to spreadsheet
 void cell_set_ulong(uchar N, uchar R, uchar C, ulong V) - Sets cell value to spreadsheet
 void cell_set_float(uchar N, uchar R, uchar C, float V) - Sets cell value to spreadsheet
 void cell_set_string(uchar N, uchar R, uchar C, char * V) - Sets cell value to spreadsheet as string
 void sheet_clear(uchar N, uchar R1, uchar C1, uchar R2, uchar C2) - Clears spreadsheet
 void sheet_clear_whole(N) - Clears whole spreadsheet
 sheet_rowadd(uchar N, uchar R1, uchar C1, uchar R2, uchar C2) - Adds one row - moves values down
 sheet_rowrem(uchar N, uchar R1, uchar C1, uchar R2, uchar C2) - Removes one row - moves values up
 sheet_coladd(uchar N, uchar R1, uchar C1, uchar R2, uchar C2) - Adds one column - moves values right
 sheet_colrem(uchar N, uchar R1, uchar C1, uchar R2, uchar C2) - Removes one column - moves values left

graph.h - Graphics drawing
 void graph_clear(uchar N) - Clears whole graphics
 void graph_thread(uchar N, uchar T) - Sets polyline thread
 void graph_thread_reset(uchar N) // Resets current polyline thread
 void graph_plot_schar(uchar N, schar X, schar Y, schar Z, uchar R, uchar G, uchar B) - Sets points on screen
 void graph_plot_uchar(uchar N, uchar X, uchar Y, uchar Z, uchar R, uchar G, uchar B) - Sets points on screen
 void graph_plot_sshort(uchar N, sshort X, sshort Y, sshort Z, uchar R, uchar G, uchar B) - Sets points on screen
 void graph_plot_ushort(uchar N, ushort X, ushort Y, ushort Z, uchar R, uchar G, uchar B) - Sets points on screen
 void graph_plot_sint(uchar N, sint X, sint Y, sint Z, uchar R, uchar G, uchar B) - Sets points on screen
 void graph_plot_uint(uchar N, uint X, uint Y, uint Z, uchar R, uchar G, uchar B) - Sets points on screen
 void graph_plot_slong(uchar N, slong X, slong Y, slong Z, uchar R, uchar G, uchar B) - Sets points on screen
 void graph_plot_ulong(uchar N, ulong X, ulong Y, ulong Z, uchar R, uchar G, uchar B) - Sets points on screen
 void graph_plot_float(uchar N, float X, float Y, float Z, uchar R, uchar G, uchar B) - Sets points on screen
 void graph_triangle(uchar N) - Converts last three points to triangle
 void graph_line(uchar N, uchar L) - Converts last points to polyline

graphtext.h - Graph text labels
 void graph_text_color(uchar N, uchar DispBackFore, uchar ColorBackR, uchar ColorBackG, uchar ColorBackB, uchar ColorForeR, uchar ColorForeG, uchar ColorForeB) - Sets text foreground and background color
 void graph_text_color_f(N, R, G, B) - Sets text foreground color with transparent background
 void graph_text_color_b(N, R, G, B) - Sets text background color with transparent foreground 
 void graph_text_xy_schar(uchar N, schar X, schar Y, schar Z, short X0, short Y0) - Sets text paint coordinates
 void graph_text_xy_uchar(uchar N, uchar X, uchar Y, uchar Z, short X0, short Y0) - Sets text paint coordinates
 void graph_text_xy_sshort(uchar N, sshort X, sshort Y, sshort Z, short X0, short Y0) - Sets text paint coordinates
 void graph_text_xy_ushort(uchar N, ushort X, ushort Y, ushort Z, short X0, short Y0) - Sets text paint coordinates
 void graph_text_xy_sint(uchar N, sint X, sint Y, sint Z, short X0, short Y0) - Sets text paint coordinates
 void graph_text_xy_uint(uchar N, uint X, uint Y, uint Z, short X0, short Y0) - Sets text paint coordinates
 void graph_text_xy_slong(uchar N, slong X, slong Y, slong Z, short X0, short Y0) - Sets text paint coordinates
 void graph_text_xy_ulong(uchar N, ulong X, ulong Y, ulong Z, short X0, short Y0) - Sets text paint coordinates
 void graph_text_xy_float(uchar N, float X, float Y, float Z, short X0, short Y0) - Sets text paint coordinates
 void graph_text_schar_v(uchar N, schar V) - Paints schar value as number
 void graph_text_uchar_v(uchar N, uchar V) - Paints uchar value as number
 void graph_text_schar_t(uchar N, schar V) - Paints schar value as 1-character string
 void graph_text_uchar_t(uchar N, uchar V) - Paints uchar value as 1-character string
 void graph_text_sshort(uchar N, sshort V) - Paints sshort value
 void graph_text_ushort(uchar N, ushort V) - Paints ushort value
 void graph_text_sint(uchar N, sint V) - Paints sint value
 void graph_text_uint(uchar N, uint V) - Paints uint value
 void graph_text_slong(uchar N, slong V) - Paints slong value
 void graph_text_ulong(uchar N, ulong V) - Paints ulong value
 void graph_text_float(uchar N, float V) - Paints float value
 void graph_text_string(uchar N, char * V) - Paints specified string

bundle.h - Controling bundle items (running and checking status)
 uchar bundle_status(uchar N) - Gets status of other script from bundle
 uchar bundle_run(uchar N) - Runs other script from bundle

buffer.h - The additional 256 pages of 64kB memory for interchange data between bundle items and file input/output
 void inline buffer_clear(uchar N) - Clears buffer
 uchar buffer_get(uchar N, ushort BufAddr, ushort ProgAddr, ushort Size) - Copys data from buffer to memory
 uchar buffer_set(uchar N, ushort BufAddr, ushort ProgAddr, ushort Size) - Copys data from memory to buffer

file.h - Disk file input/output
 uchar inline file_open(uchar N, uchar * FileName) - Opens file to read or write
 uchar inline file_open_read(uchar N, uchar * FileName) - Opens file to read only
 uchar inline file_open_write(uchar N, uchar * FileName) - Opens file to write only
 uchar inline file_close(uchar N) - Closes opened file
 ulong inline file_size(uchar N) - Gets file size
 void inline file_setpos(uchar N, ulong Pos) - Sets file pointer to specified position
 ulong inline file_getpos(uchar N) - Returns current position of file pointer
 uchar inline file_read(uchar N, uchar BufN, ushort BufAddr, ushort Size) - Copys file contents to buffer bemory
 uchar inline file_write(uchar N, uchar BufN, ushort BufAddr, ushort Size) - Copys buffer bemory to file
 
mathx.h - Basic mathematics constants and functions
 M_E_ - Euler constant
 M_PI_ - Pi constant
 angle_deg_rad - Pi/180 constant
 angle_rad_deg - 180/Pi constant
 abs_(X) - absolute number macro
 equal_(A, B, T) - check if A equals to B with T tolerance macro
 roundf_ - round float type number to integer number
