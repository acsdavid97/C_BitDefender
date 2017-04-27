/*
 * author: Acs David
 * Last updated: 2017. 04. 24
 * For detailed info about what this program does see the comments at the _tmain function.
 */

#define _CRT_SECURE_NO_WARNINGS

#include "Everything.h"
#include "../CommunicationProtocol.h"

#define BUFFSIZE 4096

PTCHAR sFilePath = NULL;
PTCHAR sUserName = _T("david");
PTCHAR sPassword = _T("defaultpassword");
PTCHAR sPipeName = _T("defaultpipename");
PTCHAR sOutPutPath = NULL;
PTCHAR sKey = NULL;

void printUsage()
{
	_tprintf(_T("Usage:\n"));
	_tprintf(_T("    program.exe filepath=<filepath> [user=<username> pass=<password> pipe=<pipename> outputpath=<outputpath> key=<encryption key>]\n"));
	_tprintf(_T("        where <filepath> is the path of the file to be encrypted\n"));
	_tprintf(_T("        where <username> is the username of the client requesting the encryption, default value: david\n"));
	_tprintf(_T("        where <password> is the password of the client requesting the encryption, default value: defaultpassword\n"));
	_tprintf(_T("        where <pipename> is the name of the pipe used to connect to the server, if it is not supplied a default value is used.\n"));
	_tprintf(_T("        where <outputpath> is the path of the resulting encryped file, if it is not supplied <filepath>.enc is used.\n"));
	_tprintf(_T("        where <key> is the encryption key used to ecrypt the file, if it not supplied <password is used.\n"));
	_tprintf(_T("    program.exe /h for this message\n"));
}


PTCHAR getRealPipeName(PTCHAR sPipeName)
{
	PTCHAR sRealPipeName = (PTCHAR)malloc(sizeof(TCHAR) * (_tcslen(sPipeName) + 10));
	if (sRealPipeName == NULL) {
		return NULL;
	}
	_tcscpy(sRealPipeName, _T("\\\\.\\pipe\\"));
	_tcscat(sRealPipeName, sPipeName);
	return sRealPipeName;
}

void parseArgument(PTCHAR arg)
{
	if (_tcsncmp(arg, _T("filepath="), 9) == 0) {
		sFilePath = arg + 9;
	} else if (_tcsncmp(arg, _T("user="), 5) == 0) {
		sUserName = arg + 5;
	} else if (_tcsncmp(arg, _T("pass="), 5) == 0) {
		sPassword = arg + 5;
	} else if (_tcsncmp(arg, _T("pipe="), 5) == 0) {
		sPipeName = arg + 5;
	} else if (_tcsncmp(arg, _T("outputpath="), 11) == 0) {
		sOutPutPath = arg + 11;
	} else if (_tcsncmp(arg, _T("key="), 4) == 0) {
		sKey = arg + 4;
	} else if(_tcscmp(arg, _T("/h")) == 0) {
		printUsage();
		exit(0);
	} else {
		_tprintf(_T("invalid argument: \"%s\"\n"), arg);
		printUsage();
		_tprintf(_T("exiting...\n"));
		exit(1);
	}
}

void validateArguments()
{
	if (sFilePath == NULL || _tcslen(sFilePath) <= 0) {
		_tprintf(_T("Error: file path not specified\n"));
		printUsage();
		exit(1);
	}

	if (sUserName == NULL || _tcslen(sUserName) <= 0) {
		_tprintf(_T("Error: user name not specified\n"));
		printUsage();
		exit(1);
	}
	
	if (sPassword == NULL || _tcslen(sPassword) <= 0) {
		_tprintf(_T("Error: password not specified\n"));
		printUsage();
		exit(1);
	}

	if (sOutPutPath == NULL || _tcslen(sOutPutPath) <= 0) {
		sOutPutPath = (PTCHAR)malloc(sizeof(TCHAR) * (_tcslen(sFilePath) + 4));
		if (sOutPutPath == NULL) {
			_tprintf(_T("memory allocation error\n"));
			exit(2);
		}
		_tcscpy(sOutPutPath, sFilePath);
		_tcscat(sOutPutPath, _T(".enc"));
	}

	if (sKey == NULL || _tcslen(sKey) <= 0) {
		sKey = sPassword;
	}
}

void printArguments()
{
	_tprintf(_T("file path: \"%s\"\n"), sFilePath);
	_tprintf(_T("user name: \"%s\"\n"), sUserName);
	_tprintf(_T("password: \"%s\"\n"), sPassword);
	_tprintf(_T("output path: \"%s\"\n"), sOutPutPath);
	_tprintf(_T("pipe name: \"%s\"\n"), sPipeName);
	_tprintf(_T("encryption key: \"%s\"\n"), sKey);
}

