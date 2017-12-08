#include "Log.h"
#include "Menu.h" 
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
extern void OnChanged_InputA(bool active);
extern void OnChanged_InputB(bool active);
extern void Long_Help();
extern const PageBase * const pointerPageHelp;
extern void *itemUnderButton[B_NumButtons];

/// Если произошло короткое нажатие кнопки, то здесь хранится имя этой кнопки до обработки  этого нажатия.
static PanelButton shortPressureButton = B_Empty;
/// Если произошло длинное нажатие кнопки, то здесь хранится имя этой кнопки до обработки этого нажатия.
static PanelButton longPressureButton = B_Empty;
/// При нажатии кнопки её имя записывается в эту переменную и хранится там до обратоки события нажатия кнопки.
static PanelButton pressButton = B_Empty;
/// При отпускании кнопки её имя записывается в эту переменную и хранится там до обработки события отпускания кнопки.
static PanelButton releaseButton = B_Empty;
static PanelRegulator pressRegulator = R_Empty;
/// Угол, на который нужно повернуть ручку УСТАНОВКА - величина означает количество щелчков, знак - направление - "-" - влево, "+" - вправо.
static int angleRegSet = 0;
///\brief  Здесь хранится адрес элемента меню, соответствующего функциональной клавише [1..5], если она находится в нижнем положении, и 0, если ни одна 
/// кнопка не нажата.
static void *itemUnderKey = 0;
/// Эта функция будет вызывана один раз после Menu::UpdateInput().
static pFuncVV funcAterUpdate = 0;
                                                
static void ProcessingShortPressureButton();            ///< Обработка короткого нажатия кнопки.
static void ProcessingLongPressureButton();             ///< Обработка длинного нажатия кнопки.
static void ProcessingPressButton();                    ///< Обработка опускания кнопки вниз.
static void ProcessingReleaseButton();                  ///< Обработка поднятия кнопки вверх.
static void ProcessingRegulatorSetRotate();             ///< Обработка поворота ручки УСТАНОВКА.
static void ProcessingRegulatorPress();                 ///< Обработка нажатия ручки.
static void OnTimerAutoHide();                          ///< Обработка события таймера автоматического сокрытия меню.
static void SwitchSetLED();                             ///< Включить/выключить светодиод ручки УСТАНОВКА, если необходимо.
static void ShortPress_Page(void *page);                    ///< Обработка короткого нажатия на элемент NamePage с адресом page.
static void ShortPress_Choice(void *choice);                ///< Обработка короткого нажатия на элемент Choice с адресом choice.
static void ShortPress_Time(void *time);
static void ShortPress_Button(void *button);                ///< Обработка короткого нажатия на элемент Button с адресом button.
static void ShortPress_Governor(void *governor);            ///< Обработка короткого нажатия на элемент Governor с адресом governor.
static void ShortPress_GovernorColor(void *governorColor);  ///< Обработка короткого нажатия на элемент GovernorColor с адресом governorColor.
static void FuncOnLongPressItem(void *item);                ///< Обработка длинного нажатия на элемент меню item.
static void FuncOnLongPressItemTime(void *item);
static void FuncOnLongPressItemButton(void *button);        ///< Обработка длинного нажатия на элемент Button с адресом button.

static  pFuncVpV    FuncForShortPressOnItem(void *item);    ///< Возвращает функцию обработки короткого нажатия на элемент меню item.
static  pFuncVpV    FuncForLongPressureOnItem(void *item);  ///< Возвращает функцию обработки длинного нажатия на элемент меню item.

