#define _CRT_SECURE_NO_WARNINGS

#include "Everything.h"
#include "../CommunicationProtocol.h"
#include "UserManagement.h"
#include "EncSyncQueue.h"

#define BUFFSIZE 4096

typedef struct ClientThreadTag{
	HANDLE hPipe;
	TCHAR *sEncryptionKey;
	DWORD dwThreadId;
	TCHAR *clientName;
}ClientThreadT;


TCHAR *sRealPipeName;
TCHAR *sPipeName = _T("defaultpipename");
TCHAR *sLoggingFile;
TCHAR *sCredFile = _T("cred.txt");
FILE *logFile = NULL;
TCHAR logBuffer[BUFFSIZE];
INT nrMaxClients = 8;
INT nrCurrentClients = 0;
INT nrWorkers = 4;
BOOL quit = FALSE;
HANDLE *gpClientThreads;

CRITICAL_SECTION g_cs;
CRITICAL_SECTION gcsCredentialManger;

CredentialManagerT *gCredentialManager;
SyncCircQueueT *gQueue;


/*
 * Logs a message to the logFile.
 */
VOID log(TCHAR *logString)
{
	_tprintf("%s\n", logString);
	if (logFile != NULL) {
		_ftprintf(logFile, _T("%s\n"), logString);
	}
}

/*
 * Returns a correct pipe name constructed from a string.
 */
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
	_tprintf(_T("    program.exe [pipe=<pipename> logfile=<log file path> credfile=<cred file path> nr_clients=<max_nr_clients> nr_workers=<nr_worker_threads>]\n"));
	_tprintf(_T("        where <pipename> is the name of the pipe to be used to accept client connections.\n"));
	_tprintf(_T("        where <log file path> is the path where the logging file should be created and updated.\n"));
	_tprintf(_T("        where <max_nr_clients> is the maximum number of concurent clients, default value of 8.\n"));
	_tprintf(_T("        where <nr_worker_threads> is the number of worker thread used to encrypt, default value of 4.\n"));
	_tprintf(_T("    program.exe /h for this message\n"));
}

/*
 * Parses an argument and sets corresponding global variables.
 * Exits if a malformatted argument is given.
 */
