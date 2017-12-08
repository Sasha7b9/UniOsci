#pragma once 


/// Вызывается один раз при обнаружении новой флешки.
void FM_Init();

void FM_Draw();

void PressSB_FM_LevelUp();

void PressSB_FM_LevelDown();

void FM_RotateRegSet(int angle);

bool FM_GetNameForNewFile(char name[255]);

void PressSB_FM_Tab();
