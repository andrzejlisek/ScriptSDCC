ViewCommand=
FileNameSource=Chart.c
FileNameBinary=Chart.ihx
Engine=1
Memory=0
Command=-mz180 --code-loc 0x%CODELOC%00 --code-size 0x%CODESIZE%00 --data-loc 0x%DATALOC%00
MemCodeLoc=03
MemCodeSize=7D
MemDataLoc=80
MemDataSize=80
MemSwapLoc=02
LibraryFiles=core.h|spreadsheet.h|graph.h|mathx.h
Description="Plots any chart using formula:\n(Axxx+Bxxy+Cxyy+Dyyy+Exx+Fxy+Gyy+Hx+Iy+J)/(Axxx+Bxxy+Cxyy+Dyyy+Exx+Fxy+Gyy+Hx+Iy+J)=0\nAter running, the constat values (from A to J in formula), x and y ranges and steps, torerance in equation and plot colors must be defined in Spreadsheet 0. After continuing, the chart will be plotted on Graph 0.\nOn Spreadsheet 1 there will be printed sample formula definitions fitting default steps and ranges."
