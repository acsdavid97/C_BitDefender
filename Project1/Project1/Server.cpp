#define _CRT_SECURE_NO_WARNINGS

#include "Everything.h"
#include "../../Project1/CommunicationProtocol.h"
#include "UserManagement.h"

#define BUFFSIZE 4096

TCHAR *sPipeName = _T("defaultpipename");
TCHAR *sLoggingFile;
TCHAR *sCredFile = _T("cred.txt");
FILE *logFile = NULL;
TCHAR logBuffer[BUFFSIZE];

VOID log(TCHAR *logString)
{
	_tprintf("%s\n", logString);
	if (logFile != NULL) {
		_ftprintf(logFile, "%s\n", logString);
	}
}

TCHAR *getRealPipeName(TCHAR *sPipeName)
{
	TCHAR *sRealPipeName = (TCHAR*)malloc(sizeof(TCHAR) * (_tcslen(sPipeName) + 10));
	if (sRealPipeName == NULL) {
		return NULL;
	}
	_tcscpy(sRealPipeName, _T("\\\\.\\pipe\\"));
	_tcscat(sRealPipeName, sPipeName);
	return sRealPipeName;
}

VOID printUsage()
{
	_tprintf("Usage:\n");
	_tprintf(_T("    program.exe [pipe=<pipename> logfile=<log file path> credfile=<cred file path>]\n"));
	_tprintf(_T("        where <pipename> is the name of the pipe to be used to accept client connections.\n"));
	_tprintf(_T("        where <log file path> is the path where the logging file should be created and updated.\n"));
	_tprintf(_T("    program.exe /h for this message\n"));
}

VOID parseArgument(TCHAR *arg)
{
	if (_tcsncmp(arg, _T("pipe="), 5) == 0) {
		sPipeName = arg + 5;
	} else if (_tcsncmp(arg, _T("logfile="), 8) == 0) {
		sLoggingFile = arg + 8;
		logFile = _tfopen(sLoggingFile, "w");
	} else if (_tcsncmp(arg, _T("credfile="), 9) == 0) {
		sCredFile = arg + 9;
	} else if (_tcscmp(arg, _T("/h")) == 0) {
		printUsage();
		exit(0);
	} else {
		_stprintf(logBuffer, _T("invalid parameter: \"%s\""), arg);
		log(logBuffer);
		printUsage();
		_stprintf(logBuffer, _T("exiting..."));
		log(logBuffer);
		exit(1);
	}
}

DWORD getNextPacket(HANDLE hPipe, TCHAR *buff)
{
	BOOL bSuccess;
	DWORD cbToBeRead;
	DWORD cbRead;
	DWORD command;

	bSuccess = ReadFile(
		hPipe,
		&command,
		sizeof(DWORD),
		&cbRead,
		NULL
	);

	if (!bSuccess || command != ENCRYPT_DATA) {
		return 0;
	}

	bSuccess = ReadFile(
		hPipe,
		&cbToBeRead,
		sizeof(DWORD),
		&cbRead,
		NULL
	);

	if (!bSuccess) {
		return 0;
	}

	bSuccess = ReadFile(
		hPipe,
		buff,
		cbToBeRead,
		&cbRead,
		NULL
	);

	if (!bSuccess) {
		return 0;
	}

	return cbRead;
}

DWORD encryptData(TCHAR* buff, DWORD dwBuffLen, TCHAR* key, DWORD dwKeyLen, DWORD dwStartPos)
{
	DWORD i;
	DWORD dwKeyIndex = 0;

	for(i = 0; i < dwBuffLen; i++) {
		dwKeyIndex = (i + dwStartPos) % dwKeyLen;
		buff[i] ^= key[dwKeyIndex];
	}

	return dwKeyIndex;
}

VOID sendPacket(HANDLE hPipe, TCHAR *buff, DWORD cbPacketLen)
{
	DWORD cbWritten;

	WriteFile(
		hPipe,
		buff,
		cbPacketLen,
		&cbWritten,
		NULL
	);
}

VOID serveClient(HANDLE hPipe, TCHAR *sEncryptionKey)
{
	TCHAR buff[BUFFSIZE];
	DWORD cbPacketSize;
	DWORD dwKeyLen = _tcslen(sEncryptionKey);
	DWORD dwPrevOffset = 0;

	while ((cbPacketSize = getNextPacket(hPipe, buff)) != 0) {
		dwPrevOffset = encryptData(buff, cbPacketSize, sEncryptionKey, dwKeyLen, dwPrevOffset);
		sendPacket(hPipe, buff, cbPacketSize);
	}
	//connection terminated
}


BOOL initializeConnection(HANDLE hPipe, InitT *init)
{
	BOOL bSuccess;
	DWORD cbRead;
	DWORD dwInitMessage = CONNECTION_ACCEPTED;
	DWORD cbBytesWritten;

	bSuccess = ReadFile(
		hPipe,
		init,
		sizeof(InitT),
		&cbRead,
		NULL);

	if (!bSuccess) {
		return FALSE;
	}

	if (init->command != INITIALIZE_CONNECTION) {
		return FALSE;
	}

	bSuccess = WriteFile(
		hPipe,
		&dwInitMessage,
		sizeof(DWORD),
		&cbBytesWritten,
		NULL);

	return bSuccess;
}

