#pragma once
#include "Panel/Controls.h"
#include "MenuItems.h"


/** @defgroup Menu
 *  @{
 */


class Menu
{
public:
    /// ���������� ����� �������� ����, ���������������� ������ button.
    void* ItemUnderButton(PanelButton button);
    /// ���������� true, ���� ������� ���� item ������ (��������� �� �� ����� ������� ����. ��� �������, ��� ��������, ��� ������� �������������� ������� ���� ����� Choice ��� Governor.
    bool IsShade(void* item);
    bool IsPressed(void* item);
    void Draw();
    /// ������� ������ ���������� � ������� �����.
    void UpdateInput();
    /// ������� ��������� ��������� ������� ������ (����� 0.5 ���.).
    void ShortPressureButton(PanelButton button);
    /// ������� ��������� �������� ������� ������ (����� 0.5 ���.).
    void LongPressureButton(PanelButton button);
    /// ������� ����������, ����� ������ ��������� �� �������� � ������� ���������.
    void PressButton(PanelButton button);
    /// ������� ����������, ����� ������ ��������� �� �������� � ������� ���������.
    void ReleaseButton(PanelButton button);
    /// ������� ��������� �������� ����� ��������� ������.
    void RotateRegSetRight();
    /// ������� ��������� �������� ����� ��������� �����.
    void RotateRegSetLeft();
    /// ���������� ����� ��������������� �������� ���� � ������������ � �����������.
    void SetAutoHide(bool active);
    /// ���������� ����� ������ ����, ������������ ��� ������� � ������ ������ �������.
    void* ItemUnderKey();
    /// ���������� ���� � �������� ������ ���� � ��������� ����, ������� � ������ �� �����.
    char* StringNavigation(char buffer[100]);

    void OpenItemTime();
    /// ���������� true, ���� ������� ���� item �������� �������, �.�. ����� ���� �����.
    bool ItemIsActive(void *item);
    /// ���������� ���������� ��������� � �������� �� ������ page.
    int NumItemsInPage(const Page * const page);
    /// ���������� ����� ������� ���������� �������� �� ������ page.
    int NumCurrentSubPage(Page *page);
    /// D��������� ����� ���������� � �������� �� ������ page.
    int NumSubPages(const Page *page);
    /// ���������� ��� �������� ���� �� ������ address.
    TypeItem TypeMenuItem(void *address);
    /// ���������� ��� ��������� �������� ����.
    TypeItem TypeOpenedItem();
    /// ���������� ����� ��������� �������� ����.
    void* OpenedItem();
    /// ���������� ����� �������� �������� ���� (�������, ��� �������, �������� �������, ������ �������� ���� ������ ���������.
    void* CurrentItem();
    /// ���������� ����� �������� ���� �������� ��������.
    void* Item
                (const Page *page,              ///< ��������, ������� ������� ����� ������
                int numElement                  ///< ���������� ����� ��������, ������� ����� ������
                );
    /// ���������� true, ���� ������� ������� �������� � ������ namePage ������.
    bool CurrentItemIsOpened(NamePage namePage);
    /// ���������� ������� �������� �������� ������� page.
    int8 PosCurrentItem(const Page *page);
    /// �������/��������� ������� ����� ��������.
    void SetCurrentItem
                        (const void *item,      ///< ����� ��������, ���������� �������� ����� ����������/�����
                        bool active             ///< true - ������� ������� �������, false - ��������� ������� �������. � ���� ������ ������� ���������� ��������� ��������
                        );
    /// ���������� ����� ��������, �������� ����������� ������� �� ������ item.
    Page* Keeper(const void *item);
    /// ���������� ��� �������� page.
    NamePage GetNamePage(const Page *page);

    NamePage GetNameOpenedPage();

    void OpenPageAndSetItCurrent(NamePage namePage);

