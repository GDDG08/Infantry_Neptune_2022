/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : str_lib.h
 *  Description  : This file contains information about string handling functions
 *  LastEditors  : 动情丶卜灬动心
 *  Date         : 2021-07-11 03:22:31
 *  LastEditTime : 2021-07-11 03:33:52
 */

#ifndef STR_LIB_H
#define STR_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"

char* Str_Itoa(int value, char* string);
int Str_Atoi(const char* str);
void Str_HexToAscii(uint8_t* src, char* dest, int len);

#ifdef __cplusplus
}
#endif

#endif