static void OnTimerStrNaviAutoHide();                   ///< Функция, которая отключит вывод строки навигации меню.


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIZE_BUFFER_FOR_BUTTONS 10
static PanelButton bufferForButtons[SIZE_BUFFER_FOR_BUTTONS] = {B_Empty};
static const PanelButton sampleBufferForButtons[SIZE_BUFFER_FOR_BUTTONS] = {B_F5, B_F5, B_F4, B_F4, B_F3, B_F3, B_F2, B_F2, B_F1, B_F1};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::UpdateInput()
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
            "Кнопка МЕНЮ выполняет следующие функции:\n"
            "1. При закрытом меню нажатие либо нажатие с удержанием в течение 0.5с открывает меню.\n"
            "2. При открытом меню удержание кнопки в течение 0.5с закрывает меню.\n"
            "3. При настройке \"СЕРВИС\x99Режим кн МЕНЮ\x99Закрывать\" текущей становится страница предыдущего уровня меню. Если текущей является "
            "корневая страница, меню закрывается.\n"
            "4. При настройке \"СЕРВИС\x99Режим кн МЕНЮ\x99Переключать\" текущей становится страница текущего уровня меню. Если текущая страница "
            "последняя в текущем уровне, происходит переход на предыдущий уровень меню.\n"
            "5. Если меню находится в режиме малых кнопок, то нажатие закрывает страницу."
            :
        "МЕНЮ button performs the following functions:\n"
            "1. At the closed menu pressing or pressing with deduction during 0.5s opens the Menu::\n"
            "2. At the open menu deduction of the button during 0.5s closes the Menu::\n"
#ifndef WIN32
#pragma push
#pragma diag_suppress 192
            "3. At control \"SERVICE\x99Mode btn MENU\x99\Close\" current becomes the page of the previous level of the Menu:: If the root page is "
            "current, the menu is closed.\n"
