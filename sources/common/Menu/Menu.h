#pragma once
#include "defines.h"
#include "Panel/Controls.h"
#include "Menu/MenuPagesNames.h"
#include "Menu/MenuItems.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @defgroup Menu
 *  @{
 *  @defgroup Pages
 *  @{
 *  @}
 */


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Menu
{
public:
    /// �������������
    static void Init();

    static void ChangeStateFlashDrive();
    /// ������� ������ ���������� � ������� �����.
    static void UpdateInput();
    /// ������� ��������� ��������� ������� ������ (����� 0.5 ���.).
    static void ShortPressureButton(PanelButton button);
    /// ������� ��������� �������� ������� ������ (����� 0.5 ���.).
    static void LongPressureButton(PanelButton button);
    /// ������� ����������, ����� ������ ��������� �� �������� � ������� ���������.
    static void PressButton(PanelButton button);
    /// ������� ����������, ����� ������ ��������� �� �������� � ������� ���������.
    static void ReleaseButton(PanelButton button);
    /// ������� ���������� � ������ ������� ��������.
    static void PressReg(PanelRegulator reg);
    /// ������� ��������� �������� ����� ��������� ������.
    static void RotateRegSetRight();
    /// ������� ��������� �������� ����� ��������� �����.
    static void RotateRegSetLeft();
    /// ���������� ����� ��������������� �������� ���� � ������������ � �����������.
    static void SetAutoHide(bool active);
    /// ���������� ����� ������ ����, ������������ ��� ������� � ������ ������ �������.
    static void *ItemUnderKey();
    /// ���������� ���� � �������� ������ ���� � ��������� ����, ������� � ������ �� �����.
    static char *StringNavigation(char buffer[100]);

    static void Show(bool show);

    static void OpenItemTime();
    /// ������� �������� �������� ����������� ������ ��������� ����, ���� ������ �����������.
    static void TemporaryEnableStrNavi();
    /// ���������� ��� ������� ����� Menu_UpdateInput().
    static void RunAfterUpdate(pFuncVV func);
    /// ���������� ����
    static void Draw();
    /// ���������� ����� ��������� �������� ����
    static Control *OpenedItem();
    /// ���������� ��� ��������� �������� ����
    static TypeItem TypeOpenedItem();

    static NamePage GetNameOpenedPage();

    static Page *PagePointerFromName(NamePage namePage);
    /// ���������� ����� �������� �������� ���� (�������, ��� �������, �������� �������, ������ �������� ���� ������ ���������
    static Control *CurrentItem();
    /// ������� �������� ������� ����
    static void CloseOpenedItem();
    /// ��������� ��� ����������� �������� Governor, GovernorColor ��� Choice �� ������ item � ����������� �� ����� delta
    static void ChangeItem(void *item, int delta);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \todo STUB
    static void OpenPageAndSetItCurrent(NamePage namePage);

    static void ShortPressOnPageItem(Page *, int);

    static NamePage GetNamePage(const Page *);

private:
    static void *RetLastOpened(Page *_page, TypeItem *_type);
};


/** @}
 */
