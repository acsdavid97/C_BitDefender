#include "EncSyncQueue.h"

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