BOOL authenticateClient(HANDLE hPipe, InitT *init, CredentialManagerT* manager)
{
	BOOL bSuccess;
	DWORD dwResponse;
	DWORD cbWrittenOrRead;
	BOOL auth;

	TCHAR *sUserName = (TCHAR*)malloc(init->cbUsernameNrBytes + 1);
	TCHAR *sPassword = (TCHAR*)malloc(init->cbPasswordNrBytes + 1);
	if (sUserName == NULL || sPassword == NULL) {
		return FALSE;
	}

	sUserName[init->cbUsernameNrBytes] = '\0';
	sPassword[init->cbPasswordNrBytes] = '\0';

	bSuccess = ReadFile(
		hPipe,
		sUserName,
		init->cbUsernameNrBytes,
		&cbWrittenOrRead,
		NULL
	);

	if (!bSuccess) {
		return FALSE;
	}
	
	bSuccess = ReadFile(
		hPipe,
		sPassword,
		init->cbPasswordNrBytes,
		&cbWrittenOrRead,
		NULL
	);

	if (!bSuccess) {
		return FALSE;
	}

	CredentialT *credential = create_CredentialT(sUserName, sPassword);
	auth = checkClientCredentials(manager, credential);

	free(credential);

	free(sUserName);
	free(sPassword);
	dwResponse = (auth) ? AUTH_SUCCESSFUL : AUTH_REJECTED;

	bSuccess = WriteFile(
		hPipe,
		&dwResponse,
		sizeof(DWORD),
		&cbWrittenOrRead,
		NULL
	);

	if (!bSuccess) {
		return FALSE;
	}

	return auth;
}

TCHAR* getEncryptionKey(HANDLE hPipe, InitT* init)
{
	BOOL bSuccess;
	DWORD cbRead;

	TCHAR *sKey = (TCHAR*)malloc(init->cbKeyNrBytes + 1);
	if (sKey == NULL) {
		return NULL;
	}
	sKey[init->cbKeyNrBytes] = '\0';
	bSuccess = ReadFile(
		hPipe,
		sKey,
		init->cbKeyNrBytes,
		&cbRead,
		NULL
	);

	if (!bSuccess) {
		return NULL;
	}

	return sKey;
}

int _tmain(int argc, TCHAR *argv[])
{
	TCHAR *sRealPipeName;
	HANDLE hPipe;
	BOOL bConnected;
	InitT init;

	sRealPipeName= getRealPipeName(sPipeName);
	for(int i = 1; i < argc; i++) {
		parseArgument(argv[i]);
	}

	_stprintf(logBuffer, _T("pipe name: %s"), sPipeName);
	log(logBuffer);
	if (logFile != NULL) {
		_stprintf(logBuffer, _T("logging file: %s"), sLoggingFile);
		log(logBuffer);
	}else {
		_stprintf(logBuffer, _T("no logging file specified or the file could not be opened"));
		log(logBuffer);
	}
	_stprintf(logBuffer, _T("cred file: %s"), sCredFile);
	log(logBuffer);
	CredentialManagerT* credentialManager = loadCredentialsFromFile(sCredFile);
	if (credentialManager == NULL) {
		_stprintf(logBuffer, _T("Could not load credentials!"));
		log(logBuffer);
		exit(1);
	}

	while(true) {
		_stprintf(logBuffer, _T(""));
		log(logBuffer);

		hPipe = CreateNamedPipe(
			sRealPipeName,				//pipe name
			PIPE_ACCESS_DUPLEX,			//pipe open mode, we need to write and to read
			PIPE_TYPE_MESSAGE,			//message mode
			PIPE_UNLIMITED_INSTANCES,	//unlimited instances (as system resources permit)
			BUFFSIZE,					// size of output buffer
			BUFFSIZE,					//size of input buffer
			0,							// default time out of 50 milliseconds
			NULL						//default security attributes
		);
		if (hPipe == INVALID_HANDLE_VALUE) {
			_stprintf(logBuffer, _T("Could not create named pipe!"));
			log(logBuffer);
			exit(1);
		}

		_stprintf(logBuffer, _T("waiting for client"));
		log(logBuffer);
		if (ConnectNamedPipe(hPipe, NULL) != 0) {
			bConnected = TRUE;
		} else {
			// an error may occour if the client is connected before the call of ConnectNamedPipe function
			// if this is the case, then we can continue safely
			bConnected = GetLastError() == ERROR_PIPE_CONNECTED;
		}

		if (!bConnected) {
			// the client could not connect
			CloseHandle(hPipe);
			continue;
		}

		//the client is connected
		_stprintf(logBuffer, _T("Client connected to the pipe"));
		log(logBuffer);
		if (!initializeConnection(hPipe, &init)) {
			_stprintf(logBuffer, _T("Could not initialize connection with client"));
			log(logBuffer);
			CloseHandle(hPipe);
			continue;
		}
		if (!authenticateClient(hPipe, &init, credentialManager)) {
			_stprintf(logBuffer, _T("Access denied for client"));
			log(logBuffer);
			CloseHandle(hPipe);
			continue;
		}
		_stprintf(logBuffer, _T("Client successfully authentificated"));
		log(logBuffer);
		TCHAR *sEncryptionKey = getEncryptionKey(hPipe, &init);
		if (sEncryptionKey == NULL) {
			_stprintf(logBuffer, _T("Could not get encryption key"));
			log(logBuffer);
			CloseHandle(hPipe);
			continue;
		}
		serveClient(hPipe, sEncryptionKey);
		_stprintf(logBuffer, _T("Client has been served"));
		log(logBuffer);
		CloseHandle(hPipe);
	}
}

