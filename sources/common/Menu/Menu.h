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


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \todo STUB
    void OpenPageAndSetItCurrent(NamePage namePage);

    Page *PagePointerFromName(NamePage namePage);

    void ShortPressOnPageItem(Page *, int);

    void *OpenedItem();

    TypeItem TypeOpenedItem();

    NamePage GetNamePage(const Page *);

    NamePage GetNameOpenedPage();
};


extern Menu menu;


/** @}
 */
