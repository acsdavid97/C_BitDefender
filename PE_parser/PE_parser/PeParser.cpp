/*	
 * Author: Ács Dávid
 * Version : 0.1
 * 
 * Description: Function implementations for parsing a MZ-PE Windows executable.
 * Date of Creation: 2017-05-13
 * 
 * Change log:
 * 2017-05-13: File created
 */

#include "PeParser.h"
#include <stdlib.h>

//machine code strings
const PTCHAR MACHINE_UNKOWN = _T("IMAGE_FILE_MACHINE_UNKNOWN/GENERAL");
const PTCHAR MACHINE_AM33 = _T("IMAGE_FILE_MACHINE_AM33");
const PTCHAR MACHINE_AMD64 = _T("IMAGE_FILE_MACHINE_AMD64");
const PTCHAR MACHINE_ARM = _T("IMAGE_FILE_MACHINE_ARM");
const PTCHAR MACHINE_ARMNT = _T("IMAGE_FILE_MACHINE_ARMNT");
const PTCHAR MACHINE_EBC = _T("IMAGE_FILE_MACHINE_EBC");
const PTCHAR MACHINE_I386 = _T("IMAGE_FILE_MACHINE_I386");
const PTCHAR MACHINE_IA64 = _T("IMAGE_FILE_MACHINE_IA64");
const PTCHAR MACHINE_M32R = _T("IMAGE_FILE_MACHINE_M32R");

//characteristic strings
const PTCHAR CHARAC_RELOCS_STRIPPED = _T("IMAGE_FILE_RELOCS_STRIPPED\n");
const PTCHAR CHARAC_EXECUTABLE_IMAGE = _T("IMAGE_FILE_EXECUTABLE_IMAGE\n");
const PTCHAR CHARAC_LINE_NUMS_STRIPPED = _T("IMAGE_FILE_LINE_NUMS_STRIPPED\n");
const PTCHAR CHARAC_LOCAL_SYSMS_STRIPPED = _T("IMAGE_FILE_LOCAL_SYMS_STRIPPED\n");
const PTCHAR CHARAC_AGGRESSIVE_WS_TRIM = _T("IMAGE_FILE_AGGRESSIVE_WS_TRIM\n");
const PTCHAR CHARAC_LARGE_ADDRESS_AWARE = _T("IMAGE_FILE_LARGE_ADDRESS_AWARE\n");
const PTCHAR CHARAC_BYTES_REVERSED_LO = _T("IMAGE_FILE_BYTES_REVERSED_LO\n");
const PTCHAR CHARAC_32BIT_MACHINE = _T("IMAGE_FILE_32BIT_MACHINE\n");
const PTCHAR CHARAC_DEBUG_STRIPPED = _T("IMAGE_FILE_DEBUG_STRIPPED\n");
const PTCHAR CHARAC_REMOVABLE_RUN_FROM_SWAP = _T("IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP\n");
const PTCHAR CHARAC_NET_RUN_FROM_SWAP = _T("IMAGE_FILE_NET_RUN_FROM_SWAP\n");
const PTCHAR CHARAC_FILE_SYSTEM = _T("IMAGE_FILE_SYSTEM\n");
const PTCHAR CHARAC_FILE_DLL = _T("IMAGE_FILE_DLL\n");
const PTCHAR CHARAC_UP_SYSYTEM_ONLY = _T("IMAGE_FILE_UP_SYSTEM_ONLY\n");
const PTCHAR CHARAC_BYTES_REVERSED_HI = _T("IMAGE_FILE_BYTES_REVERSED_HI\n");

/*
 * Returns the code of the machine in a human readable string
 * If the code does not correspond to a existing value, then the return value is NULL.
 */
