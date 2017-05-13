#ifndef COMMUNICATION_PROTOCOL_H
#define  COMMUNICATION_PROTOCOL_H
#include "Everything.h"

typedef enum CommandEnum {
	INITIALIZE_CONNECTION, CONNECTION_ACCEPTED, CONNECTION_REJECTED,
	AUTHENTICATE, AUTH_SUCCESSFUL, AUTH_REJECTED, 
	ENCRYPT_DATA, LAST_PACKET, NEXT_PACKET, 
	DATA_ENCRYPTED, DATA_NOT_ENCRYPTED,
	TERMINATE_CONNECTION
}CommandE;

typedef struct InitStruct {
	CommandE command;
	DWORD cbUsernameNrBytes;
	DWORD cbPasswordNrBytes;
	DWORD cbKeyNrBytes;
}InitT, *LPInitT;

/*
 * Gets the next packet from the pipe.
 * 
 * @param hPipe: handle to the pipe
 * @param buff: buffer where the packet will be stored, it is 0 when LAST_PACKET is the command.
 * @param pcbPacketSize: where the size of the packet will be after successful operation.
 * @return if operation successful
*/
BOOL getNextPacket(HANDLE hPipe, PTCHAR buff, LPDWORD pcbPacketSize);

/*
 * Sends the next packet to the pipe.
 * 
 * @param hPipe: handle to the pipe
 * @param buff: buffer where the packet to be sent is stored.
 * @param cbPacketLen: the size of the packet to be sent.
 * @return if operation successful or not.
 */
BOOL sendPacket(HANDLE hPipe, PTCHAR buff, DWORD cbPacketLen);


#endif
