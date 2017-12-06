#pragma once
#include <ff.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @defgroup FlashDrive
 *  @brief Работа с флешкой
 *  @{
 */

#define SIZE_FLASH_TEMP_BUFFER 512
typedef struct
{
    uint8   tempBuffer[SIZE_FLASH_TEMP_BUFFER];
    int     sizeData;
    FIL     fileObj;
    char    name[255];
} StructForWrite;

typedef struct
{
    char nameDir[_MAX_LFN + 1];
    FILINFO fno;
    DIR dir;
} StructForReadDir;

class FDrive
{
public:
    void Init(void);
    
    void Update(void);
    
    void Mount(void);
    
    bool IsConnected(void);
    
    void GetNumDirsAndFiles(const char *fullPath, int *numDirs, int *numFiles);
    
    bool GetNameDir(const char *fuulPath, int numDir, char *nameDirOut, StructForReadDir *sfrd);
    
    bool GetNextNameDir(char *nameDirOut, StructForReadDir *sfrd);
    
    void CloseCurrentDir(StructForReadDir *sfrd);
    
    bool GetNameFile(const char *fullPath, int numFile, char *nameFileOut, StructForReadDir *sfrd);
    
    bool GetNextNameFile(char *nameFileOut, StructForReadDir *sfrd);
    
    bool OpenNewFileForWrite(const char *fullPathToFile, StructForWrite *structForWrite); // Функция создаст файл для записи. Если такой файл уже существует, сотрёт его, заменит новым нулевой длины и откроет его
    
    bool WriteToFile(uint8 *data, int sizeData, StructForWrite *structForWrite);
    
    bool CloseFile(StructForWrite *structForWrite);
    
    bool AppendStringToFile(const char *string);
};

extern FDrive drive;

#define FDRIVE_IS_CONNECTED (drive.IsConnected())

/** @}
 */