PTCHAR
getMachineString(
	_In_ WORD machine // the code to be printed in human readable format.
)
{
	switch (machine)
	{
		case IMAGE_FILE_MACHINE_UNKNOWN:
			return MACHINE_UNKOWN;
		case IMAGE_FILE_MACHINE_AM33:
			return MACHINE_AM33;
		case IMAGE_FILE_MACHINE_AMD64:
			return MACHINE_AMD64;
		case IMAGE_FILE_MACHINE_ARM:
			return MACHINE_ARM;
		case IMAGE_FILE_MACHINE_ARMNT:
			return MACHINE_ARMNT;
		case IMAGE_FILE_MACHINE_EBC:
			return MACHINE_EBC;
		case IMAGE_FILE_MACHINE_I386:
			return MACHINE_I386;
		case IMAGE_FILE_MACHINE_IA64:
			return MACHINE_IA64;
		case IMAGE_FILE_MACHINE_M32R:
			return MACHINE_M32R;
		default:
			return NULL;
	}
}

/*
 * Prints to the standart output the human readable format of the caracteristic field.
 */
VOID
printCharacString(
	_In_ WORD characteristic //characterisitc to be printed in human reabable format.
)
{
	if (characteristic & IMAGE_FILE_RELOCS_STRIPPED)
	{
		_tprintf(CHARAC_RELOCS_STRIPPED);
	}
	if (characteristic & IMAGE_FILE_EXECUTABLE_IMAGE)
	{
		_tprintf(CHARAC_EXECUTABLE_IMAGE);
	}
	if (characteristic & IMAGE_FILE_LINE_NUMS_STRIPPED)
	{
		_tprintf(CHARAC_LINE_NUMS_STRIPPED);
	}
	if (characteristic & IMAGE_FILE_LOCAL_SYMS_STRIPPED)
	{
		_tprintf(CHARAC_LOCAL_SYSMS_STRIPPED);
	}
	if (characteristic & IMAGE_FILE_AGGRESIVE_WS_TRIM)
	{
		_tprintf(CHARAC_AGGRESSIVE_WS_TRIM);
	}
	if (characteristic & IMAGE_FILE_LARGE_ADDRESS_AWARE)
	{
		_tprintf(CHARAC_LARGE_ADDRESS_AWARE);
	}
	if (characteristic & IMAGE_FILE_BYTES_REVERSED_LO)
	{
		_tprintf(CHARAC_BYTES_REVERSED_LO);
	}
	if (characteristic & IMAGE_FILE_32BIT_MACHINE)
	{
		_tprintf(CHARAC_32BIT_MACHINE);
	}
	if (characteristic & IMAGE_FILE_DEBUG_STRIPPED)
	{
		_tprintf(CHARAC_DEBUG_STRIPPED);
	}
	if (characteristic & IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP)
	{
		_tprintf(CHARAC_REMOVABLE_RUN_FROM_SWAP);
	}
	if (characteristic & IMAGE_FILE_NET_RUN_FROM_SWAP)
	{
		_tprintf(CHARAC_NET_RUN_FROM_SWAP);
	}
	if (characteristic & IMAGE_FILE_SYSTEM)
	{
		_tprintf(CHARAC_FILE_SYSTEM);
	}
	if (characteristic & IMAGE_FILE_DLL)
	{
		_tprintf(CHARAC_FILE_DLL);
	}
	if (characteristic & IMAGE_FILE_UP_SYSTEM_ONLY)
	{
		_tprintf(CHARAC_UP_SYSYTEM_ONLY);
	}
	if (characteristic & IMAGE_FILE_BYTES_REVERSED_HI)
	{
		_tprintf(CHARAC_BYTES_REVERSED_HI);
	}
}

/*
 * Parses the IMAGE_FILE_HEADER of the memory mapped executable.
 * Prints the machine code, number of sections and characterisitics both in hexa and
 * human readable form.
 * Returns an ERROR_CODE
 */
ERROR_CODE
ParseImageFileHeader(
	_In_ PIMAGE_FILE_HEADER pImageFileHeader //pointer to the header to be parsed.
)
{
	PTCHAR pszMachine = NULL;

	pszMachine = getMachineString(pImageFileHeader->Machine);
	if (pszMachine == NULL)
	{
		return INVALID_MACHINE_CODE;
	}

	_tprintf(_T("Machine code: %#06x, name: %s\n"), pImageFileHeader->Machine, pszMachine);
	_tprintf(_T("Number of Sections: %d\n"), pImageFileHeader->NumberOfSections);
	_tprintf(_T("Characteristics: %#06x\n"), pImageFileHeader->Characteristics);
	printCharacString(pImageFileHeader->Characteristics);

	return SUCCESS;
}

