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

/// ���������� true, ���� ������� ���� item �������� �������, �.�. ����� ���� �����
bool ItemIsAcitve(const void *item);
/// ���������� ��� �������� ���� �� ������ address
TypeItem TypeMenuItem(const void *address);
/// ���������� true, ���� ������� ������� �������� � ������ namePage ������
bool CurrentItemIsOpened(Page *page);
/// �������/��������� ������� ����� ��������
void SetCurrentItem(const void *item,               ///< ����� ��������, ���������� �������� ����� ����������/�����
                    bool active                     ///< true - ������� ������� �������, false - ��������� ������� �������. � ���� ������ ������� ���������� ��������� ��������
                    );
/// ���������� ����� ��������, �������� ����������� ������� �� ������ item
Page *Keeper(const void *item);

bool ChangeOpenedItem(void *item, int delta);
/// ���������� ������ � �������� ��������� �������� Choice ��� NamePage �� ������ item
int HeightOpenedItem(void *item);
/// ���������� �������� �������� �� ������ item, ��� ��� �������� �� ������� �������
const char *TitleItem(void *item);
/// ���������� true, ���� button - �������������� ������� [1..5]
bool IsFunctionalButton(PanelButton button);
/// �������/������� ������� ���� �� ������ item
void OpenItem(const void *item, bool open);
/// ���������� true, ���� ������� ���� �� ������ item ������
bool ItemIsOpened(const void *item);

bool IsPageSB(const void *item);

void CallFuncOnPressButton(void *button);

/** @}
 */
