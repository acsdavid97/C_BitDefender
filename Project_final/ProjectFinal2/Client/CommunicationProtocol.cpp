
#include "CommunicationProtocol.h"

BOOL getNextPacket(HANDLE hPipe, PTCHAR buff, LPDWORD pcbPacketSize)
{
	BOOL bSuccess;
	DWORD cbRead;
	DWORD command;

	bSuccess = ReadFile(
		hPipe,
		&command,
		sizeof(DWORD),
		&cbRead,
		NULL
	);

	//check for failed read or incorrect packet header.
	if (!bSuccess || (command != LAST_PACKET && command != NEXT_PACKET)) {
		return FALSE;
	}

	if (command == LAST_PACKET) {
		*pcbPacketSize = 0;
		return TRUE;
	}

	//command is only NEXT_PACKET
	bSuccess = ReadFile(
		hPipe,
		pcbPacketSize,
		sizeof(DWORD),
		&cbRead,
		NULL
	);

	if (!bSuccess) {
		return FALSE;
	}

	bSuccess = ReadFile(
		hPipe,
		buff,
		*pcbPacketSize,
		&cbRead,
		NULL
	);

	return bSuccess;
}

BOOL sendPacket(HANDLE hPipe, PTCHAR buff, DWORD cbPacketLen)
{
	DWORD cbWritten;
	BOOL bSuccess;
	DWORD command = NEXT_PACKET;

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
