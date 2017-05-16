/*	
 * Author: Ács Dávid
 * Version : 0.3
 * 
 * Description: Function implementations for parsing a MZ-PE Windows executable.
 * Date of Creation: 2017-05-13
 * 
 * Change log:
 * 2017-05-13: File created
 * 2017-05-14: Variables renamed, to correctly reflect hungarian notation.
 * 2017-05-14: Safer approach: - always checking memory bounds by CheckAdrressRange
 *							   - using PFILE_MAPPING structure became inevitable to achieve this.
 * 2017-05-14: Output more readable
 * 2017-05-15: Import parsing corrected and split into smaller functions.
 * 2017-05-15: 64 bit executable support added, but program needs recompilation with _WIN64 defined! yay
 * 2017-05-15: MapPEFileInMemory function implementation
 */

#include "PeParser.h"

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

	_tprintf(_T("    Machine code: %#06x, name: %s\n"), pImageFileHeader->Machine, pszMachine);
	_tprintf(_T("    Number of Sections: %u\n"), pImageFileHeader->NumberOfSections);
	_tprintf(_T("    Characteristics: %#06x\n"), pImageFileHeader->Characteristics);
	PrintCharacString(pImageFileHeader->Characteristics);

	return SUCCESS;
}

ERROR_CODE
ParseImageOptionalHeader(
	_In_ PIMAGE_OPTIONAL_HEADER pImageOptionalHeader // pointer to header to be parsed.
)
{
	LPCTSTR pszSubsystem;

	if (pImageOptionalHeader->Magic != IMAGE_NT_OPTIONAL_HDR_MAGIC )
	{
		return INVALID_PE_FILE;
	}

	pszSubsystem = GetSubsystemString(pImageOptionalHeader->Subsystem);
	if (pszSubsystem == NULL)
	{
		return INVALID_SUBSYSTEM_CODE;
	}

	_tprintf(_T("      Address of entry point: %#010x\n"), pImageOptionalHeader->AddressOfEntryPoint);
#ifdef _WIN64
	_tprintf(_T("      Image base: %#018llx\n"), pImageOptionalHeader->ImageBase);
#else
	_tprintf(_T("      Image base: %#010x\n"), pImageOptionalHeader->ImageBase);
#endif
	_tprintf(_T("      Subsystem: %s\n"), pszSubsystem);
	_tprintf(_T("      Section alignment: %#010x\n"), pImageOptionalHeader->SectionAlignment);
	_tprintf(_T("      File alignment: %#010x\n"), pImageOptionalHeader->FileAlignment);
	_tprintf(_T("      Number of Rva and Sizes: %u\n"), pImageOptionalHeader->NumberOfRvaAndSizes);

	return SUCCESS;
}

VOID
ParseSectionHeader(
	_In_ PIMAGE_SECTION_HEADER pSectionHeader // the address of the section to be parsed.
)
{
	CHAR acName[9];

	memcpy(acName, pSectionHeader->Name, 8);
	acName[8] = '\0';

	printf("  section name: %s\n", acName);
	_tprintf(_T("      VirtualSize: %u (in hexa %#010x)\n"), pSectionHeader->Misc.VirtualSize, pSectionHeader->Misc.VirtualSize);
	_tprintf(_T("      VirtualAddress: %#010x\n"), pSectionHeader->VirtualAddress);
	_tprintf(_T("      SizeOfRawData: %u (in hexa: %#010x)\n"), pSectionHeader->SizeOfRawData, pSectionHeader->SizeOfRawData);
	_tprintf(_T("      PointerToRawData: %u (in hexa: %#010x)\n"), pSectionHeader->PointerToRawData, pSectionHeader->PointerToRawData);
}

ERROR_CODE
ParseSectionHeaders(
	_In_ PFILE_MAPPING pFileMapping, // pointer to file mapping
	_In_ WORD wNrSections // number of sections to parse
)
{
	WORD i;
	PIMAGE_SECTION_HEADER pSectionHeader;

	for(i = 0; i < wNrSections; i++)
	{
		_tprintf(_T("\n  section number: %d\n"), i);
		pSectionHeader = GetSectionHeader(pFileMapping, i);
		if (pSectionHeader == NULL)
		{
			return INVALID_PE_FILE;
		}

		ParseSectionHeader(pSectionHeader);
	}

	return SUCCESS;
}


