#pragma once
#include "Data/DataSettings.h"
#include "Settings/SettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @defgroup Utils
 *  @{
 *  @defgroup GlobalFunctions Global Functions
 *  @{
 */

class Translate
{
public:
    /// @brief ����������� value � ��������� ������. ��� ���� ��������� ��� 8 ��������
    /// @attention ������ ����� ��������� �� ���������� ������ �������. ���� ��������� ����� ������� ���������� �������, �� ��� ����� ����������� ����
    char* Bin2String(uint8 value, char bufferOut[9]);
    /// @brief ����������� value � ��������� ������
    /// @attention ������ ����� ��������� �� ���������� ������ �������. ���� ��������� ����� ������� ���������� �������, �� ��� ����� ����������� ����
    char* Bin2String16(uint16 value, char bufferOut[19]);
    /// @brief ����������� value � ��������� ������ � ����������������� ����
    /// @attention ������ ����� ��������� �� ���������� ������ �������. ���� ��������� ����� ������� ���������� �������, �� ��� ����� ����������� ����
    char* Hex8toString(uint8 value, char bufferOut[3], bool upper);

    char* Hex16toString(uint16 value, char bufferOut[5], bool upper);

    char* Hex32toString(uint32 value, char bufferOut[9], bool upper);
    /// @brief ����������� value � ��������� ������
    /// @attention ������ ����� ��������� �� ���������� ������ �������. ���� ��������� ����� ������� ���������� �������, �� ��� ����� ����������� ����
    char* Int2String(int value,                     ///< ��������
                     bool alwaysSign,               ///< ���� ���������� � true, ���� ����� ���������� ������
                     int numMinFields,              ///< ����������� ����� ���������� ���������. ���� ��� ������ ����� ������� �� ���������, ������ ����������� ������
                     char bufferOut[20]             ///< ���� ������������ ������������ ��������
    );

    bool String2Int(char *str, int *value);
        /// @brief ����������� time ������ � ��������� ������
    char* Time2StringAccuracy(float time,           ///< ����� � ��������. ����� ���� �������������
                              bool alwaysSign,      ///< ���� true, ���� ������������ ���� ���� time > 0
                              char bufferOut[20],   ///< ���� ������������ ������������ ��������
                              int numDigits         ///< ���������� ���� � ������
    );
    /// ����������� freq ���� � ��������� ������. ��� ���� ����� ���������� ���� � ������ ����� numDigits
    char* Freq2StringAccuracy(float freq, char bufferOut[20], int numDigits);
    /// ����������� �������� � ��������� ������
    char* Float2Db(float value, int numDigits, char bufferOut[20]);

    int BCD2Int(uint bcd);
};


extern Translate trans;

/// ���������� true, ���� value ������ � �������� [min; max]
bool IntInRange(int value,                      ///< ����������� ��������
                int min,                        ///< ������ ������� ���������
                int max                         ///< ������� ������� ���������
                );
/// �������� ������ �� ���� ��� ������. ��������� ����� ������ ���� �� ds ��� set (���� ds == 0)
void *AllocMemForChannelFromHeap(Channel ch, DataSettings *ds);
/// ���������� ���������� ������, ��������� ��� ���������� ������ ������ ������
int RequestBytesForChannel(Channel ch, DataSettings *ds);
void BubbleSortINT(int array[], int num);
/// ���������� ������������ �������� �� ���
float MaxFloat(float val1, float val2, float val3);
/// ����������� �������� �� ������ val �� delta. �����, ���� ��������� ������ max, ������������ ��� min
float CircleAddFloat(float *val, float delta, float min, float max);
/// ��������� �������� �� ������ val �� delta. �����, ���� ��������� ������ min, ������������ ��� masx
float CircleSubFloat(float *val, float delta, float min, float max);
/// ��������� � �������� �� ������ val delta. ���� ��������� �� ����������� � �������� [min; max], ����������
void AddLimitationFloat(float *val,             ///< �� ����� ������ �������� ��������.
                        float delta,            ///< � ��� ������.
                        float min,              ///< ������ ������� ���������.
                        float max               ///< ������� ������� ���������.
                        );
/// ���������� ������� ���������� ������ �� ������� value0 � value1
void SwapInt(int *value0, int *value1);
/// ��������� ���������� value0 � value1 � ������� �����������
void SortInt(int *value0, int *value1);
/// ���������� ����������� ����������, ��������������� ��� �������� ���������
char GetSymbolForGovernor(int value);
/// �������-��������. ������ �� ������
void EmptyFuncVV(void);
/// �������-��������. ������ �� ������
void EmptyFuncVpV(void *);
/// �������-��������. ������ �� ������
void EmptyFuncVII(int, int);
void EmptyFuncVI(int);
bool EmptyFuncBV();
void EmptyFuncVB(bool);
/// �������-��������. ������ �� ������
void EmptyFuncpVII(void *, int, int);
/// ��������� ������ ����� ���������
void FillArrayUINT8(uint8 *data, uint8 value, int numElements);
/// ����� ������ ������� �������, �� �������������� ���������
int FindAnotherElement(uint8 *data, uint8 value, int numElements);

char* IntToStrCat(char *_buffer, int _value);
/// ������� numElements ��������� ������� array � ����� ������ �������
void LoggingArrayUINT8(char *label, uint8 *array, int numElements);


/** @}  @}
 */
