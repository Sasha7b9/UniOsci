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
/// ���������� ����������� ����������, ��������������� ��� �������� ���������
char GetSymbolForGovernor(int value);
/// ��������� ������ ����� ���������
void FillArrayUINT8(uint8 *data, uint8 value, int numElements);
/// ����� ������ ������� �������, �� �������������� ���������
int FindAnotherElement(uint8 *data, uint8 value, int numElements);

/** @}  @}
 */
