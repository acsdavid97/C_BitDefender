/*	
 * Author: Ács Dávid
 * Version : 0.2
 * 
 * Description: Function implementations for parsing a MZ-PE Windows executable.
 * Date of Creation: 2017-05-13
 * 
 * Change log:
 * 2017-05-13: File created
 * 2017-05-14: Variables renamed, to correctly reflect hungarian notation.
 */

#include "PeParser.h"


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
	LPCTSTR pszMachine;

	pszMachine = GetMachineString(pImageFileHeader->Machine);
	if (pszMachine == NULL)
	{
		return INVALID_MACHINE_CODE;
	}

	_tprintf(_T("Machine code: %#06x, name: %s\n"), pImageFileHeader->Machine, pszMachine);
	_tprintf(_T("Number of Sections: %d\n"), pImageFileHeader->NumberOfSections);
	_tprintf(_T("Characteristics: %#06x\n"), pImageFileHeader->Characteristics);
	PrintCharacString(pImageFileHeader->Characteristics);

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
	LPCTSTR pszSubsystem;

	if (pImageOptionalHeader->Magic != 0x10b && pImageOptionalHeader->Magic != 0x107)
	{
		return INVALID_PE_FILE;
	}

	pszSubsystem = GetSubsystemString(pImageOptionalHeader->Subsystem);
	if (pszSubsystem == NULL)
	{
		return INVALID_SUBSYSTEM_CODE;
	}


	_tprintf(_T("Address of entry point: %#010x\n"), pImageOptionalHeader->AddressOfEntryPoint);
	_tprintf(_T("Image base: %#010x\n"), pImageOptionalHeader->ImageBase);
	_tprintf(_T("Subsystem: %s\n"), pszSubsystem);
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
	CHAR acName[9];

	memcpy(acName, pSectionHeader->Name, 8);
	acName[8] = '\0';

	printf("section name: %s\n", acName);
	_tprintf(_T("address: %#010x\n"), pSectionHeader->VirtualAddress);
	_tprintf(_T("size on disk: %d\n"), pSectionHeader->SizeOfRawData);
	_tprintf(_T("size in memory: %d\n"), pSectionHeader->Misc.VirtualSize);

	return SUCCESS;
}

VOID
ParseSectionHeaders(
	_In_ PIMAGE_NT_HEADERS pNtHeaders, // pointer to NT headers
	_In_ WORD wNrSections // number of sections to parse
)
{
	WORD i;
	PIMAGE_SECTION_HEADER pSectionHeader;

	for(i = 0; i < wNrSections; i++)
	{
		_tprintf(_T("\nsection number: %d\n"), i);
		//no need to check for failure, since go from 0 to wNrSections
		pSectionHeader = GetSectionHeader(pNtHeaders, i);

		ParseSectionHeader(pSectionHeader);
	}
}


ERROR_CODE
ParseExportedFunctions(
	_In_ PIMAGE_NT_HEADERS pNtHeaders,
	_In_ PVOID pvMappingAddress
) 
{
	EXPORT_DIR_VA exportDirVa;
	ERROR_CODE errorCode;
	DWORD i;
	WORD wOrdinal;
	PCHAR pszFunctionName;

	errorCode = GetExportDirectoryInVA(
		pNtHeaders,
		pvMappingAddress,
		&exportDirVa
	);

	if (errorCode != SUCCESS)
	{
		return errorCode;
	}

	printf("name of exports: %s\n", exportDirVa.pcName);
	_tprintf(_T("base of ordinals: %d\n"), exportDirVa.pExportDirectory->Base);

	for(i = 0; i < exportDirVa.pExportDirectory->NumberOfFunctions; i++)
	{
		wOrdinal = exportDirVa.pOrdinals[i];
		_tprintf(_T("\ni: %d, Ordinal: %d, "), i,  wOrdinal);

		if (wOrdinal < exportDirVa.pExportDirectory->NumberOfNames)
		{
			pszFunctionName = (PCHAR)RvaToVa(
				pNtHeaders,
				pvMappingAddress,
				exportDirVa.pNameRVAs[i]
			);
			printf("name: %s\n", pszFunctionName);
		}

		_tprintf(_T("address: %#010x\n"), exportDirVa.pAddresses[wOrdinal]);
	}

	return SUCCESS;
}

ERROR_CODE
ParseMappedPEFile(
	_In_ PFILE_MAPPING pFileMapping // the address of the Mapping of the file to be parsed.
)
{
	//todo break into subfunctions
	PIMAGE_DOS_HEADER pDOSHeader;
	PIMAGE_NT_HEADERS pNTHeaders;
	PIMAGE_FILE_HEADER pImageFileHeader;
	PIMAGE_OPTIONAL_HEADER pImageOptionalHeader;
	PVOID pvMappedAddr = pFileMapping->pvMappingAddress;
	ERROR_CODE errorCode;

	pDOSHeader = (PIMAGE_DOS_HEADER)pvMappedAddr;
	if (pDOSHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return INVALID_PE_FILE;
	}

	pNTHeaders = (PIMAGE_NT_HEADERS)((ULONGLONG)pvMappedAddr + pDOSHeader->e_lfanew);
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
	ParseSectionHeaders(pNTHeaders, pImageFileHeader->NumberOfSections);

	if (pImageOptionalHeader->NumberOfRvaAndSizes <= IMAGE_DIRECTORY_ENTRY_EXPORT)
	{
		return EXPORT_TABLE_MISSING;
	}

	errorCode = SUCCESS;
	/*
	errorCode = ParseExportedFunctions(
		pNTHeaders,
		pvMappedAddr
	);
	*/

	if (errorCode != SUCCESS)
	{
		PrintErrorCode(errorCode);
	}

	if (pImageOptionalHeader->NumberOfRvaAndSizes <= IMAGE_DIRECTORY_ENTRY_IMPORT)
	{
		return IMPORT_TABLE_MISSING;
	}

	return SUCCESS;
}
