#pragma once
#include "Menu/MenuItems.h"
#include "Panel/Controls.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Menu
 *  @{
 *  @defgroup MenuFunctions Menu Functions
 *  @brief ��������������� ������� ��� ������ � ����
 *  @{
 */

/// ���������� true, ���� ������� ������� �������� � ������ namePage ������
bool CurrentItemIsOpened(NamePage namePage);

bool ChangeOpenedItem(Control *item, int delta);
/// ���������� �������� �������� �� ������ item, ��� ��� �������� �� ������� �������
const char *TitleItem(void *item);

void CallFuncOnPressButton(void *button);

/** @}
 */
