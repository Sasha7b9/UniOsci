#pragma once


char*   Hex8toString                            // ����������� value � ��������� ������ � ����������������� ����.
                                                // @attention ������ ����� ��������� �� ���������� ������ �������. ���� ��������� ����� ������� ���������� �������, �� ��� ����� ����������� ����.
                    (uint8 value,
                    char buffer[3],
                    bool upper);
char*   Hex16toString(uint16 value, char buffer[5], bool upper);
char*   Int2String                              // ����������� value � ��������� ������.
                                                // @attention ������ ����� ��������� �� ���������� ������ �������. ���� ��������� ����� ������� ���������� �������, �� ��� ����� ����������� ����.
                    (int value,                 // ��������.
                    bool alwaysSign,            // ���� ���������� � true, ���� ����� ���������� ������.
                    int numMinFields,           // ����������� ����� ���������� ���������. ���� ��� ������ ����� ������� �� ���������, ������ ����������� ������.
                    char buffer[20]
                    );
bool    String2Int(char *str, int *value);
// ����������� time ������ � ��������� ������
// time - ����� � ��������. ����� ���� �������������
// numDigits - ���������� ���� � ������
char*   Time2StringAccuracy(float time, bool alwaysSign, char buffer[20], int numDigits);
char*   Float2Db(float value, int numDigits, char bufferOut[20]);   // ����������� �������� � ��������� ������.

bool    IntInRange                              // ���������� true, ���� value ������ � �������� [min; max].
                   (int value,                  // ����������� ��������.
                   int min,                     // ������ ������� ���������.
                   int max                      // ������� ������� ���������.
                   );

int     BCD2Int(uint bcd);

float   MaxFloat(float val1, float val2, float val3);                   // ���������� ������������ �������� �� ���.
float   CircleAddFloat(float *val, float delta, float min, float max);  // ����������� �������� �� ������ val �� delta. �����, ���� ��������� ������ max, ������������ ��� min.
float   CircleSubFloat(float *val, float delta, float min, float max);  // ��������� �������� �� ������ val �� delta. �����, ���� ��������� ������ min, ������������ ��� masx.
void    AddLimitationFloat                                              // ��������� � �������� �� ������ val delta. ���� ��������� �� ����������� � �������� [min; max], ����������.
                            (float *val,                                // �� ����� ������ �������� ��������.
                            float delta,                                // � ��� ������.
                            float min,                                  // ������ ������� ���������.
                            float max                                   // ������� ������� ���������.
                            );
void    SwapInt(int *value0, int *value1);                      // ���������� ������� ���������� ������ �� ������� value0 � value1.
void    SortInt(int *value0, int *value1);                      // ��������� ���������� value0 � value1 � ������� �����������.
char    GetSymbolForGovernor(int value);                        // ���������� ����������� ����������, ��������������� ��� �������� ���������.
void    EmptyFuncVV(void);                                      // �������-��������. ������ �� ������.
void    EmptyFuncVI(int);
void    EmptyFuncVpV(void *);                                   // �������-��������. ������ �� ������.
void    EmptyFuncVII(int, int);                                 // �������-��������. ������ �� ������.
void    EmptyFuncpVII(void *, int, int);                        // �������-��������. ������ �� ������.
void    EmptyFuncVI16(int16);                                   // �������-��������. ������ �� ������.
void    EmptyFuncVB(bool);                                      // �������-��������. ������ �� ������.
char*   IntToStrCat(char *_buffer, int _value);
