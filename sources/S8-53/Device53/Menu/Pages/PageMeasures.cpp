#include "PageMemory.h"
#include "Settings/SettingsTypes.h"
#include "Utils/Measures.h"
#include "Utils/Math.h"
#include "Menu/Pages/Definition.h"
#include "Menu/MenuItems.h"
#include "Utils/CommonFunctions.h"
#include "Settings/SettingsTypes.h"
#include "Settings/Settings.h"
#include "Display/Colors.h"
#include "defines.h"
#include "Display/Painter.h"
#include "stub.h"


/** @addtogroup Menu
 *  @{
 *  @addtogroup PageMeasures
 *  @{
 */

extern const Page pMeasures;
extern const Page mainPage;
extern const Page mspMeasTune;

static CursCntrl    GetMeasuresCursCntrlActive();           // ����� �������� �� �������� ���� ������ ���������� ����������.


void DrawSB_MeasTune_Settings(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, '\x62');
    painter.SetFont(TypeFont_8);
}

void PressSB_MeasTune_Settings()
{
    Measures_ShorPressOnSmallButtonSettings();
}

void DrawSB_MeasTune_Markers(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 2, '\x60');
    painter.SetFont(TypeFont_8);
}

void PressSB_MeasTune_Markers()
{
    Measure_ShortPressOnSmallButonMarker();
}

