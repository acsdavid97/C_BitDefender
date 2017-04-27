#define _CRT_SECURE_NO_WARNINGS
/*
author: Acs David
file name: CeasarCipher.cpp
file creation date: 2017-03-13 18:49:20
*/

#include "CaesarCipher.h"

size_t* CreateHistogramOfString(TCHAR* string) 
{
	size_t* histogram = (size_t*)calloc(26, sizeof(size_t));
	if (histogram == NULL) 
	{
		return NULL;
	}

	for (size_t i = 0; i < _tcslen(string); i++) 
	{
		if (_istalpha(string[i]))
		{
			histogram[(size_t)_totupper(string[i]) - _T('A')]++;
		}
	}

	return histogram;
}

size_t** CreateDigramHistorgram(TCHAR* string)
{
	size_t** histogram = (size_t**)calloc(26, sizeof(size_t*));
	if (histogram == NULL)
	{
		return NULL;
	}
	for (size_t i = 0; i < 26; i++)
	{
		histogram[i] = (size_t*)calloc(26, sizeof(size_t));
		if (histogram[i] == NULL)
		{
			free(histogram);
			return NULL;
		}
	}

	for (size_t i = 0; i < _tcslen(string) - 1; i++) 
	{
		if (_istalpha(string[i]) && _istalpha(string[i + 1]))
		{
			histogram[(size_t)_totupper(string[i]) - _T('A')][(size_t)_totupper(string[i + 1]) - _T('A')]++;
		}
	}

	return histogram;
}

size_t*** CreateTrigramHistorgram(TCHAR* string)
{
	size_t*** histogram = (size_t***)calloc(26, sizeof(size_t**));
	if (histogram == NULL)
	{
		return NULL;
	}
	for (size_t i = 0; i < 26; i++)
	{
		histogram[i] = (size_t**)calloc(26, sizeof(size_t*));
		if (histogram[i] == NULL)
		{
			free(histogram);
			return NULL;
		}
		for (size_t j = 0; j < 26; j++)
		{
			histogram[i][j] = (size_t*)calloc(26, sizeof(size_t));
			if (histogram[i][j] == NULL)
			{
				free(histogram[i]);
				free(histogram);
				return NULL;
			}
		}
	}

	for (size_t i = 0; i < _tcslen(string) - 2; i++) 
	{
		if (_istalpha(string[i]) && _istalpha(string[i + 1]) && _istalpha(string[i + 2]))
		{
			histogram[(size_t)_totupper(string[i]) - _T('A')]
				[(size_t)_totupper(string[i + 1]) - _T('A')]
			[(size_t)_totupper(string[i + 2]) - _T('A')]++;
		}
	}

	return histogram;
}

TCHAR inline EncryptTCHAR(TCHAR tchar, INT k) 
{
	if (_istalpha(tchar)) 
	{
		tchar = _totupper(tchar);
		int code = (tchar - _T('A') + k) % 26;
		//need to check to not be below 'A'
		code = code >= 0 ? code : 26 + code; //correcting if negative 
		return code + _T('A');
	}

	//return original tchar
	return tchar;
}

void EncryptString(TCHAR* string, INT k) 
{
	for (size_t i = 0; i < _tcsclen(string); i++) 
	{
		string[i] = EncryptTCHAR(string[i], k);
	}
}

void CrackCaeserCypher(TCHAR* string, TCHAR* frequencyString)
{
	size_t* histogram = CreateHistogramOfString(string);
	if (histogram == NULL) 
	{
		printf_s("allocation failed");
		exit(EXIT_FAILURE);
	}

	size_t maxIndex = 0;
	for (size_t i = 0; i < 26; i++) 
	{
		if (histogram[i] > histogram[maxIndex])
		{
			maxIndex = i;
		}
	}

	TCHAR mostFrequentChar = (TCHAR)(maxIndex + _T('A'));

	TCHAR* decrypted = (TCHAR*)malloc(sizeof(TCHAR) * (_tcsclen(string) + 1));
		
	for (size_t i = 0; i < _tcsclen(frequencyString); i++)
	{
		_tcscpy(decrypted, string);
		INT difference = frequencyString[i] - mostFrequentChar;
		EncryptString(decrypted, difference);
		_tprintf(_T("Is the following string readable?: %s (y to exit, anything else to continue)\n"), decrypted);
		char response = getchar();

		if (response == 'y' || response == 'Y')
		{
			_tprintf(_T("succesfully cracked caesar cypher\n"));
			return;
		}
	}
}
