#pragma once
#include "Data/DataSettings.h"
#include "Settings/SettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @defgroup Utils
 *  @{
 *  @defgroup GlobalFunctions Global Functions
 *  @{
 */

/// �������� ������ �� ���� ��� ������. ��������� ����� ������ ���� �� ds ��� set (���� ds == 0)
void *AllocMemForChannelFromHeap(Channel ch, DataSettings *ds);
/// ���������� ���������� ������, ��������� ��� ���������� ������ ������ ������
int RequestBytesForChannel(Channel ch, DataSettings *ds);
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
