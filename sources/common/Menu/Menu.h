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
    void Init(void);

    void ChangeStateFlashDrive();
    /// ������� ������ ���������� � ������� �����.
    void UpdateInput(void);
    /// ������� ��������� ��������� ������� ������ (����� 0.5 ���.).
    void ShortPressureButton(PanelButton button);
    /// ������� ��������� �������� ������� ������ (����� 0.5 ���.).
    void LongPressureButton(PanelButton button);
    /// ������� ����������, ����� ������ ��������� �� �������� � ������� ���������.
    void PressButton(PanelButton button);
    /// ������� ����������, ����� ������ ��������� �� �������� � ������� ���������.
    void ReleaseButton(PanelButton button);
    /// ������� ���������� � ������ ������� ��������.
    void PressReg(PanelRegulator reg);
    /// ������� ��������� �������� ����� ��������� ������.
    void RotateRegSetRight(void);
    /// ������� ��������� �������� ����� ��������� �����.
    void RotateRegSetLeft(void);
    /// ���������� ����� ��������������� �������� ���� � ������������ � �����������.
    void SetAutoHide(bool active);
    /// ���������� ����� ������ ����, ������������ ��� ������� � ������ ������ �������.
    void *ItemUnderKey(void);
    /// ���������� ���� � �������� ������ ���� � ��������� ����, ������� � ������ �� �����.
    char *StringNavigation(char buffer[100]);

    void Show(bool show);

    void OpenItemTime(void);
    /// ������� �������� �������� ����������� ������ ��������� ����, ���� ������ �����������.
    void TemporaryEnableStrNavi(void);
    /// ���������� ��� ������� ����� Menu_UpdateInput().
    void RunAfterUpdate(pFuncVV func);
    /// ���������� ����
    void Draw();
    /// ���������� ����� ��������� �������� ����
    void *OpenedItem(void);
    /// ���������� ��� ��������� �������� ����
    TypeItem TypeOpenedItem(void);

    NamePage GetNameOpenedPage(void);

    Page *PagePointerFromName(NamePage namePage);
    /// ���������� ����� �������� �������� ���� (�������, ��� �������, �������� �������, ������ �������� ���� ������ ���������
    void *CurrentItem(void);
    /// ������� �������� ������� ����
    void CloseOpenedItem(void);
    /// ��������� ��� ����������� �������� Governor, GovernorColor ��� Choice �� ������ item � ����������� �� ����� delta
    void ChangeItem(void *item, int delta);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \todo STUB
    void OpenPageAndSetItCurrent(NamePage namePage);

    void ShortPressOnPageItem(Page *, int);

    NamePage GetNamePage(const Page *);

private:
    void *RetLastOpened(Page *_page, TypeItem *_type);
};


extern Menu menu;


/** @}
 */
