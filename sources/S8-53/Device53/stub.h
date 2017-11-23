#pragma once
#include "Hardware/Timer.h"

void Timer_SetAndStartOnce(TypeTimer type, pFuncVV func, uint dTms);


#define FPGA_SetTShift(x)                   fpga.SetTShift(x)
#define FPGA_SetTrigLev(x, y)               fpga.SetTrigLev(x, y)
#define FPGA_SetRShift(x, y)                fpga.SetRShift(x, y)
#define FPGA_OnPressStartStop()             fpga.OnPressStartStop()
#define FDrive_OpenNewFileForWrite(x, y)    flashDrive.OpenNewFileForWrite(x, y)
#define FDrive_WriteToFile(x, y, z)         flashDrive.WriteToFile(x, y, z)
#define FDrive_CloseFile(x)                 flashDrive.CloseFile(x)
#define FSMC_SET_MODE(x)
#define FSMC_RESTORE_MODE()
#define SHOW_STRING_NAVI_TEMP               false
#define Timer_SetAndEnable(x, y, z)
#define Timer_SetAndStartOnce(x, y, z)
#define IsRegSetActiveOnCursors()           true
#define FUNC_MODE_DRAW_IS_ENABLED           true
#define PageDisplay_Init()
#define COLOR_SCHEME_IS_WHITE_LETTERS       true
#define SHOW_STRING_NAVI_TEMP               false
#define PageService_InitGlobalColors()