HANDLE connectToServer()
{
	PTCHAR sRealPipeName = getRealPipeName(sPipeName);
	DWORD dwMode;
	BOOL bSuccess;
	HANDLE hPipe;

	//trying to connect to the pipe
	while(true) {
		hPipe = CreateFile(
			sRealPipeName, //name of the pipe
			GENERIC_READ | GENERIC_WRITE, //read and write access
			0, //we don't need sharing
			NULL, //default security attributes
			OPEN_EXISTING, //opens an existing (duh?)
			0, //default attributes
			NULL //we dont' need template
		);

		if (hPipe != INVALID_HANDLE_VALUE) {
			//successfully connected to pipe, break the loop
			break;
		}

		if (GetLastError() != ERROR_PIPE_BUSY) {
			_tprintf(_T("Could not connect to pipe (pipe is not busy)\n"));
			exit(1);
		}

		//all pipes are busy, wait for 1 sec
		if (!WaitNamedPipe(sRealPipeName, 10000)) {
			_tprintf(_T("Could not connect to pipe, timed out\n"));
			exit(2);
		}
	}

	//we can only arrive here if we successfully connected
	dwMode = PIPE_READMODE_MESSAGE;
	bSuccess = SetNamedPipeHandleState(
		hPipe,
		&dwMode,
		NULL, // must be NULL, since processes are on the same computer
		NULL // must be NULL, since processes are on the same computer
	);

	if (!bSuccess) {
		_tprintf(_T("Could not set the pipe to message mode\n"));
		exit(3);
	}
	return hPipe;
}

BOOL sendPacket(HANDLE hPipe, PTCHAR buff, DWORD cbPacketLen)
{
	DWORD cbWritten;
	BOOL bSuccess;
	DWORD command = ENCRYPT_DATA;

	bSuccess = WriteFile(
		hPipe,
		&command,
		sizeof(DWORD),
		&cbWritten,
		NULL
	);

	if (!bSuccess) {
		return FALSE;
	}

	bSuccess = WriteFile(
		hPipe,
		&cbPacketLen,
		sizeof(DWORD),
		&cbWritten,
		NULL
	);

	if (!bSuccess) {
		return FALSE;
	}

	bSuccess = WriteFile(
		hPipe,
		buff,
		cbPacketLen,
		&cbWritten,
		NULL
	);

	return bSuccess;
}

BOOL getPacket(HANDLE hPipe, PTCHAR buff, DWORD cbPacketLen)
{
	DWORD cbRead;
	BOOL bSuccess;

	bSuccess = ReadFile(
		hPipe,
		buff,
		cbPacketLen,
		&cbRead,
		NULL
	);

	return bSuccess;
}

/*
 * Starts encryption process: reads bytes from the source file, sends them to the server, 
 * waits for the response(encrypted packet) and writes it to the destination file.
 */
BOOL encryptFileWithServer(HANDLE hFileSource, HANDLE hPipe, HANDLE hFileDest)
{
	DWORD cbRead = 1;
	BOOL bSuccess;
	TCHAR buffer[BUFFSIZE];
	DWORD cbWritten;
	DWORD dwResponse = TERMINATE_CONNECTION;

	while(true) {
		bSuccess = ReadFile(
			hFileSource,
			buffer,
			BUFFSIZE,
			&cbRead,
			NULL
		);
		if (!bSuccess) {
			_tprintf(_T("Could not read from source file\n"));
			break;
		}

		if (cbRead == 0) {
			// at the end of file
			_tprintf(_T("Successfully arrived at the end of file\n"));
			bSuccess = TRUE;
			break;
		}

		if (!sendPacket(hPipe, buffer, cbRead)) {
			_tprintf(_T("could not send packet\n"));
			bSuccess = FALSE;
			break;
		}

		if (!getPacket(hPipe, buffer, cbRead)) {
			_tprintf(_T("could not get packet\n"));
			bSuccess = FALSE;
			break;
		}

		bSuccess = WriteFile(
			hFileDest,
			buffer,
			cbRead,
			&cbWritten,
			NULL
		);

		if (!bSuccess) {
			_tprintf(_T("could not read\n"));
			break;
		}
	}

	WriteFile(
		hPipe,
		&dwResponse,
		sizeof(DWORD),
		&cbWritten,
		NULL
	);

	return bSuccess;
}


/*
 * Initializes the connection to the server by sending and receiving specific packets.
 */
