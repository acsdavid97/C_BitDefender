#define _CRT_SECURE_NO_WARNINGS

#include "EncSyncQueue.h"
#include "../Client/CommunicationProtocol.h"

LPEncryptDataT create_EcryptData(
	HANDLE hHeap, 
	PTCHAR toBeEncrypted,
	DWORD dwBufflen,
	PTCHAR encryptionKey,
	DWORD dwKeyLen,
	LPCRITICAL_SECTION pCriticalSection,
	PCONDITION_VARIABLE pConditionVariable)
{
	LPEncryptDataT pEncryptData = (LPEncryptDataT)HeapAlloc(
		hHeap,
		0,
		sizeof(EncryptDataT)
	);
	if (pEncryptData == NULL) {
		return NULL;
	}

	pEncryptData->toBeEncrypted = (PTCHAR) HeapAlloc(hHeap, 0, dwBufflen * sizeof(TCHAR));
	if (pEncryptData->toBeEncrypted == NULL) {
		HeapFree(hHeap, 0, pEncryptData);
		return NULL;
	}

	pEncryptData->encryptionKey = (PTCHAR) HeapAlloc(hHeap, 0, dwKeyLen * sizeof(TCHAR));
	if (pEncryptData->encryptionKey == NULL) {
		HeapFree(hHeap, 0, pEncryptData->toBeEncrypted);
		HeapFree(hHeap, 0, pEncryptData);
		return NULL;
	}

	_tcscpy(pEncryptData->toBeEncrypted, toBeEncrypted);

	_tcscpy(pEncryptData->encryptionKey, encryptionKey);

	pEncryptData->dwBuffLen = dwBufflen;
	pEncryptData->dwKeyLen = dwKeyLen;
	pEncryptData->dwStatus = DATA_NOT_ENCRYPTED;
	pEncryptData->pCriticalSection = pCriticalSection;
	pEncryptData->pConditionVariable = pConditionVariable;

	return pEncryptData;
}

VOID free_EncryptData(HANDLE hHeap, LPEncryptDataT lpEncryptData)
{
	HeapFree(hHeap, 0, lpEncryptData->encryptionKey);

	HeapFree(hHeap, 0, lpEncryptData->toBeEncrypted);

	HeapFree(hHeap, 0, lpEncryptData);
}
VOID pushSyncQueue(LPSyncCircQueueT queue, LPEncryptDataT data)
{
	EnterCriticalSection(&queue->criticalSection);

	while (queue->dwTail + 1 == queue->dwHead || (queue->dwHead == 0 && queue->dwTail == queue->dwSize - 1)) {
		//queue is full
		SleepConditionVariableCS(&queue->cvNotFull, &queue->criticalSection, INFINITE);
	}

	queue->data[queue->dwTail] = data;
	queue->dwTail = (queue->dwTail + 1) % queue->dwSize;

	LeaveCriticalSection(&queue->criticalSection);

	WakeConditionVariable(&queue->cvNotEmpty);
}

VOID popSyncQueue(LPSyncCircQueueT queue, LPEncryptDataT *data)
{
	EnterCriticalSection(&queue->criticalSection);

	while(queue->dwTail == queue->dwHead) {
		//queue is empty
		SleepConditionVariableCS(&queue->cvNotEmpty, &queue->criticalSection, INFINITE);
	}

	*data = queue->data[queue->dwHead];
	queue->dwHead = (queue->dwHead + 1) % queue->dwSize;
	LeaveCriticalSection(&queue->criticalSection);

	WakeConditionVariable(&queue->cvNotFull);
}

LPSyncCircQueueT create_SyncCircQueueT(DWORD dwSize)
{
	LPSyncCircQueueT queue = (LPSyncCircQueueT)malloc(sizeof(SyncCircQueueT));
	if (queue == NULL) {
		return NULL;
	}

	queue->data = (LPEncryptDataT*)malloc(sizeof(LPEncryptDataT) * dwSize);
	if (queue->data == NULL) {
		free(queue);
		return NULL;
	}

	queue->dwSize = dwSize;
	queue->dwHead = queue->dwTail = 0;
	InitializeCriticalSection(&queue->criticalSection);
	InitializeConditionVariable(&queue->cvNotEmpty);
	InitializeConditionVariable(&queue->cvNotFull);

	return queue;
}
