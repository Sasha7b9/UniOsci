#pragma once 
#include "defines.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Menu
 *  @{
 *  @defgroup FileManager File Manager
 *  @{
 */

/// Вызывается один раз при обнаружении новой флешки
void FM_Init();
/// Нарисовать файловый менеджер
void FM_Draw();

void PressSB_FM_LevelUp();

void PressSB_FM_LevelDown();

void FM_RotateRegSet(int angle);

bool FM_GetNameForNewFile(char name[255]);

/** @}
 */
