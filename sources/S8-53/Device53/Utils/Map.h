#pragma once


#include "Strings.h"


typedef struct
{
    char *key;
    uint8 value;
} MapElement;


uint8 GetValueFromMap(const MapElement *map, Word *key);      // ���� �������� �� �������, ������������ 255;