ERROR_CODE
ParseExportedFunctions(
	_In_ PFILE_MAPPING pFileMapping
) 
{
	EXPORT_DIR_VA exportDirVa;
	ERROR_CODE errorCode;
	DWORD i;
	WORD wOrdinal;
	PCHAR pszFunctionName;

	errorCode = GetExportDirectoryInVA(
		pFileMapping,
		&exportDirVa
	);
	if (errorCode != SUCCESS)
	{
		return errorCode;
	}

	printf("  name of exports: %s\n", exportDirVa.pcName);
	_tprintf(_T("  base of ordinals: %d\n"), exportDirVa.pExportDirectory->Base);

	// for each fucntion with ordinal
	for(i = 0; i < exportDirVa.pExportDirectory->NumberOfNames; i++)
	{
		wOrdinal = exportDirVa.pOrdinals[i];
		_tprintf(_T("\n      i: %d\n"), i);
		_tprintf(_T("      Ordinal: %u (in hexa %#010x)\n"), wOrdinal, wOrdinal);

		pszFunctionName = (PCHAR)RvaToVa(
			pFileMapping,
			exportDirVa.pNameRVAs[i]
		);
		printf("      name: %s\n", pszFunctionName);

		_tprintf(_T("      address: %#010x\n"), exportDirVa.pAddresses[wOrdinal]);
	}

	return SUCCESS;
}

ERROR_CODE
ParseThunkData(
	PFILE_MAPPING pFileMapping,
	PIMAGE_THUNK_DATA pThunkData
)
{
	DWORD dwOrdinal;
	PIMAGE_IMPORT_BY_NAME pImportByName;

	if (!CheckAddressRange(pFileMapping, pThunkData, sizeof(IMAGE_THUNK_DATA)))
	{
		return INVALID_RVA_CODE;
	}

	//while there exist entries
	while (pThunkData->u1.AddressOfData != 0)
	{
		if (pThunkData->u1.AddressOfData >> 31)
		{
			// import by ordinal
			dwOrdinal = pThunkData->u1.Ordinal & 0x0000FFFF;
			printf("      ordinal: %u (in hexa %#010x)\n", dwOrdinal, dwOrdinal);
		}
		else
		{
			// import by name
			pImportByName = (PIMAGE_IMPORT_BY_NAME)RvaToVa(
				pFileMapping,
				pThunkData->u1.AddressOfData
			);

			if (!CheckAddressRange(pFileMapping, pImportByName, 1))
			{
				return INVALID_RVA_CODE;
			}

			printf("      %s\n", pImportByName->Name);
		}
		pThunkData++;
		if (!CheckAddressRange(pFileMapping, pThunkData, sizeof(IMAGE_THUNK_DATA)))
		{
			return INVALID_RVA_CODE;
		}
	}
	return SUCCESS;

}

ERROR_CODE 
ParseOriginalFirstThunkImports(
	_In_ PFILE_MAPPING pFileMapping,
	_In_ PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor
)
{
	DWORD i;
	PCHAR pszImportName;
	PIMAGE_THUNK_DATA pThunkData;

	if (!CheckAddressRange(pFileMapping, pImportDescriptor, sizeof(IMAGE_IMPORT_DESCRIPTOR)))
	{
		return INVALID_RVA_CODE;
	}

	for (i = 0; pImportDescriptor->OriginalFirstThunk != 0; i++)
	{
		if (pImportDescriptor->OriginalFirstThunk == pImportDescriptor->FirstThunk)
		{
			//this address has been already dealt with in the FirstThunk version of this function.
			pImportDescriptor++;
			continue;
		}
		pszImportName = (PCHAR)RvaToVa(
			pFileMapping,
			pImportDescriptor->Name
		);
		if (!CheckAddressRange(pFileMapping, pszImportName, 1))
		{
			return INVALID_RVA_CODE;
		}
		printf("  name of import nr %u: %s\n", i, pszImportName);

		 pThunkData = (PIMAGE_THUNK_DATA)RvaToVa(
			pFileMapping,
			pImportDescriptor->OriginalFirstThunk
		);
		if (!CheckAddressRange(pFileMapping, pThunkData, sizeof(IMAGE_THUNK_DATA)))
		{
			return INVALID_RVA_CODE;
		}

		ParseThunkData(pFileMapping, pThunkData);
		pImportDescriptor++;
	}
	
	return SUCCESS;
}

ERROR_CODE 
ParseFirstThunkImports(
	_In_ PFILE_MAPPING pFileMapping,
	_In_ PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor
)
{
	DWORD i;
	PCHAR pszImportName;
	PIMAGE_THUNK_DATA pThunkData;

	if (!CheckAddressRange(pFileMapping, pImportDescriptor, sizeof(IMAGE_IMPORT_DESCRIPTOR)))
	{
		return INVALID_RVA_CODE;
	}

	for (i = 0; pImportDescriptor->FirstThunk != 0; i++)
	{
		pszImportName = (PCHAR)RvaToVa(
			pFileMapping,
			pImportDescriptor->Name
		);
		if (!CheckAddressRange(pFileMapping, pszImportName, 1))
		{
			return INVALID_RVA_CODE;
		}
		printf("  name of import nr %u: %s\n", i, pszImportName);

		pThunkData = (PIMAGE_THUNK_DATA)RvaToVa(
			pFileMapping,
			pImportDescriptor->FirstThunk
		);
		if (!CheckAddressRange(pFileMapping, pThunkData, sizeof(IMAGE_THUNK_DATA)))
		{
			return INVALID_RVA_CODE;
		}

		ParseThunkData(pFileMapping, pThunkData);
		pImportDescriptor++;
	}
	
	return SUCCESS;
}

