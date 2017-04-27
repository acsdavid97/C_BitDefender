#pragma once
#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H
#include "Everything.h"
#include "generic_hash_table.h"

typedef struct CredentialManagerTag{
	GenericHashTableT *hash_table;
}CredentialManagerT;

typedef struct CredentialTag {
	TCHAR *sUserName;
	TCHAR *sPassword;
	DWORD nrBytesEncrypted;
	BOOL bConnected;
}CredentialT;

unsigned int credential_hash(const CredentialT *credential);

CredentialManagerT* create_CredentialManagerT();
CredentialT* create_CredentialT(TCHAR *sUserName, TCHAR *sPassword);

CredentialManagerT* loadCredentialsFromFile(TCHAR *path);

BOOL insertCredentials(CredentialManagerT *manager, CredentialT *credential);

BOOL addBytesToClientAndDisconnect(CredentialManagerT *manager,TCHAR *clientName, DWORD nrBytes);

BOOL checkClientCredentials(CredentialManagerT *manager, CredentialT *credential);

VOID listCredentials(CredentialManagerT *manager);

void print_CredentialT(const void *a, FILE* file);
#endif