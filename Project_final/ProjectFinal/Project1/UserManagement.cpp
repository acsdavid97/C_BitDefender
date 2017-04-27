#define _CRT_SECURE_NO_WARNINGS

#include "UserManagement.h"

unsigned int credential_hash(const LPCredentialT credential) 
{
	unsigned int hash = 5381;
	int c;

	TCHAR *walker = credential->sUserName;
	while ((c = *walker++) != '\0') {
		hash = ((hash << 5) + hash) + c;
	}
	return hash;
}

int compare_credential(const LPCredentialT a, const LPCredentialT b)
{
	return _tcscmp(a->sUserName, b->sUserName);
}

LPCredentialManagerT create_CredentialManagerT()
{
	LPCredentialManagerT manager = (LPCredentialManagerT)malloc(sizeof(CredentialManagerT));
	if (manager == NULL) {
		return NULL;
	}

	manager->hash_table = create_empty_GenericHashTableT((unsigned(*)(const void*))credential_hash);
	if (manager->hash_table == NULL) {
		free(manager);
		return NULL;
	}

	return manager;
}

LPCredentialT create_CredentialT(PTCHAR sUserName, PTCHAR sPassword)
{
	LPCredentialT credential = (LPCredentialT)malloc(sizeof(CredentialT));
	if (credential == NULL) {
		return NULL;
	}

	credential->sUserName = (PTCHAR)malloc(sizeof(TCHAR) * _tcslen(sUserName));
	if (credential->sUserName == NULL) {
		free(credential);
		return NULL;
	}

	credential->sPassword = (PTCHAR)malloc(sizeof(TCHAR) * _tcslen(sPassword));
	if (credential->sPassword == NULL) {
		free(credential->sUserName);
		free(credential);
		return NULL;
	}
	_tcscpy(credential->sUserName, sUserName);
	_tcscpy(credential->sPassword, sPassword);
	credential->nrBytesEncrypted = 0;
	credential->bConnected = FALSE;

	return credential;
}

LPCredentialManagerT loadCredentialsFromFile(PTCHAR path)
{
	TCHAR buffer[1024];
	FILE *file = _tfopen(path, _T("r"));
	if (file == NULL) {
		return NULL;
	}

	LPCredentialManagerT manager = create_CredentialManagerT();
	if (manager == NULL) {
		return NULL;
	}

	while(_fgetts(buffer, 1024, file) != NULL) {
		PTCHAR sUserName = _tcstok(buffer, _T(";\n"));
		PTCHAR sPassword = _tcstok(NULL, _T(";\n"));
		CredentialT *credential = create_CredentialT(sUserName, sPassword);

		insertCredentials(manager, credential);
	}

	fclose(file);
	return manager;
}

BOOL insertCredentials(LPCredentialManagerT manager, LPCredentialT credential)
{
	if (search_element_in_hash_table(manager->hash_table, credential,
		(int(*)(const void*, const void*)) compare_credential) != NULL) {
			return FALSE;
	}
	add_element_in_hash_table(manager->hash_table, credential);
	return TRUE;
}

BOOL checkClientCredentials(LPCredentialManagerT manager, LPCredentialT credential)
{
	LPCredentialT found_cred = (LPCredentialT)search_element_in_hash_table(manager->hash_table, credential,
		(int(*)(const void*, const void*)) compare_credential);

	if (found_cred == NULL) {
		return FALSE;
	}
	BOOL bValid = _tcscmp(found_cred->sPassword, credential->sPassword) == 0;
	if (bValid) {
		found_cred->bConnected = TRUE;
	}

	return bValid;
}

BOOL addBytesToClientAndDisconnect(LPCredentialManagerT manager, PTCHAR clientName, DWORD nrBytes)
{
	LPCredentialT credential = create_CredentialT(clientName, _T("asdf"));
	
	LPCredentialT found_cred = (LPCredentialT)search_element_in_hash_table(manager->hash_table, credential,
		(int(*)(const void*, const void*)) compare_credential);

	if (found_cred == NULL) {
		return FALSE;
	}

	found_cred->nrBytesEncrypted += nrBytes;
	found_cred->bConnected = FALSE;

	return TRUE;
}

VOID listCredentials(LPCredentialManagerT manager)
{
	print_elements_in_hash_table(manager->hash_table, stdout, print_CredentialT);
}

void print_CredentialT(const void *e, FILE* file)
{
	LPCredentialT a = (LPCredentialT)e;
	_ftprintf(file, _T("name: %s, pass: %s, bytes: %lu, connected: %d"), a->sUserName, a->sPassword, a->nrBytesEncrypted, a->bConnected);
}
