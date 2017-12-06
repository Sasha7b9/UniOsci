#pragma once
#include "Hardware/Timer.h"


#define FPGA_SetTShift(x)                   fpga.SetTShift(x)
#define FPGA_SetTrigLev(x, y)               fpga.SetTrigLev(x, y)
#define FPGA_SetRShift(x, y)                fpga.SetRShift(x, y)
#define FPGA_OnPressStartStop()             fpga.OnPressStartStop()
#define FSMC_SET_MODE(x)
#define FSMC_RESTORE_MODE()
#define SHOW_STRING_NAVI_TEMP               false
#define IsRegSetActiveOnCursors()           true
#define FUNC_MODE_DRAW_IS_ENABLED           true
#define PageDisplay_Init()
#define COLOR_SCHEME_IS_WHITE_LETTERS       true
#define SHOW_STRING_NAVI_TEMP               false
#define PageService_InitGlobalColors()
#define Measures_ShorPressOnSmallButtonSettings()
#define Measure_ShortPressOnSmallButonMarker()
void Measure_RotateRegSet(int);
