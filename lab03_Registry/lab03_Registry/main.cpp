#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

void QueryKey(HKEY hKey);

void listRegistryEntry(HKEY hKey, LPCSTR achRegKey)
{
	HKEY hTestKey;

	if (RegOpenKeyEx(
		hKey,
		achRegKey,
		0,
		KEY_READ,
		&hTestKey) == ERROR_SUCCESS) {
		QueryKey(hTestKey);
	}
	RegCloseKey(hTestKey);
}

void _tmain1(void)
{
	char executableToRun[9] = "calc.exe";
	HKEY hKey;
	if (RegOpenKeyEx(
		HKEY_CURRENT_USER, 
		TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 
		0,
		KEY_SET_VALUE,
		&hKey) == ERROR_SUCCESS) {

		if (RegSetValueEx(hKey,
			TEXT("runcalc"),
			0,
			REG_SZ,
			(BYTE*)executableToRun,
			9) != ERROR_SUCCESS) {
			_tprintf(TEXT("error"));
			return;
		}
	} else {

	}
	RegCloseKey(hKey);

	listRegistryEntry(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"));
	listRegistryEntry(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"));
	listRegistryEntry(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce"));
	listRegistryEntry(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce"));
	system("pause");
}

void QueryKey(HKEY hKey)
{
	TCHAR achKey[MAX_KEY_LENGTH]; //buffer for subkey name
	DWORD cbName; //size of name string
	TCHAR achClass[MAX_PATH] = TEXT("");
	DWORD cchClassName = MAX_PATH;
	DWORD cSubKeys = 0;
	DWORD cbMaxSubKey;
	DWORD cchMaxClass;
	DWORD cValues;
	DWORD cchMaxValue;
	DWORD cbMaxValueData;
	DWORD cbSecurityDescriptor;
	FILETIME ftLastWriteTime;

	DWORD i, retCode;
	TCHAR achValue[MAX_VALUE_NAME];
	DWORD cchValue = MAX_VALUE_NAME;

	//get the class name and the value count.
	retCode = RegQueryInfoKey(
		hKey,
		achClass,
		&cchClassName,
		NULL,
		&cSubKeys,
		&cbMaxSubKey,
		&cchMaxClass,
		&cValues,
		&cchMaxValue,
		&cchMaxValue,
		&cbSecurityDescriptor,
		&ftLastWriteTime);

	if (cSubKeys) {
		_tprintf(TEXT("\nNumber of subkeys: %d\n"), cSubKeys);
		for (i = 0; i < cSubKeys; i++) {
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(hKey, i, achKey, &cbName,
			                       NULL, NULL, NULL, &ftLastWriteTime);
			if (retCode == ERROR_SUCCESS) {
				_tprintf(TEXT("(%d) %s\n"), i + 1, achKey);
			}
		}
	}
	else {
		_tprintf(TEXT("\nno keys\n"));
	}

	if (cValues) {
		_tprintf(TEXT("\nNumber of values2 :%d\n"), cValues);

		for (i = 0 , retCode = ERROR_SUCCESS; i < cValues; i++) {
			cchValue = MAX_VALUE_NAME;
			achValue[0] = '\0';
			retCode = RegEnumValue(hKey, i, achValue, &cchValue,
			                       NULL, NULL, NULL, NULL);

			if (retCode == ERROR_SUCCESS) {
				_tprintf(TEXT("(%d) %s\n"), i + 1, achValue);
			}
		}
	}
	else {
		_tprintf(TEXT("\nno values\n"));
	}
}
