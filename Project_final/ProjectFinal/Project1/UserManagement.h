#pragma once

#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include "Everything.h"
#include "generic_hash_table.h"

typedef struct CredentialManagerTag{
	GenericHashTableT *hash_table;
}CredentialManagerT, *LPCredentialManagerT;

typedef struct CredentialTag {
	PTCHAR sUserName;
	PTCHAR sPassword;
	DWORD nrBytesEncrypted;
	BOOL bConnected;
}CredentialT, *LPCredentialT;

unsigned int credential_hash(const LPCredentialT credential);

LPCredentialManagerT create_CredentialManagerT();
LPCredentialT create_CredentialT(PTCHAR sUserName, PTCHAR sPassword);

LPCredentialManagerT loadCredentialsFromFile(PTCHAR path);

BOOL insertCredentials(LPCredentialManagerT manager, LPCredentialT credential);

BOOL addBytesToClientAndDisconnect(LPCredentialManagerT manager,PTCHAR clientName, DWORD nrBytes);

BOOL checkClientCredentials(LPCredentialManagerT manager, LPCredentialT credential);

VOID listCredentials(LPCredentialManagerT manager);

void print_CredentialT(const void *a, FILE* file);

#endif