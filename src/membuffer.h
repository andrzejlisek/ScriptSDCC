#ifndef MEMBUFFER_H
#define MEMBUFFER_H

#include "eden.h"
#include <iostream>

class MemBuffer
{
public:
    MemBuffer();
    ~MemBuffer();
    bool Get(int No, int OffsetBuf, int OffsetExt, int Size, uchar *RAW, uchar *RAWX);
    bool Set(int No, int OffsetBuf, int OffsetExt, int Size, uchar *RAW, uchar *RAWX);
    bool Copy(int No1, int No2, int Offset1, int Offset2, int Size);
    void PrepareBuf(int No);
    void Clear(int No);
    uchar * BufRaw[256];
    uchar * BufOpR[256];
    uchar * BufOpW[256];
};

#endif // MEMBUFFER_H