    bool ChangeOpenedItem(void *item, int delta);
    /// ��������� ��� ����������� �������� Governor, GovernorColor ��� Choice �� ������ item � ����������� �� ����� delta
    void ChangeItem(void *item, int delta);
    /// ���������� ������ � �������� ��������� �������� Choice ��� NamePage �� ������ item.
    int HeightOpenedItem(void *item);
    /// ���������� �������� �������� �� ������ item, ��� ��� �������� �� ������� �������.
    const char* TitleItem(void *item);
    /// ���������� ������� ������� �������� �������� �� ������ page �� ������. ���� ������� ����������� 0, ��� ����� 0, ���� ������� ����������� 1, ��� ����� 5 � �.�.
    int PosItemOnTop(Page *page);
    /// ���������� true, ���� button - �������������� ������� [1..5].
    bool IsFunctionalButton(PanelButton button);
    /// ������� �������� ������� ����.
    void CloseOpenedItem();

    void OpenItem(const void *item, bool open);          // �������/������� ������� ���� �� ������ item.

    bool ItemIsOpened(void *item);                       // ���������� true, ���� ������� ���� �� ������ item ������.

    void ChangeSubPage(const Page *page, int delta);

    //void PrintStatePage(Page *page);

    void ShortPressOnPageItem(Page *page, int numItem);   // �������� ������� ��������� ������� ������ ��� ������ numItem �������� page;

    Page* PagePointerFromName(NamePage namePage);

    bool PageIsSB(const Page *page);

    SmallButton* SmallButonFromPage(Page *page, int numButton);

    void ChangeStateFlashDrive();

private:
    /// ��������� ��������� ������� ������.
    void ProcessingShortPressureButton();
    /// ��������� �������� ������� ������.
    void ProcessingLongPressureButton();
    /// ��������� ��������� ������ ����.
    void ProcessingPressButton();
    /// ��������� �������� ������ �����.
    void ProcessingReleaseButton();
    /// ��������� �������� ����� ���������.
    void ProcessingRegulatorSet();
    /// ��������/��������� ��������� ����� ���������, ���� ����������.
    void SwitchSetLED();
    /// ��������� ��������� ������� �� ������� NamePage � ������� page.
    void ShortPress_Page(void *page);
    /// ��������� ��������� ������� �� ������� Choice � ������� choice.
    void ShortPress_Choice(void *choice);

    void ShortPress_Time(void *time);
    /// ��������� ��������� ������� �� ������� Button � ������� button.
    void ShortPress_Button(void *button);
    /// ��������� ��������� ������� �� ������� Governor � ������� governor.
    void ShortPress_Governor(void *governor);
    /// ��������� ��������� ������� �� ������� GovernorColor � ������� governorColor.
    void ShortPress_GovernorColor(void *governorColor);

    void ShortPress_IP(void *item);

    void ShortPress_MAC(void *item);

    void ShortPress_ChoiceReg(void *choice);

    void ShortPress_SmallButton(void *smallButton);
    /// ��������� �������� ������� �� ������� ���� item.
    void FuncOnLongPressItem(void *item);

    void FuncOnLongPressItemTime(void *item);
    /// ��������� �������� ������� �� ������� Button � ������� button.
    void FuncOnLongPressItemButton(void *button);
    /// ���������� ������� ��������� ��������� ������� �� ������� ���� item.
    void ExecuteFuncForShortPressOnItem(void *item);
    /// ���������� ������� ��������� �������� ������� �� ������� ���� item.
    void ExecuteFuncForLongPressureOnItem(void *item);

    bool NeedForFireSetLED();

    void DrawOpenedPage(Page *page, int layer, int yTop);

    void DrawTitlePage(Page *page, int layer, int yTop);

    void DrawItemsPage(Page *page, int layer, int yTop);

    void DrawPagesUGO(Page *page, int right, int bottom);

    int CalculateX(int layer);

    void ResetItemsUnderButton();

    int ItemOpenedPosY(void *item);

    void* RetLastOpened(Page *_page, TypeItem *_type);
};

extern Menu menu;

/** @}
 */
