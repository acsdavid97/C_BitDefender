/*
author: Acs David
file name: caesarCipher.h
file creation date: 2016-11-20 23:26:24
*/

#ifndef caesarCipher_H
#define caesarCipher_H

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <Windows.h>
#include <string.h>
#include <math.h>

// structs

// define your functions here

size_t* CreateHistogramOfString(TCHAR* string);
size_t** CreateDigramHistorgram(TCHAR* string);
size_t*** CreateTrigramHistorgram(TCHAR* string);
void EncryptString(TCHAR* string, INT k);
void CrackCaeserCypher(TCHAR* string, TCHAR* frequencyString);

#endif
