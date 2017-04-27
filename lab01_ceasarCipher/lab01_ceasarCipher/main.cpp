/*
	author: Acs David
	file name: main.cpp
	file creation date: 2017-03-12 20:46:28
*/

#include "caesarCipher.h"


int main()
{
	TCHAR* inputString = (TCHAR*)malloc(sizeof(TCHAR) * 1024);
	_tprintf(_T("Enter the string to be encrypted:\n"));
	_fgetts(inputString, 1024, stdin);

	size_t* histogram = CreateHistogramOfString(inputString);
	_tprintf(_T("histogram: \n"));
	for (size_t i = 0; i < 26; i++)
	{
		if (histogram[i] > 0)
		{
			_tprintf(_T("%c: %zu\n"), (TCHAR)(i + _T('A')), histogram[i]);
		}
	}
	_tprintf(_T("\n"));

	size_t** digramHistogram = CreateDigramHistorgram(inputString);
	_tprintf(_T("digram histogram: \n"));
	for (size_t i = 0; i < 26; i++)
	{
		for (size_t j = 0; j < 26; j++)
		{
			if (digramHistogram[i][j] > 0)
			{
				_tprintf(_T("%c%c: %zu\n"), (TCHAR)(i + _T('A')), (TCHAR)(j + _T('A')), digramHistogram[i][j]);
			}
		}
	}
	_tprintf(_T("\n"));

	size_t*** trigramHistogram = CreateTrigramHistorgram(inputString);

	_tprintf(_T("trigram histogram: \n"));
	for (size_t i = 0; i < 26; i++)
	{
		for (size_t j = 0; j < 26; j++)
		{
			for (size_t k = 0; k < 26; k++)
			{
				if (trigramHistogram[i][j][k] > 0)
				{
					_tprintf(_T("%c%c%c: %zu\n"), (TCHAR)(i + _T('A')), (TCHAR)(j + _T('A')), (TCHAR)(k + _T('A')), trigramHistogram[i][j][k]);
				}
			}
		}
	}
	_tprintf(_T("\n"));

	int n;
	_tprintf(_T("enter displacement(k) to encrypt string: "));
	_tscanf_s(_T("%d"), &n);

	_tsystem(_T("pause"));
	_tsystem(_T("CLS"));

	EncryptString(inputString, n);

	_tprintf(_T("Ecrypted string: %s\n"), inputString);

	_tprintf(_T("Attempting to crack the code...\n"));

	CrackCaeserCypher(inputString, _T("ETAOINSRHLDCUMFPGWYBVKXJQZ"));

	_tsystem(_T("pause"));
	return 0;
}