void RotateRegMeasureSetField(int angle)
{
    CursCntrl cursCntrl = GetMeasuresCursCntrlActive();
    CursActive cursActive = MEAS_CURS_ACTIVE;

    int sign = Sign(angle);

    if (cursCntrl == CursCntrl_1 || cursCntrl == CursCntrl_1_2)
    {
        if (cursActive == CursActive_T)
        {
            MEAS_POS_CUR_T0 -= sign;
            Limitation<int16>(&MEAS_POS_CUR_T0, 0, sMemory_GetNumPoints(false) - 1);
        }
        else
        {
            MEAS_POS_CUR_U0 += sign;
            Limitation<int16>(&MEAS_POS_CUR_U0, 0, 200);
        }
    }
    if (cursCntrl == CursCntrl_2 || cursCntrl == CursCntrl_1_2)
    {
        if (cursActive == CursActive_T)
        {
            MEAS_POS_CUR_T1 -= sign;
            Limitation<int16>(&MEAS_POS_CUR_T1, 0, sMemory_GetNumPoints(false) - 1);
        }
        else
        {
            MEAS_POS_CUR_U1 += sign;
            Limitation<int16>(&MEAS_POS_CUR_U1, 0, 200);
        }
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(   sbMeasTuneSettings,                                                                    //--- ��������� - ��������� - ��������� ---
    "���������", "Setup",
    "��������� ������� ����������� ���������",
    "Allows to choose necessary measurements",
    mspMeasTune, FuncActive, PressSB_MeasTune_Settings, DrawSB_MeasTune_Settings
);

DEF_SMALL_BUTTON(   sbMeasTuneMarkers,                                                                        //--- ��������� - ��������� - ������ ---
    "������", "Marker",
    "��������� ���������� ������� ��� ����������� �������� ���������",
    "Allows to establish markers for visual control of measurements",
    mspMeasTune, FuncActive, PressSB_MeasTune_Markers, DrawSB_MeasTune_Markers
);

static bool IsActiveChoiceMeasuresChannels()
{
    return SHOW_MEASURES;
}

static bool IsActivePageMeasuresFields()
{
    return SHOW_MEASURES;
}

static bool IsActiveChoiceMeasuresSignal()
{
    return SHOW_MEASURES;
}

static bool IsActiveButtonMeasuresTune()
{
    return SHOW_MEASURES;
}

static bool IsActiveButtonMeasuresFieldSet()
{
    return MEAS_FIELD_IS_HAND;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActiveChoiceMeasuresNumber()
{
    return SHOW_MEASURES;
}

DEF_CHOICE_7(       mcMeasuresNumber,                                                                                 //--- ��������� - ���������� ---
    pMeasures,
    NUM_MEASURES, IsActiveChoiceMeasuresNumber, FuncChangedChoice, FuncDraw,
    "����������", "Number"
    ,
    "������������� ������������ ���������� ��������� ���������:\n"
    "\"1\" - ���� ���������\n"
    "\"2\" - ��� ���������\n"
    "\"1�5\" - 1 ������ � ����� �����������\n"
    "\"2�5\" - 2 ������ � ����� ����������� � ������\n"
    "\"3�5\" - 3 ������ � ����� ����������� � ������\n"
    "\"6x1\" - 6 ����� �� ������ ��������� � ������\n"
    "\"6�2\" - 6 ����� �� ��� ��������� � ������"
    ,
    "Sets the maximum number of output measurements:\n"
    "\"1\" - one measurement\n"
    "\"2\" - two measurements\n"
    "\"1x5\" - 1 line with the five dimensions\n"
    "\"2x5\" - two rows with five measurements in each\n"
    "\"3x5\" - 3 lines with five measurements in each"
    "\"6x1\" - 6 lines, one in each dimension\n"
    "\"6x2\" - 6 lines of two dimensions in each\n"
    ,
    "1",   "1",
    "2",   "2",
    "1x5", "1x5",
    "2x5", "2x5",
    "3x5", "3x5",
    "6x1", "6x1",
    "6x2", "6x2"
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(   mcMeasuresChannels,                                                                                       //--- ��������� - ������ ---
    pMeasures,
    SOURCE_MEASURE, IsActiveChoiceMeasuresNumber, FuncChangedChoice, FuncDraw,
    "������", "Channels",
    "�� ����� ������� �������� ���������",
    "Which channels to output measurement",
    "1",     "1",
    "2",     "2",
    "1 � 2", "1 and 2"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcMeasuresIsShow,                                                                                 //--- ��������� - ���������� ---
    pMeasures, SHOW_MEASURES, FuncActive, FuncChangedChoice, FuncDraw,
    "����������", "Show",
    "�������� ��� �� �������� ��������� �� �����",
    "Output or output measurements on screen",
    "���", "No",
    "��",  "Yes"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcMeasuresSignal,                                                                                        //--- ��������� - ��� ---
    pMeasures,
    MODE_VIEW_SIGNALS, IsActiveChoiceMeasuresSignal, FuncChangedChoice, FuncDraw,
    "���", "View",
    "��������� ��� ��� ���� ������ ������� ��� ���������� ���������� ��� ���������� ���������",
    "Decrease or no zone output signal to avoid overlapping of its measurement results",
    "��� ����",  "As is",
    "���������", "Reduce"
);

/*
// ��������� -> ���� -> �������
const Choice mcMeasuresFieldType =
{
    Item_Choice, &mspMeasuresField, 0,
    {
        "�������", "Field",
        "����� �������, �� ������� ����� ������� �������� ��� �������������� ���������",
        "Select an area from which the values will be taken for automatic measurements"
    },
    {   
        {"�����",   "Screen"},
        {"������",  "Memorye"},
        {"�������", "Cursors"}
    },
    (int8*)&MEAS_FIELD
};
*/

CursCntrl GetMeasuresCursCntrlActive()
{
    if(MEAS_CURS_ACTIVE_IS_T)
    {
        return MEAS_CURS_CNTRL_T;
    }
    return MEAS_CURS_CNTRL_U;
}

static void PressSB_MeasTune_Exit()
{
    display.RemoveAddDrawFunction();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON_EXIT(  sbExitMeasTune,                                                                        //--- ��������� - ��������� - ����� ---
    mspMeasTune, FuncActive, PressSB_MeasTune_Exit, DrawSB_Exit
);


// ��������� - ��������� ///////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_SB(    mspMeasTune,    static,
    Page_SB_MeasTuneMeas, &pMeasures, IsActiveButtonMeasuresTune,
    EmptyPressPage, FuncDrawPage, Measure_RotateRegSet,
    "���������", "CONFIGURE",
    "������� � ����� ������ ��������� ���������� � ����� ���������",
    "Transition to rezhm of exact control of quantity and types of measurements",
    &sbExitMeasTune,
    0,
    0,
    0,
    &sbMeasTuneMarkers,
    &sbMeasTuneSettings
);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5(         pMeasures,                                                                                                        // ��������� ///
    ,
    Page_Measures, &mainPage, FuncActive, EmptyPressPage,
    "���������", "MEASURES",
    "�������������� ���������",
    "Automatic measurements",
    mcMeasuresIsShow,
    mcMeasuresNumber,
    mcMeasuresChannels,
    mcMeasuresSignal,
    mspMeasTune
);

/** @}  @}
 */