BOOL initilizeConnection(HANDLE hPipe)
{
	BOOL bSuccess;
	DWORD cbWritten;
	InitT initMessage;
	DWORD dwResponse = CONNECTION_REJECTED;
	DWORD dwRead;

	initMessage.command = INITIALIZE_CONNECTION;
	initMessage.cbPasswordNrBytes = _tcslen(sPassword) * sizeof(TCHAR);
	initMessage.cbUsernameNrBytes = _tcslen(sUserName) * sizeof(TCHAR);
	initMessage.cbKeyNrBytes = _tcslen(sKey) * sizeof(TCHAR);

	bSuccess = WriteFile(
		hPipe, //handle to pipe
		&initMessage, //message to be written
		sizeof(InitT), //number bytes to write
		&cbWritten, //number of bytes written
		NULL //no overlap
	);

	if (!bSuccess) {
		return FALSE;
	}

	bSuccess = ReadFile(
		hPipe, //handle to pipe
		&dwResponse, // buffer for response 
		sizeof(DWORD), // number of bytes to be read
		&dwRead, // number of bytes read
		NULL//no overlap
	);

	if (!bSuccess) {
		return FALSE;
	}

	return dwResponse == CONNECTION_ACCEPTED;
}

/*
 * Tries to authenticate the user with the server, by sending the username and password.
 * If auth is successful then it return TRUE, otherwise FALSE.
 */
BOOL authenthicate(HANDLE hPipe)
{
	BOOL bSuccess;
	DWORD cbUserNameNrBytes = _tcslen(sUserName) * sizeof(TCHAR);
	DWORD cbPasswordNrBytes = _tcslen(sPassword) * sizeof(TCHAR);
	DWORD cbWritten;
	DWORD dwResponse = AUTH_REJECTED;

	bSuccess = WriteFile(
		hPipe,
		sUserName,
		cbUserNameNrBytes,
		&cbWritten,
		NULL
	);

	if (!bSuccess) {
		return FALSE;
	}

	bSuccess = WriteFile(
		hPipe,
		sPassword,
		cbPasswordNrBytes,
		&cbWritten,
		NULL
	);

	if (!bSuccess) {
		return FALSE;
	}

	bSuccess = ReadFile(
		hPipe,
		&dwResponse,
		sizeof(DWORD),
		&cbWritten,
		NULL
	);

	if (!bSuccess) {
		return FALSE;
	}

	return dwResponse == AUTH_SUCCESSFUL;
}

/*
 * Sends the ecryption key to the server.
*/
BOOL sendEncryptionKey(HANDLE hPipe)
{
	BOOL bSuccess;
	DWORD cbWritten;

	bSuccess = WriteFile(
		hPipe,
		sKey,
		sizeof(TCHAR) * _tcslen(sKey),
		&cbWritten,
		NULL
	);

	return bSuccess;
}

/*
 * Client program for encryption.
 * Tries to connect to server, then authenticates the user(credential provided in command line arguments).
 * Once authenticated the client sends the bytes of the file, whose path is specified by the user, to the server for ecryption.
 * The ecrypted file is saved to the file path specified by the user.
 */
int _tmain(int argc, PTCHAR argv[])
{
	for(int i = 1; i < argc; i++) {
		parseArgument(argv[i]);
	}

	validateArguments();
	printArguments();

	HANDLE hFileSource = CreateFile(
		sFilePath,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hFileSource == INVALID_HANDLE_VALUE) {
		_tprintf(_T("File does not exist, or could not be open: %s\n"), sFilePath);
		exit(1);
	}

	HANDLE hFileDest = CreateFile(
		sOutPutPath,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hFileDest == INVALID_HANDLE_VALUE) {
		_tprintf(_T("Could not create destination file\n"));
		CloseHandle(hFileSource);
		exit(1);
	}


	_tprintf(_T("Attempting to connect to pipe\n"));
	HANDLE hPipe = connectToServer();
	_tprintf(_T("Successfully connected to the pipe\n"));

	if (!initilizeConnection(hPipe)) {
		_tprintf(_T("Could not initialize connection (server is probably busy)\n"));
		CloseHandle(hFileSource);
		CloseHandle(hFileDest);
		exit(2);
	}
	_tprintf(_T("Initialized connection\n"));

	if (!authenthicate(hPipe)) {
		_tprintf(_T("Authentication not successful!\n"));
		CloseHandle(hPipe);
		CloseHandle(hFileSource);
		CloseHandle(hFileDest);
		exit(3);
	}
	_tprintf(_T("Authenticated successfully\n"));

	if (!sendEncryptionKey(hPipe)) {
		_tprintf(_T("An error occoured while sending encryption key\n"));
		CloseHandle(hPipe);
		CloseHandle(hFileSource);
		CloseHandle(hFileDest);
		exit(4);
	}
	_tprintf(_T("beginning encrypting the file\n"));

	if(!encryptFileWithServer(hFileSource, hPipe, hFileDest)) {
		_tprintf(_T("An error occured while ecrypting\n"));
		CloseHandle(hPipe);
		CloseHandle(hFileSource);
		CloseHandle(hFileDest);
		exit(5);
	}
	_tprintf(_T("Encryption completed without error\n"));

	CloseHandle(hPipe);
	CloseHandle(hFileSource);
	CloseHandle(hFileDest);
}