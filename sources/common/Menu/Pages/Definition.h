#pragma once
#include "defines.h"
#include "Panel/Controls.h"
#include "Settings/SettingsTypes.h"
#include "Menu/MenuItems.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define COMMON_BEGIN_SB_EXIT  "�����", "Exit", "������ ��� ������ � ���������� ����", "Button to return to the previous menu",


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ���������� �������� ����, ������� ������ ����������� �� ������� ������ button.
const void *PageForButton(PanelButton button);
/// ���������� true, ���� item - ����� ������� �������� ����.
bool IsMainPage(const void *item);
/// ������� ��������� ������ ������ �� ������ ������ ����.
void DrawSB_Exit(int x, int y);
/// ������������� ������� ��������� ������� ������ ������ �� ������ ������ ����.
void OnPressSB_Exit(void);

void DrawMenuCursVoltage(int x, int y, bool top, bool bottom);

void DrawMenuCursTime(int x, int y, bool left, bool right);
/// ������������ ������� ��������� ��� ����� ������ ���������� ������� ���������.
void CalculateConditions(int16 pos0, int16 pos1, CursCntrl cursCntrl, bool *cond0, bool *cond1);

extern const Page mainPage;
