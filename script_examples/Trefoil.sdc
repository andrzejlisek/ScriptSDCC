ViewCommand=start F:\__ToBackup\Develop\QT\ScriptSDCC\Progs\Trefoil.c
FileNameSource=Trefoil.c
FileNameBinary=Trefoil.ihx
Engine=0
Memory=1
Command=-mmcs51 --model-large --code-loc 0x%CODELOC%00 --code-size 0x%CODESIZE%00 --xram-loc 0x%DATALOC%00 --xram-size 0x%DATASIZE%00
MemCodeLoc=03
MemCodeSize=7D
MemDataLoc=80
MemDataSize=80
MemSwapLoc=02
LibraryFiles=core.h|graph.h|mathx.h
Description="Plots sample trefoil knots on Graph 0 and Graph 1.\nKnot on Graph 0:\nX = sin(T) + 2.0 * sin(T*2)\nY = cos(T) - 2.0 * cos(T*2)\nZ = 0.0 - sin(T*3)\nT = <0\# 2*pi>\n\nKnot on Graph 1:\nX = (2.0 + cos(T*3)) * sin(T*2)\nY = (2.0 + cos(T*3)) * cos(T*2) * (-1)\nZ = sin(T*3)\nT = <0\# 2*pi>\n"
