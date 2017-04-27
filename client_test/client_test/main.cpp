#include "Everything.h"

#define BUFF_SIZE 256

int _tmain(int argc, char **argv) {
	LPCTSTR lpszPipeName = _T("\\\\.\\pipe\\mynamedpipe");
	BOOL fSuccess = FALSE;
	HANDLE hNamedPipe;
	TCHAR buffer[BUFF_SIZE];
	DWORD cbReadBytes;
	
	while(true) {
		hNamedPipe = CreateFile(
			lpszPipeName,
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (hNamedPipe == INVALID_HANDLE_VALUE) {
			break;
		}

		do {
			fSuccess = ReadFile(
				hNamedPipe,
				buffer,
				BUFF_SIZE,
				&cbReadBytes,
				NULL);
			if (!fSuccess) {
				break;
			}
			buffer[cbReadBytes] = '\0';
			_tprintf(_T("successfully read %s\n"), buffer);
		} while (!fSuccess);
	}
	_tprintf(_T("end of program"));
	system("pause");
}