VOID parseArgument(TCHAR *arg)
{
	if (_tcsncmp(arg, _T("pipe="), 5) == 0) {
		sPipeName = arg + 5;
	} else if (_tcsncmp(arg, _T("logfile="), 8) == 0) {
		sLoggingFile = arg + 8;
		logFile = _tfopen(sLoggingFile, _T("w"));
	} else if (_tcsncmp(arg, _T("credfile="), 9) == 0) {
		sCredFile = arg + 9;
	} else if (_tcsncmp(arg, _T("nr_clients="), 11) == 0) {
		if(_stscanf(arg + 11, _T("%d"), &nrMaxClients) != 1 || nrMaxClients <= 0) {
			_stprintf(logBuffer, _T("invalid number at nr_clients"));
			log(logBuffer);
			exit(3);
		}
	} else if (_tcsncmp(arg, _T("nr_workers="), 11) == 0) {
		if(_stscanf(arg + 11, _T("%d"), &nrWorkers) != 1 || nrWorkers <= 0) {
			_stprintf(logBuffer, _T("invalid number at nr_workers"));
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

/*
 * Gets the next packet from the pipe.
 * 
 * @param hPipe: handle to the pipe
 * @param buff: buffer where the packet will be stored
 * @return the number of bytes read
*/
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

VOID printParams()
{
	_tprintf(_T("pipename: %s\n"), sPipeName);
	if (logFile == NULL) {
		_tprintf(_T("no logging file was specified\n"));
	} else {
		_tprintf(_T("logging file: %s\n"), sLoggingFile);
	}
	_tprintf(_T("nr_max_clients: %d\n"), nrMaxClients);
	_tprintf(_T("nr_workers: %d\n"), nrWorkers);
	_tprintf(_T("credential file: %s\n"), sCredFile);
}

/*
 * Encrypts a buff with key using xor.
 */
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

/*
 * Sends a packet through the pipe.
 */
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

/*
 * Function of worker threads.
 * Gets packet info from gQueue and ecrypts it.
 * When the packet is encrypted, the thread requesting the encryption will be signaled.
 */
DWORD WINAPI workerThread(void* arg)
{
	EncryptDataT* encData;
	while(true) {
		popSyncQueue(gQueue, &encData);
		_tprintf(_T("starting encryption\n"));
		encData->dwStartPos = encryptData(encData->toBeEncrypted, encData->dwBuffLen,
			encData->encryptionKey, encData->dwKeyLen, encData->dwStartPos);

		EnterCriticalSection(encData->criticalSection);

		encData->dwStatus = DATA_ENCRYPTED;
		_tprintf(_T("done with the ec encryption\n"));
		
		LeaveCriticalSection(encData->criticalSection);
		WakeConditionVariable(encData->conditionVariable);
	}
}

/*
 *Function of client threads.
 *Gets packet from the pipe and puts it in the gQueue.
 *Once the thread this thread has been signaled that the encryption is done the ecrypted packets will be sent back to the client though the pipe.
*/
DWORD WINAPI serveClient(ClientThreadT* clientThreadArg)
{
	TCHAR buff[BUFFSIZE];
	DWORD cbPacketSize;
	DWORD dwKeyLen = _tcslen(clientThreadArg->sEncryptionKey);
	DWORD dwPrevOffset = 0;
	EncryptDataT encryptDataField;
	CONDITION_VARIABLE conditionVariable;
	CRITICAL_SECTION criticalSection;
	DWORD cbTotalEncrypted = 0;

	InitializeCriticalSection(&criticalSection);
	InitializeConditionVariable(&conditionVariable);

	encryptDataField.criticalSection = &criticalSection;
	encryptDataField.conditionVariable = &conditionVariable;
	encryptDataField.encryptionKey = clientThreadArg->sEncryptionKey;
	encryptDataField.dwKeyLen = dwKeyLen;
	encryptDataField.dwStatus = DATA_NOT_ENCRYPTED;
	encryptDataField.toBeEncrypted = buff;

	_stprintf(logBuffer, _T("client thread initialized\n"));
	log(logBuffer);

	while ((cbPacketSize = getNextPacket(clientThreadArg->hPipe, buff)) != 0) {

		encryptDataField.dwBuffLen = cbPacketSize;
		encryptDataField.dwStartPos = dwPrevOffset;

		pushSyncQueue(gQueue, &encryptDataField);

		EnterCriticalSection(encryptDataField.criticalSection);
		_tprintf(_T("waiting for encrytion\n"));

		while (encryptDataField.dwStatus != DATA_ENCRYPTED) {
			SleepConditionVariableCS(encryptDataField.conditionVariable, encryptDataField.criticalSection, INFINITE);
		}
		_tprintf(_T("done waiting for encrytion\n"));
		dwPrevOffset = encryptDataField.dwStartPos;
		LeaveCriticalSection(encryptDataField.criticalSection);

		sendPacket(clientThreadArg->hPipe, buff, cbPacketSize);
		cbTotalEncrypted += cbPacketSize;
	}
	//connection terminated

	_stprintf(logBuffer, _T("terminated client thread"));
	log(logBuffer);

	CloseHandle(clientThreadArg->hPipe);


	EnterCriticalSection(&gcsCredentialManger);
	addBytesToClientAndDisconnect(gCredentialManager, clientThreadArg->clientName, cbTotalEncrypted);
	LeaveCriticalSection(&gcsCredentialManger);

	EnterCriticalSection(&g_cs);
	nrCurrentClients--;
	LeaveCriticalSection(&g_cs);
	return 0;
}


/*
 * Initializes connection with the client on the pipe.
 * Initialization parameters are saved in init.
 */
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

/*
 * Authenticates a client through hPipe.
 * For auth a CredentialManager is used.
 */
BOOL authenticateClient(HANDLE hPipe, InitT *init, CredentialManagerT* manager, TCHAR **sUserName)
{
	BOOL bSuccess;
	DWORD dwResponse;
	DWORD cbWrittenOrRead;
	BOOL auth;

	*sUserName = (TCHAR*)malloc(init->cbUsernameNrBytes + sizeof(TCHAR));
	TCHAR *sPassword = (TCHAR*)malloc(init->cbPasswordNrBytes + sizeof(TCHAR));
	if (*sUserName == NULL || sPassword == NULL) {
		return FALSE;
	}

	(*sUserName)[init->cbUsernameNrBytes/sizeof(TCHAR)] = '\0';
	sPassword[init->cbPasswordNrBytes/sizeof(TCHAR)] = '\0';

	bSuccess = ReadFile(
		hPipe,
		*sUserName,
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

	CredentialT *credential = create_CredentialT(*sUserName, sPassword);
	auth = checkClientCredentials(manager, credential);

	free(credential);

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

/*
 * Returns the encryption key of the client.
 */
TCHAR* getEncryptionKey(HANDLE hPipe, InitT* init)
{
	BOOL bSuccess;
	DWORD cbRead;

	TCHAR *sKey = (TCHAR*)malloc(init->cbKeyNrBytes + 1);
	if (sKey == NULL) {
		return NULL;
	}
	sKey[init->cbKeyNrBytes/sizeof(TCHAR)] = '\0';
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

DWORD WINAPI commandThread(void* arg)
{
	DWORD max_clients;
	TCHAR buff[BUFFSIZE];

	printParams();
	_fgetts(buff, BUFFSIZE, stdin);

	while (_tcscmp(buff, _T("exit\n")) != 0) {
		if (_tcscmp(buff, _T("list\n")) == 0) {
			listCredentials(gCredentialManager);

		}else if(_tcscmp(buff, _T("params\n")) == 0) {
			printParams();
		}else if(_tcscmp(buff, _T("help\n")) == 0) {
			_tprintf(_T("possible commands:\n"));
			_tprintf(_T("list -- list information about clients\n"));
			_tprintf(_T("params -- print the parameters of the program\n"));
			_tprintf(_T("exit -- gracefully ends the execution of the program\n"));
		}
		else {
			_tprintf(_T("invalid command -- check out our cool help function!\n"));
		}
		_fgetts(buff, BUFFSIZE, stdin);
	}

	EnterCriticalSection(&g_cs);
	max_clients = nrMaxClients;
	nrMaxClients = 0;
	LeaveCriticalSection(&g_cs);

	_tprintf(_T("no more clients are accepted!\n"));
	_tprintf(_T("waiting for client threads to terminate\n"));
	for(DWORD i = 0; i < max_clients; i++) {
		if (gpClientThreads[i] != NULL) {
			WaitForSingleObject(gpClientThreads[i], INFINITE);
		}
	}

	free(sRealPipeName);
	free(gCredentialManager);
	free(gpClientThreads);
	if (logFile != NULL) {
		fclose(logFile);
	}
	_tprintf(_T("exiting..."));
	ExitProcess(0);
}


/*
 * initializes the Server and prints out some parameters.
 */
void initializeServer(int argc, TCHAR *argv[])
{
	for (int i = 1; i < argc; i++) {
		parseArgument(argv[i]);
	}

	gpClientThreads = (HANDLE*)malloc(sizeof(HANDLE) * nrMaxClients);
	if (gpClientThreads == NULL) {
		_stprintf(logBuffer, _T("Could not allocate memory\n"));
		log(logBuffer);
		exit(4);
	}

	for (INT i = 0; i < nrMaxClients; i++) {
		gpClientThreads[i] = NULL;
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
	_stprintf(logBuffer, _T("number of worker_threads: %d"), nrWorkers);
	log(logBuffer);
	_stprintf(logBuffer, _T("cred file: %s"), sCredFile);
	log(logBuffer);
	gCredentialManager = loadCredentialsFromFile(sCredFile);
	if (gCredentialManager == NULL) {
		_stprintf(logBuffer, _T("Could not load credentials!"));
		log(logBuffer);
		exit(1);
	}

	gQueue = create_SyncCircQueueT(nrMaxClients);
	if (gQueue == NULL) {
		_stprintf(logBuffer, _T("Could not allocate memory!"));
		log(logBuffer);
		exit(6);
	}

	for(INT i = 0; i < nrWorkers; i++) {
		HANDLE hThread = (HANDLE)_beginthreadex(
			NULL,
			0,
			(_beginthreadex_proc_type)workerThread,
			(void*)i,
			0,
			NULL
		);

		if (hThread == NULL) {
			_stprintf(logBuffer, _T("could not create worker threads!"));
			log(logBuffer);
			exit(5);
		}
	}

	HANDLE hThread = (HANDLE)_beginthreadex(
		NULL,
		0,
		(_beginthreadex_proc_type)commandThread,
		NULL,
		0,
		NULL
	);

	if (hThread == NULL) {
		_stprintf(logBuffer, _T("could not create command thread!"));
		log(logBuffer);
		exit(5);
	}
	CloseHandle(hThread);

	InitializeCriticalSection(&g_cs);
	InitializeCriticalSection(&gcsCredentialManger);

}

/*
 * returns a pipe to a connected client.
 */
HANDLE getClientConnection(TCHAR *pipeName)
{
	HANDLE hPipe;
	BOOL bConnected;

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
		}
		else {
			return hPipe;
		}
	}
}

/*
 * Server program for encryption.
 * Server creates a pipe and worker threads, after that it waits for clients to connect.
 * Once a client has connected, the server will authenticate the client and get the encryption key from the client.
 * A separate thread is created for each of the authenticated clients, which will read the bytes to be encrypted, submit those bytes to encryption to the worker threads.
 * Once the bytes are ecrypted, they are sent back to client.
 */
int _tmain(int argc, TCHAR *argv[])
{
	HANDLE hPipe;
	InitT init;
	HANDLE hThread;
	DWORD dwThreadId = 0;
	TCHAR* clientName;

	initializeServer(argc, argv);

	while(true) {

		hPipe = getClientConnection(sRealPipeName);
		//the client is connected
		_stprintf(logBuffer, _T("Client connected to the pipe"));
		log(logBuffer);
		if (!initializeConnection(hPipe, &init)) {
			_stprintf(logBuffer, _T("Could not initialize connection with client"));
			log(logBuffer);
			CloseHandle(hPipe);
			continue;
		}
		if (!authenticateClient(hPipe, &init, gCredentialManager, &clientName)) {
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
		clientThreadArg->clientName = clientName;

		EnterCriticalSection(&g_cs);

		clientThreadArg->dwThreadId = dwThreadId;
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
		gpClientThreads[dwThreadId] = hThread;
		dwThreadId = (dwThreadId + 1) % nrMaxClients;

		nrCurrentClients++;
		LeaveCriticalSection(&g_cs);

		_stprintf(logBuffer, _T("Client thread has been created"));
		log(logBuffer);
	}
}

