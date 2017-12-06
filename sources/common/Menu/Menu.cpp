#include "Log.h"
#include "Menu.h" 
#include "MenuDrawing.h"
#include "Menu/MenuFunctions.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "Hardware/Sound.h"
#include "Hardware/Timer.h"
#include "Pages/Definition.h"
#include "Menu/Pages/PageCursors.h"
#include "Menu/Pages/PageDisplay.h"
#include "Menu/Pages/PageMemory.h"
#include "Panel/Panel.h"
#include "Utils/Math.h"
#include "Utils/CommonFunctions.h"
#include "stub.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Menu menu;

extern void OnChanged_InputA(bool active);
extern void OnChanged_InputB(bool active);
extern void Long_Help(void);
extern const PageBase * const pointerPageHelp;

/// ���� ��������� �������� ������� ������, �� ����� �������� ��� ���� ������ �� ���������  ����� �������.
static PanelButton shortPressureButton = B_Empty;
/// ���� ��������� ������� ������� ������, �� ����� �������� ��� ���� ������ �� ��������� ����� �������.
static PanelButton longPressureButton = B_Empty;
/// ��� ������� ������ � ��� ������������ � ��� ���������� � �������� ��� �� �������� ������� ������� ������.
static PanelButton pressButton = B_Empty;
/// ��� ���������� ������ � ��� ������������ � ��� ���������� � �������� ��� �� ��������� ������� ���������� ������.
static PanelButton releaseButton = B_Empty;
static PanelRegulator pressRegulator = R_Empty;
/// ����, �� ������� ����� ��������� ����� ��������� - �������� �������� ���������� �������, ���� - ����������� - "-" - �����, "+" - ������.
static int angleRegSet = 0;
///\brief  ����� �������� ����� �������� ����, ���������������� �������������� ������� [1..5], ���� ��� ��������� � ������ ���������, � 0, ���� �� ���� 
/// ������ �� ������.
static void *itemUnderKey = 0;
/// ��� ������� ����� �������� ���� ��� ����� Menu::UpdateInput().
static pFuncVV funcAterUpdate = 0;
                                                
static void ProcessingShortPressureButton(void);            ///< ��������� ��������� ������� ������.
static void ProcessingLongPressureButton(void);             ///< ��������� �������� ������� ������.
static void ProcessingPressButton(void);                    ///< ��������� ��������� ������ ����.
static void ProcessingReleaseButton(void);                  ///< ��������� �������� ������ �����.
static void ProcessingRegulatorSetRotate(void);             ///< ��������� �������� ����� ���������.
static void ProcessingRegulatorPress(void);                 ///< ��������� ������� �����.
static void OnTimerAutoHide(void);                          ///< ��������� ������� ������� ��������������� �������� ����.
static void SwitchSetLED(void);                             ///< ��������/��������� ��������� ����� ���������, ���� ����������.
static void ShortPress_Page(void *page);                    ///< ��������� ��������� ������� �� ������� NamePage � ������� page.
static void ShortPress_Choice(void *choice);                ///< ��������� ��������� ������� �� ������� Choice � ������� choice.
static void ShortPress_Time(void *time);
static void ShortPress_Button(void *button);                ///< ��������� ��������� ������� �� ������� Button � ������� button.
static void ShortPress_Governor(void *governor);            ///< ��������� ��������� ������� �� ������� Governor � ������� governor.
static void ShortPress_GovernorColor(void *governorColor);  ///< ��������� ��������� ������� �� ������� GovernorColor � ������� governorColor.
static void FuncOnLongPressItem(void *item);                ///< ��������� �������� ������� �� ������� ���� item.
static void FuncOnLongPressItemTime(void *item);
static void FuncOnLongPressItemButton(void *button);        ///< ��������� �������� ������� �� ������� Button � ������� button.

static  pFuncVpV    FuncForShortPressOnItem(void *item);    ///< ���������� ������� ��������� ��������� ������� �� ������� ���� item.
static  pFuncVpV    FuncForLongPressureOnItem(void *item);  ///< ���������� ������� ��������� �������� ������� �� ������� ���� item.

