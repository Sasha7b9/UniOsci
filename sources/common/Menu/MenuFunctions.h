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
/// �������/��������� ������� ����� ��������
void SetCurrentItem(const void *item,               ///< ����� ��������, ���������� �������� ����� ����������/�����
                    bool active                     ///< true - ������� ������� �������, false - ��������� ������� �������. � ���� ������ ������� ���������� ��������� ��������
                    );
bool ChangeOpenedItem(Control *item, int delta);
/// ���������� ������ � �������� ��������� �������� Choice ��� NamePage �� ������ item
int HeightOpenedItem(Control *item);
/// ���������� �������� �������� �� ������ item, ��� ��� �������� �� ������� �������
const char *TitleItem(void *item);
/// ���������� true, ���� button - �������������� ������� [1..5]
bool IsFunctionalButton(PanelButton button);
/// �������/������� ������� ���� �� ������ item
void OpenItem(const void *item, bool open);
/// ���������� true, ���� ������� ���� �� ������ item ������
bool ItemIsOpened(const Control *item);

bool IsPageSB(const Control *item);

void CallFuncOnPressButton(void *button);

/** @}
 */
