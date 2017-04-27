/*
author: David
file name: main
file creation date: 2017-03-15 11:16:12
*/

#include "Everything.h"

#define BUF_SIZE 256 

static VOID Encrypt(HANDLE hInFile, HANDLE hOutFile, BYTE key) {
	DWORD nIn;
	DWORD nOut;

	BYTE buffer[BUF_SIZE];

	BOOL WriteOK = TRUE;

	while (WriteOK && ReadFile(hInFile, buffer, BUF_SIZE, &nIn, NULL)) {
		for (DWORD iCrypt = 0; iCrypt < nIn; iCrypt++) {
			buffer[iCrypt] ^= key;
		}

	}


int main()
{

	return 0;
}