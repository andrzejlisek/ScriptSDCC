#include "filehandle.h"

FileHandle::FileHandle()
{
    TempBuf = NEWARR(char, char[65536]);
    TempOpX = NEWARR(char, char[65536]);
    for (int I = 0; I < 256; I++)
    {
        F[I] = NULL;
    }
    for (int I = 0; I < 65536; I++)
    {
        TempOpX[I] = 1;
    }
}

FileHandle::~FileHandle()
{
    for (int I = 0; I < 256; I++)
    {
        FileClose(I);
    }
    DELARR(char, TempBuf);
    DELARR(char, TempOpX);
}

bool FileHandle::FileOpen(uchar N, string FileName, char AccessMode)
{
    FileClose(N);
    FileMutex.lock();
    FN[N] = FileName;
    FA[N] = AccessMode;
    switch (AccessMode)
    {
        case 1: F[N] = NEW(fstream, fstream(FileName, ios::binary | ios::out)); break;
        case 2: F[N] = NEW(fstream, fstream(FileName, ios::binary | ios::in));  break;
        case 3: F[N] = NEW(fstream, fstream(FileName, ios::binary | ios::in | ios::out));  break;
    }
    if (F[N]->is_open())
    {
        FileMutex.unlock();
        return true;
    }
    else
    {
        DEL(fstream, F[N]);
        F[N] = NULL;
        FileMutex.unlock();
        return false;
    }
}

bool FileHandle::FileClose(uchar N)
{
    FileMutex.lock();
    if (F[N] != NULL)
    {
        F[N]->close();
        DEL(fstream, F[N]);
        F[N] = NULL;
        FileMutex.unlock();
        return true;
    }
    else
    {
        FileMutex.unlock();
        return false;
    }
}

uint FileHandle::FileSize(uchar N)
{
    FileMutex.lock();
    if (F[N] != NULL)
    {
        uint X = 0;
        uint S = 0;
        switch (FA[N])
        {
            case 1:
                X = F[N]->tellp();
                F[N]->seekp(0, ios_base::end);
                S = F[N]->tellp();
                F[N]->seekp(X);
            break;
            case 2:
            case 3:
                X = F[N]->tellg();
                F[N]->seekg(0, ios_base::end);
                S = F[N]->tellg();
                F[N]->seekg(X);
            break;
        }
        FileMutex.unlock();
        return S;
    }
    FileMutex.unlock();
    return 0;
}

bool FileHandle::FileSetPos(uchar N, uint Pos)
{
    FileMutex.lock();
    if (F[N] != NULL)
    {
        if ((FA[N] == 1) || (FA[N] == 3))
        {
            F[N]->seekp(Pos);
        }
        if ((FA[N] == 2) || (FA[N] == 3))
        {
            F[N]->seekg(Pos);
        }
        FileMutex.unlock();
        return true;
    }
    FileMutex.unlock();
    return false;
}

uint FileHandle::FileGetPos(uchar N)
{
    uint S = 0;
    FileMutex.lock();
    if (F[N] != NULL)
    {
        if ((FA[N] == 2) || (FA[N] == 3))
        {
            S = F[N]->tellg();
        }
        else
        {
            S = F[N]->tellp();
        }
        FileMutex.unlock();
        return true;
    }
    FileMutex.unlock();
    return S;
}

bool FileHandle::DataPut(uchar N, uchar * DataBuf, uchar * DataBufX, uint DataAddr, uint DataSize)
{
    FileMutex.lock();
    if ((DataAddr + DataSize <= 65536) && (F[N] != NULL))
    {
        memcpy(DataBufX + DataAddr, TempOpX, DataSize);
        memcpy(TempBuf, DataBuf + DataAddr, DataSize);
        F[N]->write(TempBuf, DataSize);
        if (FA[N] == 3)
        {
            F[N]->seekg(F[N]->tellp());
        }
        FileMutex.unlock();
        return true;
    }
    FileMutex.unlock();
    return false;
}

bool FileHandle::DataGet(uchar N, uchar * DataBuf, uchar * DataBufX, uint DataAddr, uint DataSize)
{
    FileMutex.lock();
    if ((DataAddr + DataSize <= 65536) && (F[N] != NULL))
    {
        F[N]->read(TempBuf, DataSize);
        memcpy(DataBufX + DataAddr, TempOpX, DataSize);
        memcpy(DataBuf + DataAddr, TempBuf, DataSize);
        if (FA[N] == 3)
        {
            F[N]->seekp(F[N]->tellg());
        }
        FileMutex.unlock();
        return true;
    }
    FileMutex.unlock();
    return false;
}

string FileHandle::Info(uchar N)
{
    FileMutex.lock();
    if (F[N] != NULL)
    {
        string Info = " " + FN[256];
        switch (FA[N])
        {
            case 1: Info = FN[256] + " [W]"; break;
            case 2: Info = FN[256] + " [R]"; break;
            case 3: Info = FN[256] + " [R&W]"; break;
        }
        FileMutex.unlock();
        return Info;
    }
    else
    {
        FileMutex.unlock();
        return "NULL";
    }
}