/*
 * Parses the IMAGE_OPTIONAL_HEADER of the memory mapped executable.
 * Returns an ERROR_CODE
 */
ERROR_CODE
ParseImageOptionalHeader(
	_In_ PIMAGE_OPTIONAL_HEADER pImageOptionalHeader // pointer to header to be parsed.
)
{
	if (pImageOptionalHeader->Magic != 0x10b && pImageOptionalHeader->Magic != 0x107)
	{
		return INVALID_PE_FILE;
	}

	_tprintf(_T("Address of entry point: %#010x\n"), pImageOptionalHeader->AddressOfEntryPoint);
	_tprintf(_T("Image base: %#010x\n"), pImageOptionalHeader->ImageBase);
	//TODO: subsystem!!!
	_tprintf(_T("Section alignment: %#010x\n"), pImageOptionalHeader->SectionAlignment);
	_tprintf(_T("File alignment: %#010x\n"), pImageOptionalHeader->FileAlignment);
	_tprintf(_T("Number of Rva and Sizes: %d\n"), pImageOptionalHeader->NumberOfRvaAndSizes);

	return SUCCESS;
}

ERROR_CODE
ParseSectionHeader(
	_In_ PIMAGE_SECTION_HEADER pSectionHeader // the address of the section to be parsed.
)
{
	CHAR name[9];

	for(INT i = 0; i < 8; i++)
	{
		name[i] = pSectionHeader->Name[i];
	}
	name[8] = '\0';

	printf("section name: %s\n", name);
	_tprintf(_T("address: %#010x\n"), pSectionHeader->VirtualAddress);
	_tprintf(_T("size on disk: %d\n"), pSectionHeader->SizeOfRawData);
	_tprintf(_T("size in memory: %d\n"), pSectionHeader->Misc.VirtualSize);

	return SUCCESS;
}

ERROR_CODE
ParseMappedPEFile(
	_In_ LPVOID lpvMappedAddr // the address of the Mapping of the file to be parsed.
)
{
	PIMAGE_DOS_HEADER pDOSHeader = NULL;
	PIMAGE_NT_HEADERS pNTHeaders = NULL;
	PIMAGE_FILE_HEADER pImageFileHeader = NULL;
	PIMAGE_OPTIONAL_HEADER pImageOptionalHeader = NULL;
	ERROR_CODE errorCode = SUCCESS;
	PIMAGE_SECTION_HEADER pSectionHeader = NULL;

	pDOSHeader = (PIMAGE_DOS_HEADER)lpvMappedAddr;
	if (pDOSHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return INVALID_PE_FILE;
	}

	pNTHeaders = (PIMAGE_NT_HEADERS)((LONGLONG)lpvMappedAddr + pDOSHeader->e_lfanew);
	if (pNTHeaders->Signature != IMAGE_NT_SIGNATURE)
	{
		return INVALID_PE_FILE;
	}

	pImageFileHeader = &pNTHeaders->FileHeader;
	pImageOptionalHeader = &pNTHeaders->OptionalHeader;

	_tprintf(_T("\nFile Header:\n"));
	errorCode = ParseImageFileHeader(pImageFileHeader);
	if (errorCode != SUCCESS)
	{
		return errorCode;
	}

	_tprintf(_T("\nFile Optional Header:\n"));
	errorCode = ParseImageOptionalHeader(pImageOptionalHeader);
	if (errorCode != SUCCESS)
	{
		return errorCode;
	}

	_tprintf(_T("\nSection headers:\n"));
	for(DWORD i = 0; i < pImageFileHeader->NumberOfSections; i++)
	{
		_tprintf(_T("\nsection number: %d\n"), i);
		pSectionHeader = (PIMAGE_SECTION_HEADER)((LONGLONG)pImageOptionalHeader 
			+ sizeof(IMAGE_OPTIONAL_HEADER) + i * sizeof(IMAGE_SECTION_HEADER));

		ParseSectionHeader(pSectionHeader);
	}

	return SUCCESS;
}
