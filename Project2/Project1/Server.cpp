#define _CRT_SECURE_NO_WARNINGS

#include "Everything.h"
#include "../../Project1/CommunicationProtocol.h"
#include "UserManagement.h"

#define BUFFSIZE 4096

typedef struct ClientThreadTag{
	HANDLE hPipe;
	TCHAR *sEncryptionKey;
}ClientThreadT;

TCHAR *sRealPipeName;
TCHAR *sPipeName = _T("defaultpipename");
TCHAR *sLoggingFile;
TCHAR *sCredFile = _T("cred.txt");
FILE *logFile = NULL;
TCHAR logBuffer[BUFFSIZE];
INT nrMaxClients = 4;
INT nrCurrentClients = 0;

CRITICAL_SECTION g_cs;

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
	_tprintf(_T("Usage:\n"));
	_tprintf(_T("    program.exe [pipe=<pipename> logfile=<log file path> credfile=<cred file path> nr_clients=<max_nr_clients>]\n"));
	_tprintf(_T("        where <pipename> is the name of the pipe to be used to accept client connections.\n"));
	_tprintf(_T("        where <log file path> is the path where the logging file should be created and updated.\n"));
	_tprintf(_T("        where <max_nr_clients> is the maximum number of concurent clients, default value of 4.\n"));
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
	} else if (_tcsncmp(arg, _T("nr_clients="), 11) == 0) {
		if(_stscanf(arg + 11, _T("%d"), &nrMaxClients) != 1 || nrMaxClients <= 0) {
			_stprintf(logBuffer, _T("invalid number at nr_clients"));
			log(logBuffer);
			exit(3);
		}
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

DWORD WINAPI serveClient(ClientThreadT* clientThreadArg)
{
	TCHAR buff[BUFFSIZE];
	DWORD cbPacketSize;
	DWORD dwKeyLen = _tcslen(clientThreadArg->sEncryptionKey);
	DWORD dwPrevOffset = 0;

	while ((cbPacketSize = getNextPacket(clientThreadArg->hPipe, buff)) != 0) {
		dwPrevOffset = encryptData(buff, cbPacketSize, clientThreadArg->sEncryptionKey, dwKeyLen, dwPrevOffset);
		sendPacket(clientThreadArg->hPipe, buff, cbPacketSize);
	}
	//connection terminated

	CloseHandle(clientThreadArg->hPipe);

	EnterCriticalSection(&g_cs);
	nrCurrentClients--;
	LeaveCriticalSection(&g_cs);
	return 0;
}


BOOL initializeConnection(HANDLE hPipe, InitT *init)
{
	BOOL bSuccess;
	BOOL bAccepted;
	DWORD cbRead;
	DWORD dwInitMessage;
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
	EnterCriticalSection(&g_cs);
	bAccepted = nrCurrentClients < nrMaxClients;
	if (bAccepted) {
		_stprintf(logBuffer, _T("Connection accepted, current nr_clients: %d"), nrCurrentClients);
	} else {
		_stprintf(logBuffer, _T("Connection not accepted, due to high number of clients"));
	}
	log(logBuffer);
	LeaveCriticalSection(&g_cs);

	dwInitMessage = (bAccepted) ? CONNECTION_ACCEPTED : CONNECTION_REJECTED;
	bSuccess = WriteFile(
		hPipe,
		&dwInitMessage,
		sizeof(DWORD),
		&cbBytesWritten,
		NULL);

	return bSuccess && bAccepted;
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

CredentialManagerT* initializeServer(int argc, TCHAR *argv[])
{
	for(int i = 1; i < argc; i++) {
		parseArgument(argv[i]);
	}

	sRealPipeName = getRealPipeName(sPipeName);

	_stprintf(logBuffer, _T("pipe name: %s"), sPipeName);
	log(logBuffer);
	if (logFile != NULL) {
		_stprintf(logBuffer, _T("logging file: %s"), sLoggingFile);
		log(logBuffer);
	}else {
		_stprintf(logBuffer, _T("no logging file specified or the file could not be opened"));
		log(logBuffer);
	}
	_stprintf(logBuffer, _T("max number of clients: %d"), nrMaxClients);
	log(logBuffer);
	_stprintf(logBuffer, _T("cred file: %s"), sCredFile);
	log(logBuffer);
	CredentialManagerT* credentialManager = loadCredentialsFromFile(sCredFile);
	if (credentialManager == NULL) {
		_stprintf(logBuffer, _T("Could not load credentials!"));
		log(logBuffer);
		exit(1);
	}
	
	InitializeCriticalSection(&g_cs);
	return credentialManager;
}

int _tmain(int argc, TCHAR *argv[])
{
	HANDLE hPipe;
	BOOL bConnected;
	InitT init;
	HANDLE hThread;

	CredentialManagerT * credentialManager = initializeServer(argc, argv);

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

		ClientThreadT* clientThreadArg = (ClientThreadT*)malloc(sizeof(ClientThreadT));
		if (clientThreadArg == NULL) {
			_stprintf(logBuffer, _T("could not allocate memory"));
			log(logBuffer);
			exit(5);
		}

		clientThreadArg->hPipe = hPipe;
		clientThreadArg->sEncryptionKey = sEncryptionKey;

		hThread = (HANDLE)_beginthreadex(
			NULL, //default security attr
			0, //default stack
			(_beginthreadex_proc_type)serveClient,
			clientThreadArg,
			0,
			NULL
		);

		if (hThread == NULL) {
			_stprintf(logBuffer, _T("Thread creation failed"));
			log(logBuffer);
			exit(6);
		}
		EnterCriticalSection(&g_cs);
		nrCurrentClients++;
		LeaveCriticalSection(&g_cs);

		CloseHandle(hThread);

		_stprintf(logBuffer, _T("Client thread has been created"));
		log(logBuffer);
	}
}

