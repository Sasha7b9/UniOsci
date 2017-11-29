#pragma once
#include "Panel/Controls.h"


/// ���������� ����� �������� ����, ���������������� ������ button
void *ItemUnderButton(PanelButton button);
/// ���������� true, ���� ������, ��������������� �������� ���� item, ��������� � ������� ���������
bool IsPressed(void *item                   ///< ��������� �� ������� ����
                );
/// @brief ���������� true, ���� ������� ���� item ������� (��������� �� �� ����� ������� ����. ��� �������, ��� ��������, ��� ������� �������������� ������� 
/// ���� ����� Choice ��� Governor
bool IsShade(void *item                     ///< ��������� �� ������� ����
            );