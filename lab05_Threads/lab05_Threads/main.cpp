#include <stdlib.h>
#include <stdio.h>

#include "Everything.h"

#define MAX_THREADS 10
#define MAX_VALUE 100000

DWORD counter = 0;
CRITICAL_SECTION critical_section;

DWORD WINAPI Increment(PVOID arg)
{
	for (int i = 0; i < MAX_VALUE; i++) {
		EnterCriticalSection(&critical_section);
		counter++;
		LeaveCriticalSection(&critical_section);
	}
	return 0;
}

int _tmain()
{
	HANDLE hThread[MAX_THREADS];
	DWORD dwThreadID[MAX_THREADS];
	InitializeCriticalSectionAndSpinCount(&critical_section, 0x400);

	for (int i = 0; i < MAX_THREADS; i++)
		hThread[i] = CreateThread(NULL, 0, Increment, (PVOID)i, 0, &dwThreadID[i]);

	WaitForMultipleObjects(MAX_THREADS, hThread, TRUE, INFINITE);
	_tprintf(_T("counter: %d\n"), counter);
	system("pause");
}
