#pragma once
#include "Panel/Controls.h"
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
    /// ������� ���������� � ������ ������� ���������.
    static void PressReg(PanelRegulator reg);
    /// ������� ��������� �������� ����� ��������� ������.
    static void RotateRegSetRight();
    /// ������� ��������� �������� ����� ��������� �����.
    static void RotateRegSetLeft();
    /// ���������� ����� ��������������� �������� ���� � ������������ � �����������.
    static void SetAutoHide(bool active);
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

    static NamePage GetNameOpenedPage();

    static Page *PagePointerFromName(NamePage namePage);
    /// ���������� ����� �������� �������� ���� (�������, ��� �������, �������� �������, ������ �������� ���� ������ ���������
    static Control *CurrentItem();
    /// ������� �������� ������� ����
    static void CloseOpenedItem();
    /// ��������� ��� ����������� �������� Governor, GovernorColor ��� Choice �� ������ item � ����������� �� ����� delta
    static void ChangeItem(Control *item, int delta);

    static int CalculateX(int layer);

    ///\brief  ����� �������� ����� �������� ����, ���������������� �������������� ������� [1..5], ���� ��� ��������� � ������ ���������, � 0, 
    /// ���� �� ���� ������ �� ������.
    static void *itemUnderKey;

    static void *itemUnderButton[B_NumButtons];
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \todo STUB
    static void OpenPageAndSetItCurrent(NamePage namePage);

    static void ShortPressOnPageItem(Page *, int);

    static NamePage GetNamePage(const Page *);

private:
    static void *RetLastOpened(Page *_page, TypeItem *_type);
    /// ��������� ��������� ������� ������
    static void ProcessingShortPressureButton();
    /// ��������� �������� ������� ������
    static void ProcessingLongPressureButton();
    /// ��������� ��������� ������ ����
    static void ProcessingPressButton();
    /// ��������� �������� ������ �����
    static void ProcessingReleaseButton();
    ///< ��������� �������� ����� ���������
    static void ProcessingRegulatorSetRotate();
    /// ��������� ������� �����
    static void ProcessingRegulatorPress();
    /// ��������� ������� ������� ��������������� �������� ����
    static void OnTimerAutoHide();
    /// ��������/��������� ��������� ����� ���������, ���� ����������
    static void SwitchSetLED();
    /// �������, ������� �������� ����� ������ ��������� ����
    static void OnTimerStrNaviAutoHide();
    /// ���������� true, ���� �������� ��������� ������ ������
    static bool NeedForFireSetLED();
    
    static void ProcessButtonForHint(PanelButton button);
    
    static void ShortPress_ChoiceReg(void *choice);
    
    static void ShortPress_IP(void *item);
    
    static void ShortPress_MAC(void *item);
    
    static void ResetItemsUnderButton();
          
    /// ���� ��������� �������� ������� ������, �� ����� �������� ��� ���� ������ �� ���������  ����� �������.
    static PanelButton shortPressureButton;
    /// ���� ��������� ������� ������� ������, �� ����� �������� ��� ���� ������ �� ��������� ����� �������.
    static PanelButton longPressureButton;
    /// ��� ������� ������ � ��� ������������ � ��� ���������� � �������� ��� �� �������� ������� ������� ������.
    static PanelButton pressButton;
    /// ��� ���������� ������ � ��� ������������ � ��� ���������� � �������� ��� �� ��������� ������� ���������� ������.
    static PanelButton releaseButton;
    
    static PanelRegulator pressRegulator;
    /// ����, �� ������� ����� ��������� ����� ��������� - �������� �������� ���������� �������, ���� - ����������� - "-" - �����, "+" - ������.
    static int angleRegSet;
    /// ��� ������� ����� �������� ���� ��� ����� Menu::UpdateInput().
    static pFuncVV funcAterUpdate;
  
    static const int SIZE_BUFFER_FOR_BUTTONS = 10;
    static PanelButton bufferForButtons[SIZE_BUFFER_FOR_BUTTONS];
    
    static const PanelButton sampleBufferForButtons[SIZE_BUFFER_FOR_BUTTONS];
};


/** @}
 */
