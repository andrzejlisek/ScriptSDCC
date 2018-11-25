#ifndef FILEHANDLE_H
#define FILEHANDLE_H

#include "eden.h"
#include <iostream>
#include <mutex>

class FileHandle
{
public:
    FileHandle();
    ~FileHandle();
    bool FileOpen(uchar N, string FileName, char AccessMode);
    bool FileClose(uchar N);
    uint FileSize(uchar N);
    bool FileSetPos(uchar N, uint Pos);
    uint FileGetPos(uchar N);
    bool DataPut(uchar N, uchar * DataBuf, uchar * DataBufX, uint DataAddr, uint DataSize);
    bool DataGet(uchar N, uchar * DataBuf, uchar * DataBufX, uint DataAddr, uint DataSize);
    string Info(uchar N);
private:
    mutex FileMutex;
    fstream * F[256];
    char FA[256];
    char * TempBuf;
    char * TempOpX;
    string FN[256];
};

#endif // FILEHANDLE_H
