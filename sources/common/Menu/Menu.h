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
    static void Init(void);

    static void ChangeStateFlashDrive();
    /// ������� ������ ���������� � ������� �����.
    static void UpdateInput(void);
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
    static void RotateRegSetRight(void);
    /// ������� ��������� �������� ����� ��������� �����.
    static void RotateRegSetLeft(void);
    /// ���������� ����� ��������������� �������� ���� � ������������ � �����������.
    static void SetAutoHide(bool active);
    /// ���������� ����� ������ ����, ������������ ��� ������� � ������ ������ �������.
    static void *ItemUnderKey(void);
    /// ���������� ���� � �������� ������ ���� � ��������� ����, ������� � ������ �� �����.
    static char *StringNavigation(char buffer[100]);

    static void Show(bool show);

    static void OpenItemTime(void);
    /// ������� �������� �������� ����������� ������ ��������� ����, ���� ������ �����������.
    static void TemporaryEnableStrNavi(void);
    /// ���������� ��� ������� ����� Menu_UpdateInput().
    static void RunAfterUpdate(pFuncVV func);
    /// ���������� ����
    static void Draw();
    /// ���������� ����� ��������� �������� ����
    static Control *OpenedItem(void);
    /// ���������� ��� ��������� �������� ����
    static TypeItem TypeOpenedItem(void);

    static NamePage GetNameOpenedPage(void);

    static Page *PagePointerFromName(NamePage namePage);
    /// ���������� ����� �������� �������� ���� (�������, ��� �������, �������� �������, ������ �������� ���� ������ ���������
    static Control *CurrentItem(void);
    /// ������� �������� ������� ����
    static void CloseOpenedItem(void);
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
