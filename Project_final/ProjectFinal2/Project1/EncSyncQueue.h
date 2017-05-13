#pragma once

#ifndef ENCRYPT_DATA_H
#define ENCRYPT_DATA_H

#include "Everything.h"

typedef struct EncryptDataTag{
	PTCHAR toBeEncrypted;
	DWORD dwBuffLen;
	PTCHAR encryptionKey;
	DWORD dwKeyLen;
	LPCRITICAL_SECTION pCriticalSection;
	PCONDITION_VARIABLE pConditionVariable;
	DWORD dwStatus;
	DWORD dwIndex;
}EncryptDataT, *LPEncryptDataT;

typedef struct SyncCircQueueTag {
	DWORD dwHead;
	DWORD dwTail;
	LPEncryptDataT *data;
	DWORD dwSize;
	CRITICAL_SECTION criticalSection;
	CONDITION_VARIABLE cvNotFull;
	CONDITION_VARIABLE cvNotEmpty;
}SyncCircQueueT, *LPSyncCircQueueT;

LPEncryptDataT create_EcryptData(
	HANDLE hHeap, 
	PTCHAR toBeEncrypted,
	DWORD dwBufflen,
	PTCHAR encryptionKey,
	DWORD dwKeyLen,
	LPCRITICAL_SECTION pCriticalSection,
	PCONDITION_VARIABLE pConditionVariable
);

VOID free_EncryptData(HANDLE hHeap, LPEncryptDataT lpEncryptData);

VOID pushSyncQueue(LPSyncCircQueueT queue, LPEncryptDataT data);

VOID popSyncQueue(LPSyncCircQueueT queue, LPEncryptDataT *data);

LPSyncCircQueueT create_SyncCircQueueT(DWORD dwSize);

#endif
