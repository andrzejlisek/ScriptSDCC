#include "core.h"
#include "spreadsheet.h"
#include "console.h"
#include "buffer.h"
#include "file.h"


void main()
{
 uint FileSize;
 char Buf[1025];
 uint I;

 console_clear(0);
 print_string(0, "Input file names in Spreadsheet 0 and continue script");
 print_line(0);

 sheet_clear_whole(1);
 cell_set_string(0, 0, 0, "Input file:");
 cell_set_string(0, 1, 0, "Output file:");
 cell_set_string(0, 0, 1, "in.txt");
 cell_set_string(0, 1, 1, "out.txt");
 stop();
 cell_get_string(0, 0, 1, Buf, 1024);
 print_string(0, "Input file: ");
 print_string(0, Buf);
 print_line(0);
 if (file_open_read(0, Buf))
 {
  FileSize = file_size(0);
  print_string(0, "File loaded");
  print_line(0);
  print_string(0, "File size: ");
  print_ulong(0, FileSize);
  print_line(0);
  if (FileSize > 1024)
  {
   FileSize = 1024;
  }
  file_read(0, 0, 0, FileSize);
  buffer_get(0, 0, (ushort)Buf, FileSize);
  Buf[FileSize] = 0;
  file_close(0);
  for (I = 0; I < FileSize; I++)
  {
   if ((Buf[I] >= 65) && (Buf[I] <= 90))
   {
    Buf[I] += 32;
   }
   else
   {
    if ((Buf[I] >= 97) && (Buf[I] <= 122))
    {
     Buf[I] -= 32;
    }
   }
  }
  buffer_set(1, 0, (ushort)Buf, FileSize);
  cell_get_string(0, 1, 1, Buf, 1024);
  print_string(0, "Output file: ");
  print_string(0, Buf);
  print_line(0);
  if (file_open_write(1, Buf))
  {
   file_write(1, 1, 0, FileSize);
   file_close(1);
   print_string(0, "File saved");
   print_line(0);
  }
  else
  {
   print_string(0, "Error in file saving");
   print_line(0);
  }
 }
 else
 {
  print_string(0, "Error in file loading");
  print_line(0);
 }
 end();
}
