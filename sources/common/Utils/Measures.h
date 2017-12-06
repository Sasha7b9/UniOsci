#pragma once
#include "defines.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Utils
 *  @{
 *  @defgroup Measures
 *  @{
 */

// Виды измерений
typedef enum
{
    Meas_None,
    Meas_VoltageMax,
    Meas_VoltageMin,
    Meas_VoltagePic,
    Meas_VoltageMaxSteady,
    Meas_VoltageMinSteady,
    Meas_VoltageAmpl,
    Meas_VoltageAverage,
    Meas_VoltageRMS,
    Meas_VoltageVybrosPlus,
    Meas_VoltageVybrosMinus,
    Meas_Period,
    Meas_Freq,
    Meas_TimeNarastaniya,
    Meas_TimeSpada,
    Meas_DurationPlus,
    Meas_DurationMinus,
    Meas_SkvaznostPlus,
    Meas_SkvaznostMinus,
    Meas_DelayPlus,
    Meas_DelayMinus,
    Meas_PhazaPlus,
    Meas_PhazaMinus,
    Meas_NumMeasures
} Meas;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Measures
{
public:
    char GetChar(Meas measure);

    bool IsActive(int row, int col);

    void GetActive(int *row, int *col);

    void SetActive(int row, int col);

    int GetDY();

    int GetDX();

    const char *Name(int row, int col);

    Meas Type(int row, int col);

    int NumRows();

    int NumCols();

    int GetTopTable();

    int GetDeltaGridLeft();

    int GetDeltaGridBottom();

    void ShortPressOnSmallButonMarker();
    /// Нарисовать страницу выбора измерений
    void DrawPageChoice();
};


extern Measures measures;


/** @}  @}
 */
