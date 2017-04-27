#pragma once

#ifndef ENCRYPT_DATA_H
#define ENCRYPT_DATA_H

#include "Everything.h"

typedef struct EncryptDataTag{
	TCHAR *toBeEncrypted;
	DWORD dwBuffLen;
	TCHAR *encryptionKey;
	DWORD dwKeyLen;
	DWORD dwStartPos;
	CRITICAL_SECTION *criticalSection;
	CONDITION_VARIABLE *conditionVariable;
	DWORD dwStatus;
}EncryptDataT, *LPEncryptDataT;

typedef struct SyncCircQueueTag {
	DWORD dwHead;
	DWORD dwTail;
	EncryptDataT **data;
	DWORD dwSize;
	CRITICAL_SECTION criticalSection;
	CONDITION_VARIABLE cvNotFull;
	CONDITION_VARIABLE cvNotEmpty;
}SyncCircQueueT, *LPSyncCircQueueT;

VOID pushSyncQueue(LPSyncCircQueueT queue, LPEncryptDataT data);

VOID popSyncQueue(LPSyncCircQueueT queue, LPEncryptDataT *data);

LPSyncCircQueueT create_SyncCircQueueT(DWORD dwSize);

#endif
