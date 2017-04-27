#define _CRT_SECURE_NO_WARNINGS

#include "UserManagement.h"

unsigned int credential_hash(const CredentialT *credential) 
{
	unsigned int hash = 5381;
	int c;

	TCHAR *walker = credential->sUserName;
	while ((c = *walker++) != '\0') {
		hash = ((hash << 5) + hash) + c;
	}
	return hash;
}

int compare_credential(const CredentialT *a, const CredentialT *b)
{
	return _tcscmp(a->sUserName, b->sUserName);
}

CredentialManagerT* create_CredentialManagerT()
{
	CredentialManagerT* manager = (CredentialManagerT*)malloc(sizeof(CredentialManagerT));
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

CredentialT* create_CredentialT(TCHAR *sUserName, TCHAR *sPassword)
{
	CredentialT* credential = (CredentialT*)malloc(sizeof(CredentialT));
	if (credential == NULL) {
		return NULL;
	}

	credential->sUserName = (TCHAR*)malloc(sizeof(TCHAR) * _tcslen(sUserName));
	if (credential->sUserName == NULL) {
		free(credential);
		return NULL;
	}

	credential->sPassword = (TCHAR*)malloc(sizeof(TCHAR) * _tcslen(sPassword));
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

CredentialManagerT* loadCredentialsFromFile(TCHAR *path)
{
	TCHAR buffer[1024];
	FILE *file = _tfopen(path, _T("r"));
	if (file == NULL) {
		return NULL;
	}

	CredentialManagerT* manager = create_CredentialManagerT();
	if (manager == NULL) {
		return NULL;
	}

	while(_fgetts(buffer, 1024, file) != NULL) {
		TCHAR *sUserName = _tcstok(buffer, _T(";\n"));
		TCHAR *sPassword = _tcstok(NULL, _T(";\n"));
		CredentialT *credential = create_CredentialT(sUserName, sPassword);

		insertCredentials(manager, credential);
	}

	fclose(file);
	return manager;
}

BOOL insertCredentials(CredentialManagerT *manager, CredentialT *credential)
{
	if (search_element_in_hash_table(manager->hash_table, credential,
		(int(*)(const void*, const void*)) compare_credential) != NULL) {
			return FALSE;
	}
	add_element_in_hash_table(manager->hash_table, credential);
	return TRUE;
}

BOOL checkClientCredentials(CredentialManagerT *manager, CredentialT *credential)
{
	CredentialT* found_cred = (CredentialT*)search_element_in_hash_table(manager->hash_table, credential,
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

BOOL addBytesToClientAndDisconnect(CredentialManagerT *manager,TCHAR *clientName, DWORD nrBytes)
{
	CredentialT* credential = create_CredentialT(clientName, _T("asdf"));
	
	CredentialT* found_cred = (CredentialT*)search_element_in_hash_table(manager->hash_table, credential,
		(int(*)(const void*, const void*)) compare_credential);

	if (found_cred == NULL) {
		return FALSE;
	}

	found_cred->nrBytesEncrypted += nrBytes;
	found_cred->bConnected = FALSE;

	return TRUE;
}

VOID listCredentials(CredentialManagerT *manager)
{
	print_elements_in_hash_table(manager->hash_table, stdout, print_CredentialT);
}

void print_CredentialT(const void *e, FILE* file)
{
	CredentialT *a = (CredentialT*)e;
	_ftprintf(file, _T("name: %s, pass: %s, bytes: %lu, connected: %d"), a->sUserName, a->sPassword, a->nrBytesEncrypted, a->bConnected);
}