static void OnTimerStrNaviAutoHide(void);                   ///< �������, ������� �������� ����� ������ ��������� ����.


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIZE_BUFFER_FOR_BUTTONS 10
static PanelButton bufferForButtons[SIZE_BUFFER_FOR_BUTTONS] = {B_Empty};
static const PanelButton sampleBufferForButtons[SIZE_BUFFER_FOR_BUTTONS] = {B_F5, B_F5, B_F4, B_F4, B_F3, B_F3, B_F2, B_F2, B_F1, B_F1};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::UpdateInput(void)
{
    ProcessingShortPressureButton();
    ProcessingLongPressureButton();
    ProcessingRegulatorSetRotate();
    ProcessingPressButton();
    ProcessingReleaseButton();
    ProcessingRegulatorPress();
    SwitchSetLED();

    if (funcAterUpdate)
    {
        funcAterUpdate();
        funcAterUpdate = 0;
    }
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ShortPressureButton(PanelButton button)
{
    if (!HINT_MODE_ENABLE)
    {
        if (button == B_Memory && FDRIVE_IS_CONNECTED && MODE_BTN_MEMORY_IS_SAVE)
        {
            NEED_SAVE_TO_FLASHDRIVE = 1;
        }

        shortPressureButton = button;
    }
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::LongPressureButton(PanelButton button)
{
    if (!HINT_MODE_ENABLE)
    {
        longPressureButton = button;
        NEED_FINISH_DRAW = 1;
    }
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void ProcessButtonForHint(PanelButton button)
{
    if (button == B_Menu)
    {
        gStringForHint = LANG_RU ?
            "������ ���� ��������� ��������� �������:\n"
            "1. ��� �������� ���� ������� ���� ������� � ���������� � ������� 0.5� ��������� ����.\n"
            "2. ��� �������� ���� ��������� ������ � ������� 0.5� ��������� ����.\n"
            "3. ��� ��������� \"������\x99����� �� ����\x99���������\" ������� ���������� �������� ����������� ������ ����. ���� ������� �������� "
            "�������� ��������, ���� �����������.\n"
            "4. ��� ��������� \"������\x99����� �� ����\x99�����������\" ������� ���������� �������� �������� ������ ����. ���� ������� �������� "
            "��������� � ������� ������, ���������� ������� �� ���������� ������� ����.\n"
            "5. ���� ���� ��������� � ������ ����� ������, �� ������� ��������� ��������."
            :
        "���� button performs the following functions:\n"
            "1. At the closed menu pressing or pressing with deduction during 0.5s opens the menu.\n"
            "2. At the open menu deduction of the button during 0.5s closes the menu.\n"
#ifndef WIN32
#pragma push
#pragma diag_suppress 192
            "3. At control \"SERVICE\x99Mode btn MENU\x99\Close\" current becomes the page of the previous level of the menu. If the root page is "
            "current, the menu is closed.\n"
#pragma pop
#endif
            "4. At control \"SERVICE\x99Mode btn MENU\x99Toggle\" current becomes the page of the current level of the menu. If the current page the "
            "last in the current level, happens transition to the previous level of the menu.\n"
            "5. If the menu is in the mode of small buttons, pressing closes the page.";

    } else if (button == B_Cursors)
    {
        gStringForHint = LANG_RU ? 
            "������ ������� ��������� ���� ��������� ���������."
            :
            "������� button to open the menu cursor measurements.";
    }
    else if (button == B_Display)
    {
        gStringForHint = LANG_RU ?
            "������ ������� ��������� ���� �������� �������."
            :
            "������� button opens the display settings menu.";
    }
    else if (button == B_Memory)
    {
        gStringForHint = LANG_RU ?
            "1. ��� ��������� \"������\x99����� ��\x99��� �� ������\x99����\" ��������� ���� ������ � �������.\n"
            "2. ��� ��������� \"������\x99����� ��\x99��� �� ������\x99����������\" ��������� ������ �� ����-����."
            :
            "1. When setting \"������-EXT\x99STORAGE\x99Mode btn MEMORY\x99Menu\" opens a menu of memory\n"
            "2. When setting \"������-EXT\x99STORAGE\x99Mode btn MEMORY\x99Save\" saves the signal to the flash drive";
    }
    else if (button == B_Measures)
    {
        gStringForHint = LANG_RU ?
            "������ ����� ��������� ���� �������������� ���������."
            :
            "����� button opens a menu of automatic measurements.";
    }
    else if (button == B_Help)
    {
        gStringForHint = LANG_RU ?
            "1. ������ ������ ��������� ���� ������.\n"
            "2. ������� � ��������� ������ ������ � ������� 0.5� �������� � ��������� ����� ������ ���������."
            :
            "1. ������ button opens the help menu.\n"
            "2. Pressing and holding the ������ for 0.5s enables and disables output mode hints.";
    }
    else if (button == B_Service)
    {
        gStringForHint = LANG_RU ?
            "������ ������ ��������� ���� ��������� ������������."
            :
            "������ button opens a menu of service options.";
    }
    else if (button == B_Start)
    {
        gStringForHint = LANG_RU ?
            "������ ����/�TO� ��������� � ������������� ������� ����� ����������."
            :
            "����/�TO� button starts and stops the process of gathering information.";
    }
    else if (button == B_ChannelA)
    {
        gStringForHint = LANG_RU ?
            "1. ������ �����1 ��������� ���� �������� ������ 1.\n"
            "2. ������� � ��������� ������ �����1 � ������� 0.5� ������������� �������� ������ 1 �� ��������� 0�."
            :
            "1. �����1 button opens the settings menu of the channel 1.\n"
            "2. Pressing and holding the button �����1 for 0.5c for the offset of the vertical channel 1 0V.";
    }
    else if (button == B_ChannelB)
    {
        gStringForHint = LANG_RU ?
            "1. ������ �����2 ��������� ���� �������� ������ 2.\n"
            "2. ������� � ��������� ������ �����2 � ������� 0.5� ������������� �������� ������ 2 �� ��������� 0�."
            :
            "1. �����2 button opens the settings menu of the channel 2.\n"
            "2. Pressing and holding the button �����2 for 0.5c for the offset of the vertical channel 2 0V.";
    }
    else if (button == B_Time)
    {
        gStringForHint = LANG_RU ?
            "1. ������ ���� ��������� ���� �������� ���������.\n"
            "2. ������� � ��������� ������ ���� � ������� 0.5� ������������� �������� �� ����������� 0�."
            :
            "1. ���� button open the settings menu sweep.\n"
            "2. Pressing and holding the button ���� for 0.5s Is the offset horizontal 0s.";
    }
    else if (button == B_Trig)
    {
        gStringForHint = LANG_RU ?
            "1. ������ ����� ��������� ���� �������� �������������.\n"
            "2. ������� � ��������� � ������� 0.5� ������ ����� ��� ��������� \"������\x99��� ���� �����\x99�����������\" ���������� �������������� "
            "��������� ������ �������������.\n"
            "3. ������� � ��������� � ������� 0.5� ������ ����� ��� ��������� \"������\x99��� ���� �����\x99����� ������\" ������������� ������� "
            "������������� 0�."
            :
            "1. ����� button opens a menu settings synchronization.\n"
#ifndef WIN32
#pragma push
#pragma diag_suppress 192
            "2. Pressing and holding the button ����� for 0.5s when setting \"SERVICE\x99Mode long TRIG\x99\Autolevel\" automatically adjust the "
            "trigger level.\n"
#pragma pop
#endif
            "3. Pressing and holding the button ����� for 0.5s when setting \"SERVICE\x99Mode long TRIG\x99SReset trig level\" sets the trigger "
            "level 0V.";
    }
    else
    {
        shortPressureButton = button;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::PressButton(PanelButton button)
{
    sound.ButtonPress();
    if (HINT_MODE_ENABLE)
    {
        ProcessButtonForHint(button);
        return;
    }

    if (!MENU_IS_SHOWN)
    {
        for (int i = SIZE_BUFFER_FOR_BUTTONS - 1; i > 0; i--)
        {
            bufferForButtons[i] = bufferForButtons[i - 1];
        }
        bufferForButtons[0] = button;
      
        if (memcmp(bufferForButtons, sampleBufferForButtons, SIZE_BUFFER_FOR_BUTTONS) == 0)
        {
            SHOW_DEBUG_MENU = 1;
            Display::ShowWarning(MenuDebugEnabled);
        }
    }
    pressButton = button;
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ReleaseButton(PanelButton button)
{
    sound.ButtonRelease();
    if (!HINT_MODE_ENABLE)
    {
        releaseButton = button;
    }
};

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::PressReg(PanelRegulator reg)
{
    if (!HINT_MODE_ENABLE)
    {
        pressRegulator = reg;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::RotateRegSetRight(void)
{   
    if (!HINT_MODE_ENABLE)
    {
        angleRegSet++;
        NEED_FINISH_DRAW = 1;
    }
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::RotateRegSetLeft(void)
{
    if (!HINT_MODE_ENABLE)
    {
        angleRegSet--;
        NEED_FINISH_DRAW = 1;
    }
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
void *Menu::ItemUnderKey(void)
{
    return itemUnderKey;
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::SetAutoHide(bool)
{
    if(!MENU_IS_SHOWN)
    {
        return;
    }
    if(sDisplay_TimeMenuAutoHide() == 0)
    {
        Timer::Disable(kMenuAutoHide);
    }
    else
    {
        Timer::SetAndStartOnce(kMenuAutoHide, OnTimerAutoHide, (uint)sDisplay_TimeMenuAutoHide());
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Menu::StringNavigation(char buffer[100])
{
    buffer[0] = 0;
    const char * titles[10] = {0};
    int numTitle = 0;
    void *item = OpenedItem();
    if(IsMainPage(item))
    {
        return 0;
    }
    while(!IsMainPage(item))
    {
        titles[numTitle++] = TitleItem(item);
        item = ((Control *)item)->Keeper();
    }
    for(int i = 9; i >= 0; i--)
    {
        if(titles[i])
        {
            strcat(buffer, titles[i]);
            if(i != 0)
            {
                strcat(buffer, " - ");
            }
        }
    }
    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnTimerAutoHide(void)
{
    menu.Show(false);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void ProcessingShortPressureButton(void)
{
    if(shortPressureButton != B_Empty)
    {
        if (shortPressureButton == B_Memory && MODE_BTN_MEMORY_IS_SAVE && FDRIVE_IS_CONNECTED)
        {
            EXIT_FROM_SETNAME_TO = (uint)(MENU_IS_SHOWN ? RETURN_TO_MAIN_MENU : RETURN_TO_DISABLE_MENU);
            Memory_SaveSignalToFlashDrive();
            shortPressureButton = B_Empty;
            return;
        }
        NEED_FINISH_DRAW = 1;
        menu.SetAutoHide(true);

        PanelButton button = shortPressureButton;

        do
        {
            if (button == B_Help)
            {
                Long_Help();
            }
            else if(button == B_Menu)                                   // ���� ������ ������ ���� � �� �� ��������� � ����� ��������� ���������.
            {
                if(!MENU_IS_SHOWN)
                {
                    menu.Show(true);
                }
                else
                {
                    if (menu.TypeOpenedItem() == Item_Page)
                    {
                        menu.TemporaryEnableStrNavi();
                    }
                    menu.CloseOpenedItem();
                }
            }
            else if (MENU_IS_SHOWN && IsFunctionalButton(button))       // ���� ���� �������� � ������ �������������� �������
            {
                void *item = ItemUnderButton(button);
                if (HINT_MODE_ENABLE)
                {
                    SetItemForHint(item);
                }
                else
                {
                    FuncForShortPressOnItem(item)(item);
                }
            }
            else                                                        // ���� ���� �� ��������.
            {
                NamePage name = ((const Page *)menu.OpenedItem())->GetNamePage();
                if(button == B_ChannelA && name == Page_ChannelA && MENU_IS_SHOWN)
                {
                    SET_ENABLED_A = !SET_ENABLED_A;
                    OnChanged_InputB(true);
                    break;
                }
                if(button == B_ChannelB && name == Page_ChannelB && MENU_IS_SHOWN)
                {
                    SET_ENABLED_B = !SET_ENABLED_B;
                    OnChanged_InputB(true);
                    break;
                }

                const void *page = PageForButton(button);
                if(page && page != pointerPageHelp)
                {
                    SetCurrentItem(page, true);
                    OpenItem(page, true);
                    menu.TemporaryEnableStrNavi();
                    menu.Show(true);
                }
            }
        } while(false);

        shortPressureButton = B_Empty;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ProcessingLongPressureButton(void)
{
    PanelButton button = longPressureButton;

    if(button != B_Empty)
    {
        sound.ButtonRelease();
        NEED_FINISH_DRAW = 1;
        menu.SetAutoHide(true);

        if(button == B_Time)
        {
            FPGA_SetTShift(0);
        }
        else if(button == B_Trig)
        {
            FPGA_SetTrigLev(TRIGSOURCE, TrigLevZero);
        }
        else if(button == B_ChannelA)
        {
            FPGA_SetRShift(A, RShiftZero);
        }
        else if(button == B_ChannelB)
        {
            FPGA_SetRShift(B, RShiftZero);
        }
        else if(button == B_Menu)
        {
            if (menu.OpenedItem()->IsPageSB())
            {
                menu.CloseOpenedItem();
            }
            else
            {
                menu.Show(!MENU_IS_SHOWN);
                if (menu.TypeOpenedItem() != Item_Page)
                {
                    menu.TemporaryEnableStrNavi();
                }
            }
        }
        else if(MENU_IS_SHOWN && IsFunctionalButton(button))
        {
            void *item = ItemUnderButton(button);
            FuncForLongPressureOnItem(item)(item);
            if (menu.TypeOpenedItem() != Item_Page)
            {
                menu.TemporaryEnableStrNavi();
            }
        }
        longPressureButton = B_Empty;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void ProcessingRegulatorPress(void)
{
    if (pressRegulator != R_Empty)
    {
        menu.SetAutoHide(true);
        if (pressRegulator == R_Set)
        {
            menu.Show(!MENU_IS_SHOWN);
            if (menu.TypeOpenedItem() != Item_Page)
            {
                menu.TemporaryEnableStrNavi();
            }
        }

        pressRegulator = R_Empty;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ProcessingRegulatorSetRotate(void)
{
    if (angleRegSet == 0)
    {
        return;
    }

    if (MENU_IS_SHOWN || menu.TypeOpenedItem() != Item_Page)
    {
        Control *item = menu.CurrentItem();
        TypeItem type = item->Type();
        static const int step = 2;
        if (menu.TypeOpenedItem() == Item_Page && (type == Item_ChoiceReg || type == Item_Governor || type == Item_IP || type == Item_MAC))
        {
            if (angleRegSet > step || angleRegSet < -step)
            {
                menu.ChangeItem(item, angleRegSet);
                angleRegSet = 0;
            }
            return;
        }
        else
        {
            item = menu.OpenedItem();
            type = item->Type();
            if (MenuIsMinimize())
            {
                CurrentPageSBregSet(angleRegSet);
            }
            else if (type == Item_Page || type == Item_IP || type == Item_MAC || type == Item_Choice || type == Item_ChoiceReg || type == Item_Governor)
            {
                if (ChangeOpenedItem(item, angleRegSet))
                {
                    angleRegSet = 0;
                }
                return;
            }
            else if (type == Item_GovernorColor)
            {
                menu.ChangeItem(item, angleRegSet);
            }
            else if (type == Item_Time)
            {
                angleRegSet > 0 ? ((Time *)item)->IncCurrentPosition() : ((Time *)item)->DecCurrentPosition();
            }
        }
    }

    angleRegSet = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ProcessingPressButton(void)
{
    if((pressButton >= B_F1 && pressButton <= B_F5) || pressButton == B_Menu)
    {
        if (pressButton != B_Menu)
        {
            itemUnderKey = ItemUnderButton(pressButton);
        }
    }
    if (pressButton == B_Start && !MODE_WORK_IS_RAM)
    {
        FPGA_OnPressStartStop();
    }
    pressButton = B_Empty;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ProcessingReleaseButton(void)
{
    if(releaseButton >= B_F1 && releaseButton <= B_F5 || pressButton == B_Menu)
    {
        itemUnderKey = 0;
        releaseButton = B_Empty;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ShortPress_Page(void *item)
{
    Page *page = (Page *)item;
    if (page->funcOnPress)
    {
        page->funcOnPress();
    }

    page->SetCurrentSB();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::TemporaryEnableStrNavi(void)
{
    if (SHOW_STRING_NAVI_TEMP)
    {
        SHOW_STRING_NAVIGATION = 1;                                            // ������������� ������� ����, ��� ���� �������� ������ ��������� ����
        Timer::SetAndStartOnce(kStrNaviAutoHide, OnTimerStrNaviAutoHide, 3000); // � ��������� ������, ������� ��� ��������
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
static void OnTimerStrNaviAutoHide(void)
{
    SHOW_STRING_NAVIGATION = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ShortPress_Choice(void *choice_)
{
    Choice *choice = (Choice*)choice_;

    if (!choice->IsActive())
    {
        CHOICE_RUN_FUNC_CHANGED(choice, false);
    }
    else if (!ItemIsOpened(choice))
    {
        SetCurrentItem(choice, menu.CurrentItem() != choice);
        choice->StartChange(1);
    }
    else
    {
        choice->ChangeIndex(-1);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ShortPress_ChoiceReg(void *choice_)
{
    Choice *choice = (Choice*)choice_;

    if(!choice->IsActive()) 
    {
        CHOICE_RUN_FUNC_CHANGED(choice, false);
    } 
    else if(menu.OpenedItem() != choice) 
    {
        SetCurrentItem(choice, menu.CurrentItem() != choice);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FuncOnLongPressItemButton(void *button)
{
    ShortPress_Button(button);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ShortPress_Button(void *button)
{
    if(!((Button *)button)->IsActive())
    {
        return;
    }
    SetCurrentItem(button, true);
    CallFuncOnPressButton(button);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FuncOnLongPressItem(void *item)
{
    if (menu.CurrentItem() != item)
    {
        SetCurrentItem(item, true);
    }
    OpenItem(item, !ItemIsOpened((Control *)item));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FuncOnLongPressItemTime(void *time)
{
    if (menu.CurrentItem() != time)
    {
        SetCurrentItem(time, true);
    }
    if(ItemIsOpened((Control *)time) && *((Time*)time)->curField == iSET)
    {
        ((Time *)time)->SetNewTime();
    }
    OpenItem(time, !ItemIsOpened((Control *)time));
    ((Time *)time)->SetOpened();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ShortPress_Time(void *time)
{
    if(!ItemIsOpened((const Control *)time))
    {
        SetCurrentItem(time, true);
        ((Time *)time)->SetOpened();
        OpenItem(time, true);
    }
    else
    {
        ((Time *)time)->SelectNextPosition();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ShortPress_Governor(void *governor)
{
    Governor *gov = (Governor*)governor;
    if(!gov->IsActive())
    {
        return;
    }
    if(menu.OpenedItem() == gov)
    {
        gov->NextPosition();
    }
    else
    {
        SetCurrentItem(gov, menu.CurrentItem() != gov);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ShortPress_IP(void *item)
{
    if (menu.OpenedItem() == item)
    {
        ((IPaddress*)item)->NextPosition();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ShortPress_MAC(void *item)
{
    if (menu.OpenedItem() == item)
    {
        CircleIncrease<int8>(&gCurDigit, 0, 5);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ShortPress_GovernorColor(void *governorColor)
{
    GovernorColor *governor = (GovernorColor*)governorColor;
    if(!governor->IsActive())
    {
        return;
    }
    if(menu.OpenedItem() == governor)
    {
        CircleIncrease<int8>(&(governor->ct->currentField), 0, 3);
    }
    else
    {
        FuncOnLongPressItem(governorColor);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void ShortPress_SmallButton(void *smallButton)
{
    SButton *sb = (SButton *)smallButton;
    if (sb)
    {
        pFuncVV func = sb->funcOnPress;
        if (func)
        {
            func();
            itemUnderKey = smallButton;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pFuncVpV FuncForShortPressOnItem(void *item)
{
    static const pFuncVpV shortFunction[Item_NumberItems] =
    {
        EmptyFuncVpV,               // Item_None
        ShortPress_Choice,          // Item_Choice
        ShortPress_Button,          // Item_Button
        ShortPress_Page,            // Item_Page
        ShortPress_Governor,        // Item_Governor
        ShortPress_Time,            // Item_Time
        ShortPress_IP,              // Item_IP
        ShortPress_GovernorColor,   // Item_GovernorColor
        EmptyFuncVpV,               // Item_Formula
        ShortPress_MAC,             // Item_MAC
        ShortPress_ChoiceReg,       // Item_ChoiceReg
        ShortPress_SmallButton      // Item_SmallButton
    };

    if(!item)
    {
        return EmptyFuncVpV;
    }

    return shortFunction[((Control *)item)->Type()];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pFuncVpV FuncForLongPressureOnItem(void *item)
{
    static const pFuncVpV longFunction[Item_NumberItems] =
    {
        EmptyFuncVpV,               // Item_None
        FuncOnLongPressItem,        // Item_Choice
        FuncOnLongPressItemButton,  // Item_Button
        FuncOnLongPressItem,        // Item_Page
        FuncOnLongPressItem,        // Item_Governor
        FuncOnLongPressItemTime,    // Item_Time
        FuncOnLongPressItem,        // Item_IP
        FuncOnLongPressItem,        // Item_GovernorColor
        EmptyFuncVpV,               // Item_Formula
        FuncOnLongPressItem,        // Item_MAC
        FuncOnLongPressItem,        // Item_ChoiceReg
        ShortPress_SmallButton      // Item_SmallButton
    };

    if(!item)
    {
        return EmptyFuncVpV;
    }

    Control *control = (Control *)item;

    return control->IsActive() ? longFunction[control->Type()] : EmptyFuncVpV;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ChangeStateFlashDrive()
{
    if(!FDRIVE_IS_CONNECTED)
    {
        if(GetNameOpenedPage() == PageSB_Memory_Drive_Manager)
        {
            ((Page *)OpenedItem())->ShortPressOnItem(0);
        }
    }
    else if(FLASH_AUTOCONNECT)
    {
        OnPress_Drive_Manager();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::OpenItemTime(void)
{
    Display::ShowWarning(TimeNotSet);
    ShortPressureButton(B_Service);
    UpdateInput();
    Display::Update();
    for (int i = 0; i < 2; i++)
    {
        RotateRegSetRight();
        UpdateInput();
        Display::Update();
    }
    ShortPressureButton(B_F4);
    UpdateInput();
    Display::Update();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
/// ���������� true, ���� �������� ��������� ������ ������
static bool NeedForFireSetLED(void)
{
    if (!MENU_IS_SHOWN)
    {
        TypeItem type = menu.TypeOpenedItem();
        return (type == Item_ChoiceReg) || (type == Item_Choice) || (type == Item_Governor);
    }

    NamePage name = menu.GetNameOpenedPage();
    if (
            name == PageSB_Debug_SerialNumber   ||
            name == PageSB_Service_FFT_Cursors  || 
            name == PageSB_Measures_Tune        || 
            name == PageSB_Memory_Last          || 
            name == PageSB_Memory_Internal      ||
            IsRegSetActiveOnCursors()           ||
            (name == PageSB_Service_Function && FUNC_MODE_DRAW_IS_ENABLED)
        )
    {
        return true;
    }
    
    TypeItem type = menu.CurrentItem()->Type();
    if (type == Item_Governor    ||
        type == Item_ChoiceReg   ||
        type == Item_GovernorColor)
    {
        return true;
    }

    type = menu.TypeOpenedItem();
    if (type == Item_Choice       ||
        (type == Item_Page && ((const Page *)menu.OpenedItem())->NumSubPages() > 1)
        )
    {
        return true;
    }

    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void SwitchSetLED(void)
{
    static bool first = true;
    static bool prevState = false;  // true - �����, false - �� �����

    bool state = NeedForFireSetLED();

    if (first)
    {
        first = false;
        panel.EnableLEDRegSet(state);
        prevState = state;
    }
    else if (prevState != state)
    {
        panel.EnableLEDRegSet(state);
        prevState = state;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Show(bool show)
{
    MENU_IS_SHOWN = show;
    if (show)
    {
        Menu::TemporaryEnableStrNavi();
    }
    Menu::SetAutoHide(true);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Init(void)
{
    PageDisplay_Init();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::RunAfterUpdate(pFuncVV func)
{
    funcAterUpdate = func;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Control *Menu::OpenedItem(void)
{
    TypeItem type = Item_None;
    return (Control *)RetLastOpened((Page *)&mainPage, &type);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
TypeItem Menu::TypeOpenedItem(void)
{
    return OpenedItem()->Type();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
NamePage Menu::GetNameOpenedPage(void)
{
    return ((const Page *)OpenedItem())->GetNamePage();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void *Menu::RetLastOpened(Page *page, TypeItem *type)
{
    if (CurrentItemIsOpened(page->GetNamePage()))
    {
        int8 posActItem = page->PosCurrentItem();
        void *item = page->Item(posActItem);
        TypeItem typeLocal = page->Item(posActItem)->Type();
        if (typeLocal == Item_Page)
        {
            return RetLastOpened((Page *)item, type);
        }
        else
        {
            return item;
        }
    }
    *type = Item_Page;
    return page;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Control *Menu::CurrentItem(void)
{
    TypeItem type = Item_None;
    void *lastOpened = RetLastOpened((Page *)&mainPage, &type);
    int8 pos = ((const Page *)lastOpened)->PosCurrentItem();
    if (type == Item_Page && pos != 0x7f)
    {
        return ((const Page *)lastOpened)->Item(pos);
    }
    return (Control *)lastOpened;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::CloseOpenedItem(void)
{
    Control *item = OpenedItem();
    if (TypeOpenedItem() == Item_Page)
    {
        if (item->IsPageSB())
        {
            CallFuncOnPressButton(((Page *)item)->SmallButonFromPage(0));
        }
        NamePage name = ((Control *)item)->Keeper()->name;
        SetMenuPosActItem(name, MENU_POS_ACT_ITEM(name) & 0x7f);
        if (item == (Control *)&mainPage)
        {
            menu.Show(false);
        }
    }
    else
    {
        OpenItem(item, false);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ChangeItem(void *item, int delta)
{
    TypeItem type = ((Control *)item)->Type();
    if (type == Item_Choice || type == Item_ChoiceReg)
    {
        ((Choice *)item)->StartChange(delta);
    }
    else if (type == Item_Governor)
    {
        Governor *governor = (Governor*)item;
        if (OpenedItem() != governor)
        {
            governor->StartChange(delta);
        }
        else
        {
            governor->ChangeValue(delta);
        }
    }
    else if (type == Item_GovernorColor)
    {
        ((GovernorColor *)item)->ChangeValue(delta);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Page *Menu::PagePointerFromName(NamePage)
{
    return 0;
}
