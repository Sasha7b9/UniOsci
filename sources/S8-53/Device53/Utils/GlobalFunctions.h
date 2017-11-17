#pragma once
#include "defines.h"

                                                
                   
char*   Bin2String                              //  ����������� value � ��������� ������. ��� ���� ��������� ��� 8 ��������.
                                                //  @attention ������ ����� ��������� �� ���������� ������ �������. ���� ��������� ����� ������� ���������� �������, �� ��� ����� ����������� ����.
                    (uint8 value,
                    char buffer[9]);
float   CircleAddFloat(float *val, float delta, float min, float max);  // ����������� �������� �� ������ val �� delta. �����, ���� ��������� ������ max, ������������ ��� min.
float   CircleSubFloat(float *val, float delta, float min, float max);  // ��������� �������� �� ������ val �� delta. �����, ���� ��������� ������ min, ������������ ��� masx.
void    AddLimitationFloat                                              // ��������� � �������� �� ������ val delta. ���� ��������� �� ����������� � �������� [min; max], ����������.
                            (float *val,                                // �� ����� ������ �������� ��������.
                            float delta,                                // � ��� ������.
                            float min,                                  // ������ ������� ���������.
                            float max                                   // ������� ������� ���������.
                            );
char    GetSymbolForGovernor(int value);                        // ���������� ����������� ����������, ��������������� ��� �������� ���������.
void    EmptyFuncVV();                                      // �������-��������. ������ �� ������.
void    EmptyFuncVI(int);
void    EmptyFuncVpV(void *);                                   // �������-��������. ������ �� ������.
void    EmptyFuncpVII(void *, int, int);                        // �������-��������. ������ �� ������.
void    EmptyFuncVI16(int16);                                   // �������-��������. ������ �� ������.
void    EmptyFuncVB(bool);                                      // �������-��������. ������ �� ������.
void    IntToStrCat(char *_buffer, int _value);