#pragma pop
#endif
            "4. At control \"SERVICE\x99Mode btn MENU\x99Toggle\" current becomes the page of the current level of the Menu:: If the current page the "
            "last in the current level, happens transition to the previous level of the Menu::\n"
            "5. If the menu is in the mode of small buttons, pressing closes the page.";

    } else if (button == B_Cursors)
    {
        gStringForHint = LANG_RU ? 
            "Кнопка КУРСОРЫ открывает меню курсорных измерений."
            :
            "КУРСОРЫ button to open the menu cursor measurements.";
    }
    else if (button == B_Display)
    {
        gStringForHint = LANG_RU ?
            "Кнопка ДИСПЛЕЙ открывает меню настроек дисплея."
            :
            "ДИСПЛЕЙ button opens the display settings Menu::";
    }
    else if (button == B_Memory)
    {
        gStringForHint = LANG_RU ?
            "1. При настройке \"ПАМЯТЬ\x99ВНЕШН ЗУ\x99Реж кн ПАМЯТЬ\x99Меню\" открывает меню работы с памятью.\n"
            "2. При настройке \"ПАМЯТь\x99ВНЕШН ЗУ\x99Реж кн ПАМЯТЬ\x99Сохранение\" сохраняет сигнал на флеш-диск."
            :
            "1. When setting \"ПАМЯТЬ-EXT\x99STORAGE\x99Mode btn MEMORY\x99Menu\" opens a menu of memory\n"
            "2. When setting \"ПАМЯТЬ-EXT\x99STORAGE\x99Mode btn MEMORY\x99Save\" saves the signal to the flash drive";
    }
    else if (button == B_Measures)
    {
        gStringForHint = LANG_RU ?
            "Кнопка ИЗМЕР открывает меню автоматических измерений."
            :
            "ИЗМЕР button opens a menu of automatic measurements.";
    }
    else if (button == B_Help)
    {
        gStringForHint = LANG_RU ?
            "1. Кнопка ПОМОЩЬ открывает меню помощи.\n"
            "2. Нажатие и удержание кнопки ПОМОЩЬ в течение 0.5с включает и отключает режим вывода подсказок."
            :
            "1. ПОМОЩЬ button opens the help Menu::\n"
            "2. Pressing and holding the ПОМОЩЬ for 0.5s enables and disables output mode hints.";
    }
    else if (button == B_Service)
    {
        gStringForHint = LANG_RU ?
            "Кнопка СЕРВИС открывает меню сервисных возможностей."
            :
            "СЕРВИС button opens a menu of service options.";
    }
    else if (button == B_Start)
    {
        gStringForHint = LANG_RU ?
            "Кнопка ПУСК/СTOП запускает и останавливает процесс сбора информации."
            :
            "ПУСК/СTOП button starts and stops the process of gathering information.";
    }
    else if (button == B_ChannelA)
    {
        gStringForHint = LANG_RU ?
            "1. Кнопка КАНАЛ1 открывает меню настроек канала 1.\n"
            "2. Нажатие и удержание кнопки КАНАЛ1 в течение 0.5с устанавливает смещение канала 1 по вертикали 0В."
            :
            "1. КАНАЛ1 button opens the settings menu of the channel 1.\n"
            "2. Pressing and holding the button КАНАЛ1 for 0.5c for the offset of the vertical channel 1 0V.";
    }
    else if (button == B_ChannelB)
    {
        gStringForHint = LANG_RU ?
            "1. Кнопка КАНАЛ2 открывает меню настроек канала 2.\n"
            "2. Нажатие и удержание кнопки КАНАЛ2 в течение 0.5с устанавливает смещение канала 2 по вертикали 0В."
            :
            "1. КАНАЛ2 button opens the settings menu of the channel 2.\n"
            "2. Pressing and holding the button КАНАЛ2 for 0.5c for the offset of the vertical channel 2 0V.";
    }
    else if (button == B_Time)
    {
        gStringForHint = LANG_RU ?
            "1. Кнопка РАЗВ открывает меню настроек развертки.\n"
            "2. Нажатие и удержание кнопки РАЗВ в течение 0.5с устанавливает смещение по горизонтали 0с."
            :
            "1. РАЗВ button open the settings menu sweep.\n"
            "2. Pressing and holding the button РАЗВ for 0.5s Is the offset horizontal 0s.";
    }
    else if (button == B_Trig)
    {
        gStringForHint = LANG_RU ?
            "1. Кнопка СИНХР открывает меню настроек синхронизации.\n"
            "2. Нажатие и удержание в течение 0.5с кнопки СИНХР при настройке \"СЕРВИС\x99Реж длит СИНХР\x99Автоуровень\" производит автоматическую "
            "настройку уровня синхронизации.\n"
            "3. Нажатие и удержание в течение 0.5с кнопки СИНХР при настройке \"СЕРВИС\x99Реж длит СИНХР\x99Сброс уровня\" устанавливает уровень "
            "синхронизации 0В."
            :
            "1. СИНХР button opens a menu settings synchronization.\n"
#ifndef WIN32
#pragma push
#pragma diag_suppress 192
            "2. Pressing and holding the button СИНХР for 0.5s when setting \"SERVICE\x99Mode long TRIG\x99\Autolevel\" automatically adjust the "
            "trigger level.\n"
#pragma pop
#endif
            "3. Pressing and holding the button СИНХР for 0.5s when setting \"SERVICE\x99Mode long TRIG\x99SReset trig level\" sets the trigger "
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
    Sound::ButtonPress();
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
    Sound::ButtonRelease();
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
void Menu::RotateRegSetRight()
{   
    if (!HINT_MODE_ENABLE)
    {
        angleRegSet++;
        NEED_FINISH_DRAW = 1;
    }
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::RotateRegSetLeft()
{
    if (!HINT_MODE_ENABLE)
    {
        angleRegSet--;
        NEED_FINISH_DRAW = 1;
    }
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
void *Menu::ItemUnderKey()
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
    Control *item = OpenedItem();
    if(IsMainPage(item))
    {
        return 0;
    }
    while(!IsMainPage(item))
    {
        titles[numTitle++] = item->Title();
        item = (Control *)item->keeper;
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
void OnTimerAutoHide()
{
    Menu::Show(false);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void ProcessingShortPressureButton()
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
        Menu::SetAutoHide(true);

        PanelButton button = shortPressureButton;

        do
        {
            if (button == B_Help)
            {
                Long_Help();
            }
            else if(button == B_Menu)                                   // Если нажата кнопка МЕНЮ и мы не находимся в режме настройки измерений.
            {
                if(!MENU_IS_SHOWN)
                {
                    Menu::Show(true);
                }
                else
                {
                    if (Menu::TypeOpenedItem() == Item_Page)
                    {
                        Menu::TemporaryEnableStrNavi();
                    }
                    Menu::CloseOpenedItem();
                }
            }
            else if (MENU_IS_SHOWN && Panel::IsFunctionalButton(button))       // Если меню показано и нажата функциональная клавиша
            {
                void *item = itemUnderButton[button];
                if (HINT_MODE_ENABLE)
                {
                    SetItemForHint(item);
                }
                else
                {
                    FuncForShortPressOnItem(item)(item);
                }
            }
            else                                                        // Если меню не показано.
            {
                NamePage name = ((const Page *)Menu::OpenedItem())->GetNamePage();
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

                Page *page = (Page *)PageForButton(button);
                if(page && page != (Page *)pointerPageHelp)
                {
                    page->SetCurrent(true);
                    page->Open(true);
                    Menu::TemporaryEnableStrNavi();
                    Menu::Show(true);
                }
            }
        } while(false);

        shortPressureButton = B_Empty;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ProcessingLongPressureButton()
{
    PanelButton button = longPressureButton;

    if(button != B_Empty)
    {
        Sound::ButtonRelease();
        NEED_FINISH_DRAW = 1;
        Menu::SetAutoHide(true);

        if(button == B_Time)
        {
            FPGA::SetTShift(0);
        }
        else if(button == B_Trig)
        {
            FPGA::SetTrigLev(TRIGSOURCE, TrigLevZero);
        }
        else if(button == B_ChannelA)
        {
            FPGA::SetRShift(A, RShiftZero);
        }
        else if(button == B_ChannelB)
        {
            FPGA::SetRShift(B, RShiftZero);
        }
        else if(button == B_Menu)
        {
            if (Menu::OpenedItem()->IsPageSB())
            {
                Menu::CloseOpenedItem();
            }
            else
            {
                Menu::Show(!MENU_IS_SHOWN);
                if (Menu::TypeOpenedItem() != Item_Page)
                {
                    Menu::TemporaryEnableStrNavi();
                }
            }
        }
        else if(MENU_IS_SHOWN && Panel::IsFunctionalButton(button))
        {
            void *item = itemUnderButton[button];
            FuncForLongPressureOnItem(item)(item);
            if (Menu::TypeOpenedItem() != Item_Page)
            {
                Menu::TemporaryEnableStrNavi();
            }
        }
        longPressureButton = B_Empty;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void ProcessingRegulatorPress()
{
    if (pressRegulator != R_Empty)
    {
        Menu::SetAutoHide(true);
        if (pressRegulator == R_Set)
        {
            Menu::Show(!MENU_IS_SHOWN);
            if (Menu::TypeOpenedItem() != Item_Page)
            {
                Menu::TemporaryEnableStrNavi();
            }
        }

        pressRegulator = R_Empty;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ProcessingRegulatorSetRotate()
{
    if (angleRegSet == 0)
    {
        return;
    }

    if (MENU_IS_SHOWN || Menu::TypeOpenedItem() != Item_Page)
    {
        Control *item = Menu::CurrentItem();
        TypeItem type = item->Type();
        static const int step = 2;
        if (Menu::TypeOpenedItem() == Item_Page && (type == Item_ChoiceReg || type == Item_Governor || type == Item_IP || type == Item_MAC))
        {
            if (angleRegSet > step || angleRegSet < -step)
            {
                Menu::ChangeItem(item, angleRegSet);
                angleRegSet = 0;
            }
            return;
        }
        else
        {
            item = Menu::OpenedItem();
            type = item->Type();
            if (MenuIsMinimize())
            {
                CurrentPageSBregSet(angleRegSet);
            }
            else if (type == Item_Page || type == Item_IP || type == Item_MAC || type == Item_Choice || type == Item_ChoiceReg || type == Item_Governor)
            {
                if (item->ChangeOpened(angleRegSet))
                {
                    angleRegSet = 0;
                }
                return;
            }
            else if (type == Item_GovernorColor)
            {
                Menu::ChangeItem(item, angleRegSet);
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
void ProcessingPressButton()
{
    if((pressButton >= B_F1 && pressButton <= B_F5) || pressButton == B_Menu)
    {
        if (pressButton != B_Menu)
        {
            itemUnderKey = itemUnderButton[pressButton];
        }
    }
    if (pressButton == B_Start && !MODE_WORK_IS_RAM)
    {
        FPGA::OnPressStartStop();
    }
    pressButton = B_Empty;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ProcessingReleaseButton()
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
void Menu::TemporaryEnableStrNavi()
{
    if (SHOW_STRING_NAVI_TEMP)
    {
        SHOW_STRING_NAVIGATION = 1;                                            // Устанавливаем признак того, что надо выводить строку навигации меню
        Timer::SetAndStartOnce(kStrNaviAutoHide, OnTimerStrNaviAutoHide, 3000); // и запускаем таймер, который его отключит
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
static void OnTimerStrNaviAutoHide()
{
    SHOW_STRING_NAVIGATION = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ShortPress_Choice(void *item)
{
    Choice *choice = (Choice*)item;

    if (!choice->IsActive())
    {
        CHOICE_RUN_FUNC_CHANGED(choice, false);
    }
    else if (!choice->IsOpened())
    {
        choice->SetCurrent(Menu::CurrentItem() != choice);
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
    else if(Menu::OpenedItem() != choice) 
    {
        choice->SetCurrent(Menu::CurrentItem() != choice);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FuncOnLongPressItemButton(void *button)
{
    ShortPress_Button(button);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ShortPress_Button(void *item)
{
    Button *button = (Button *)item;
    if(!button->IsActive())
    {
        return;
    }
    button->SetCurrent(true);
    CallFuncOnPressButton(button);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FuncOnLongPressItem(void *item)
{
    Control *control = (Control *)item;
    if (Menu::CurrentItem() != control)
    {
        control->SetCurrent(true);
    }
    control->Open(!control->IsOpened());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FuncOnLongPressItemTime(void *item)
{
    Time *time = (Time *)item;
    if (Menu::CurrentItem() != time)
    {
        time->SetCurrent(true);
    }
    if(time->IsOpened() && (*time->curField == iSET))
    {
        time->SetNewTime();
    }
    time->Open(!time->IsOpened());
    time->SetOpened();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ShortPress_Time(void *item)
{
    Time *time = (Time *)item;
    if(!time->IsOpened())
    {
        time->SetCurrent(true);
        time->SetOpened();
        time->Open(true);
    }
    else
    {
        ((Time *)time)->SelectNextPosition();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ShortPress_Governor(void *governor)
{
    Governor *gov = (Governor *)governor;
    if(!gov->IsActive())
    {
        return;
    }
    if(Menu::OpenedItem() == gov)
    {
        gov->NextPosition();
    }
    else
    {
        gov->SetCurrent(Menu::CurrentItem() != gov);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ShortPress_IP(void *item)
{
    if (Menu::OpenedItem() == item)
    {
        ((IPaddress*)item)->NextPosition();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ShortPress_MAC(void *item)
{
    if (Menu::OpenedItem() == item)
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
    if(Menu::OpenedItem() == governor)
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
void Menu::OpenItemTime()
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
/// Возвращает true, если лампочка УСТАНОВКА должна гореть
static bool NeedForFireSetLED()
{
    if (!MENU_IS_SHOWN)
    {
        TypeItem type = Menu::TypeOpenedItem();
        return (type == Item_ChoiceReg) || (type == Item_Choice) || (type == Item_Governor);
    }

    NamePage name = Menu::GetNameOpenedPage();
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
    
    TypeItem type = Menu::CurrentItem()->Type();
    if (type == Item_Governor    ||
        type == Item_ChoiceReg   ||
        type == Item_GovernorColor)
    {
        return true;
    }

    type = Menu::TypeOpenedItem();
    if (type == Item_Choice       ||
        (type == Item_Page && ((const Page *)Menu::OpenedItem())->NumSubPages() > 1)
        )
    {
        return true;
    }

    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void SwitchSetLED()
{
    static bool first = true;
    static bool prevState = false;  // true - горит, false - не горит

    bool state = NeedForFireSetLED();

    if (first)
    {
        first = false;
        Panel::EnableLEDRegSet(state);
        prevState = state;
    }
    else if (prevState != state)
    {
        Panel::EnableLEDRegSet(state);
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
void Menu::Init()
{
    PageDisplay_Init();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::RunAfterUpdate(pFuncVV func)
{
    funcAterUpdate = func;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Control *Menu::OpenedItem()
{
    TypeItem type = Item_None;
    return (Control *)RetLastOpened((Page *)&mainPage, &type);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
TypeItem Menu::TypeOpenedItem()
{
    return OpenedItem()->Type();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
NamePage Menu::GetNameOpenedPage()
{
    return ((const Page *)OpenedItem())->GetNamePage();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void *Menu::RetLastOpened(Page *page, TypeItem *type)
{
    if (page->CurrentItemIsOpened())
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
Control *Menu::CurrentItem()
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
void Menu::CloseOpenedItem()
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
            Menu::Show(false);
        }
    }
    else
    {
        item->Open(false);
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