ERROR_CODE
ParseImports(
	_In_ PFILE_MAPPING pFileMapping
)
{
	ERROR_CODE errorCode;
	PIMAGE_NT_HEADERS pImageNtHeaders;
	PIMAGE_DATA_DIRECTORY pImportDir;
	PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor;

	pImageNtHeaders = GetNtHeaders(pFileMapping);
	if (pImageNtHeaders == NULL)
	{
		return INVALID_PE_FILE;
	}

	//get pointer to IMPORT_DESCRIPTOR
	pImportDir = &pImageNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)RvaToVa(
		pFileMapping,
		pImportDir->VirtualAddress
	);

	//parse imports
	errorCode = ParseFirstThunkImports(
		pFileMapping,
		pImportDescriptor
	);
	if (errorCode != SUCCESS)
	{
		return errorCode;
	}

	errorCode = ParseOriginalFirstThunkImports(
		pFileMapping,
		pImportDescriptor
	);
	if (errorCode != SUCCESS)
	{
		return errorCode;
	}

	return SUCCESS;
}


ERROR_CODE
MapPEFileInMemory(
	_In_ LPCTSTR pszFilePath, // file path, where the executable is stored
	_Out_ PFILE_MAPPING pFileMapping // where the file mapping will be stored, if operation successful.
)
{
	HANDLE hFile = INVALID_HANDLE_VALUE;
	HANDLE hFileMapping = INVALID_HANDLE_VALUE;
	ERROR_CODE errorCode = SUCCESS;

	__try
	{
		hFile = CreateFile(
			pszFilePath, //file name
			GENERIC_READ, //access
			0, // no sharing
			NULL, //default security attr
			OPEN_EXISTING, //exititing file
			FILE_ATTRIBUTE_NORMAL, //normal file
			NULL // no template file
		);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			errorCode = FILE_OPENING_ERROR;
			__leave;
		}

		GetFileSizeEx(hFile, (PLARGE_INTEGER)&pFileMapping->ullSize);
		
		hFileMapping = CreateFileMapping(
			hFile, //handle to file
			NULL, //default security attr
			PAGE_READONLY,
			0,
			0, // map the whole file
			NULL //anonymous mapping
		);
		if (hFileMapping == INVALID_HANDLE_VALUE)
		{
			errorCode = FILE_MAPPING_ERROR;
			__leave;
		}

		pFileMapping->pvMappingAddress = MapViewOfFile(
			hFileMapping, //file mapping
			FILE_MAP_READ, // read only access
			0,
			0, // map the whole file
			0
		);
		if (pFileMapping->pvMappingAddress == NULL)
		{
			errorCode = MAP_VIEW_ERROR;
			__leave;
		}
	}
	__finally
	{
		CloseHandle(hFile);
		CloseHandle(hFileMapping);
	}

	return errorCode;
}

ERROR_CODE
UnMapPEFileInMemory(
	_In_ PFILE_MAPPING pFileMapping // file mapping to be unmapped
)
{
	if (pFileMapping->pvMappingAddress == NULL)
	{
		return INVALID_ARGS;
	}
	if(!UnmapViewOfFile(pFileMapping->pvMappingAddress))
	{
		return FILE_UNMAPPING_ERROR;
	}
	return SUCCESS;
}

ERROR_CODE
ParseMappedPEFile(
	_In_ PFILE_MAPPING pFileMapping // the address of the Mapping of the file to be parsed.
)
{
	PIMAGE_DOS_HEADER pDOSHeader;
	PIMAGE_NT_HEADERS pNTHeaders;
	PIMAGE_FILE_HEADER pImageFileHeader;
	PIMAGE_OPTIONAL_HEADER pImageOptionalHeader;
	ERROR_CODE errorCode;

	pDOSHeader = (PIMAGE_DOS_HEADER)pFileMapping->pvMappingAddress;
	if (pDOSHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return INVALID_PE_FILE;
	}

	pNTHeaders = GetNtHeaders(pFileMapping);
	if (pNTHeaders == NULL)
	{
		return INVALID_PE_FILE;
	}

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
	errorCode = ParseSectionHeaders(pFileMapping, pImageFileHeader->NumberOfSections);
	if (errorCode != SUCCESS)
	{
		return errorCode;
	}

	_tprintf(_T("\nExported functions:\n"));
	errorCode = ParseExportedFunctions(
		pFileMapping
	);
	if (errorCode != SUCCESS)
	{
		PrintErrorCode(errorCode);
	}

	_tprintf(_T("\nImported functions by module:\n"));
	errorCode = ParseImports(pFileMapping);
	if (errorCode != SUCCESS)
	{
		PrintErrorCode(errorCode);
	}
	
	return SUCCESS;
}
