#pragma once


float   CircleAddFloat(float *val, float delta, float min, float max);  // ����������� �������� �� ������ val �� delta. �����, ���� ��������� ������ max, ������������ ��� min.
float   CircleSubFloat(float *val, float delta, float min, float max);  // ��������� �������� �� ������ val �� delta. �����, ���� ��������� ������ min, ������������ ��� masx.
void    AddLimitationFloat                                              // ��������� � �������� �� ������ val delta. ���� ��������� �� ����������� � �������� [min; max], ����������.
(float *val,                                // �� ����� ������ �������� ��������.
float delta,                                // � ��� ������.
float min,                                  // ������ ������� ���������.
float max                                   // ������� ������� ���������.
);
char    GetSymbolForGovernor(int value);                        // ���������� ����������� ����������, ��������������� ��� �������� ���������.
void    EmptyFuncVV(void);                                      // �������-��������. ������ �� ������.
void    EmptyFuncVI(int);
void    EmptyFuncVpV(void *);                                   // �������-��������. ������ �� ������.
bool    EmptyFuncBV(void);                                      // �������-��������. ������ �� ������.
void    EmptyFuncpVII(void *, int, int);                        // �������-��������. ������ �� ������.
void    EmptyFuncVI16(int16);                                   // �������-��������. ������ �� ������.
void    EmptyFuncVB(bool);                                      // �������-��������. ������ �� ������.
char*   IntToStrCat(char *_buffer, int _value);
