#include "membuffer.h"

MemBuffer::MemBuffer()
{
    for (int I = 0; I < 256; I++)
    {
        BufRaw[I] = NULL;
        BufOpR[I] = NULL;
        BufOpW[I] = NULL;
    }
}

MemBuffer::~MemBuffer()
{
    for (int I = 0; I < 256; I++)
    {
        if (BufRaw[I] != NULL)
        {
            DELARR(uchar, BufRaw[I]);
            DELARR(uchar, BufOpR[I]);
            DELARR(uchar, BufOpW[I]);
        }
    }
}

void MemBuffer::PrepareBuf(int No)
{
    if (BufRaw[No] == NULL)
    {
        BufRaw[No] = NEW(uchar, uchar[65536]);
        BufOpR[No] = NEW(uchar, uchar[65536]);
        BufOpW[No] = NEW(uchar, uchar[65536]);
        for (int I = 0; I < 65536; I++)
        {
            BufRaw[No][I] = 0;
            BufOpR[No][I] = 0;
            BufOpW[No][I] = 0;
        }
    }
}

void MemBuffer::Clear(int No)
{
    if (BufRaw[No] != NULL)
    {
        DELARR(uchar, BufRaw[No]);
        DELARR(uchar, BufOpR[No]);
        DELARR(uchar, BufOpW[No]);
        BufRaw[No] = NULL;
        BufOpR[No] = NULL;
        BufOpW[No] = NULL;
    }
}

bool MemBuffer::Get(int No, int OffsetBuf, int OffsetExt, int Size, uchar *RAW, uchar *RAWX)
{
    if ((OffsetBuf < 0) || (OffsetExt < 0) || ((OffsetBuf + Size) >= 65536) || ((OffsetBuf + Size) >= 65536))
    {
        return false;
    }
    PrepareBuf(No);
    int IBuf = OffsetBuf;
    int IExt = OffsetExt;
    for (int I = 0; I < Size; I++)
    {
        RAW[IExt] = BufRaw[No][IBuf];
        BufOpR[No][IBuf] = 1;
        RAWX[IExt] = 1;
        IBuf++;
        IExt++;
    }
    return true;
}

bool MemBuffer::Set(int No, int OffsetBuf, int OffsetExt, int Size, uchar *RAW, uchar *RAWX)
{
    if ((OffsetBuf < 0) || (OffsetExt < 0) || ((OffsetBuf + Size) >= 65536) || ((OffsetBuf + Size) >= 65536))
    {
        return false;
    }
    PrepareBuf(No);
    int IBuf = OffsetBuf;
    int IExt = OffsetExt;
    for (int I = 0; I < Size; I++)
    {
        BufRaw[No][IBuf] = RAW[IExt];
        BufOpW[No][IBuf] = 1;
        RAWX[IExt] = 1;
        IBuf++;
        IExt++;
    }
    return true;
}

bool MemBuffer::Copy(int No1, int No2, int Offset1, int Offset2, int Size)
{
    if ((Offset1 < 0) || (Offset2 < 0) || ((Offset1 + Size) >= 65536) || ((Offset2 + Size) >= 65536))
    {
        return false;
    }
    PrepareBuf(No1);
    PrepareBuf(No2);
    int I1 = Offset1;
    int I2 = Offset2;
    for (int I = 0; I < Size; I++)
    {
        BufRaw[No2][I2] = BufRaw[No1][I1];
        BufOpR[No1][I1] = 1;
        BufOpW[No2][I2] = 1;
        I1++;
        I2++;
    }
    return true;
}
