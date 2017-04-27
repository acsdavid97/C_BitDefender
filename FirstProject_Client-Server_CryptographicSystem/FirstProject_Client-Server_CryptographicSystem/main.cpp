
#include <Everything.h>

#define BUFF_SIZE 256

int _tmain(int argc, char **argv)
{
	HANDLE hNamedPipe;
	BOOL fConnected = FALSE;
	BOOL fWriteSuccess = FALSE;
	DWORD cbWrittenBytes = 0;
	LPCTSTR lpszPipeName = _T("\\\\.\\pipe\\mynamedpipe");
	LPCTSTR lpszPipeMessage = _T("this is a message");

	while(true) {
		hNamedPipe = CreateNamedPipe(
			lpszPipeName,
			PIPE_ACCESS_OUTBOUND,
			PIPE_TYPE_BYTE,
			1,
			BUFF_SIZE,
			BUFF_SIZE,
			0,
			NULL
			);

		if (hNamedPipe == INVALID_HANDLE_VALUE) {
			_tprintf(_T("no pipe for you\n"));
				break;
		}

		fConnected = ConnectNamedPipe(hNamedPipe, NULL) ?
			TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

		if (fConnected) {
			fWriteSuccess = WriteFile(
				hNamedPipe,
				lpszPipeMessage,
				4,
				&cbWrittenBytes,
				NULL);
			if (!fWriteSuccess) {
				_tprintf(_T("write failed\n"));
				break;
			}
		}
		else {
			_tprintf(_T("not connected\n"));
		}
	}
	_tprintf(_T("end of program\n"));
	system("pause");
}