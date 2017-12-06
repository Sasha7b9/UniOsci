#pragma once
#include "defines.h"
#include "main.h"


class FDrive
{
public:
    void Init(void);

    bool Update(void);

    bool FileExist(char *fileName);

    int OpenFileForRead(char *fileName);
    /// Считывает из открытого файла numBytes байт. Возвращает число реально считанных байт
    int ReadFromFile(int numBytes, uint8 *buffer);

    void CloseOpenedFile(void);
};

extern FDrive drive;
