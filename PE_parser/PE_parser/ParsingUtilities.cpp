/*	
 * Author: Ács Dávid
 * Version : 0.2
 * 
 * Description: Utility function implementations for parsing a MZ-PE Windows executable.
 * Date of Creation: 2017-05-14
 * 
 * Change log:
 * 2017-05-14: File created
 * 2017-05-14: RvaToVa function implemented and tested
 * 2017-05-14: GetExportDirectoryInVA function added.
 */

#include "ParsingUtilities.h"

PVOID
AddToPointer(
	_In_ PVOID pvPointer,
	_In_ ULONGLONG ullToAdd
)
{
	return (PVOID)((ULONGLONG)pvPointer + ullToAdd);
}

BOOL 
CheckAddressRange(
	_In_ PFILE_MAPPING pFileMapping,
	_In_ PVOID pvStart,
	_In_ ULONGLONG ullSize
)
{
	PVOID pvEndArea = AddToPointer(pvStart, ullSize);
	PVOID pvEndMapping = AddToPointer(pFileMapping->pvMappingAddress, pFileMapping->ullSize);
	return pFileMapping->pvMappingAddress <= pvStart && pvEndArea <= pvEndMapping;
}

PIMAGE_NT_HEADERS
GetNtHeaders(
	_In_ PFILE_MAPPING pFileMapping
)
{
	//todo implement
}

PVOID
RvaToVa(
	_In_ PIMAGE_NT_HEADERS pImageNtHeaders, //pointer to NT_HEADERS
	_In_ PVOID pvMappingAddress, // where the file is mapped
	_In_ DWORD rva // relative virtual address
)
{
	PIMAGE_SECTION_HEADER pSectionHeader;
	DWORD dwVirtAddr;
	DWORD dwSizeOfData;
	DWORD dwNrSections = pImageNtHeaders->FileHeader.NumberOfSections;

	if (rva == 0 || dwNrSections == 0)
	{
		return NULL;
	}

	//find the corresponding section
	for(DWORD i = 0; i < dwNrSections; i++)
	{
		pSectionHeader = GetSectionHeader(pImageNtHeaders, i);
		dwVirtAddr = pSectionHeader->VirtualAddress;
		dwSizeOfData = pSectionHeader->SizeOfRawData;

		if (dwVirtAddr <= rva && rva < dwVirtAddr + dwSizeOfData)
		{
			//found the corresponding section
			return (PVOID)((ULONGLONG) pvMappingAddress + rva - dwVirtAddr + pSectionHeader->PointerToRawData);
		}
	}

	return NULL;
}


LPCTSTR
GetMachineString(
	_In_ WORD machine // the code to be printed in human readable format.
)
{
	switch (machine)
	{
		case IMAGE_FILE_MACHINE_UNKNOWN:
			return _T("IMAGE_FILE_MACHINE_UNKNOWN");
		case IMAGE_FILE_MACHINE_AM33:
			return _T("IMAGE_FILE_MACHINE_AM33");
		case IMAGE_FILE_MACHINE_AMD64:
			return _T("IMAGE_FILE_MACHINE_AMD64");
		case IMAGE_FILE_MACHINE_ARM:
			return _T("IMAGE_FILE_MACHINE_ARM");
		case IMAGE_FILE_MACHINE_ARMNT:
			return _T("IMAGE_FILE_MACHINE_ARMNT");
		case IMAGE_FILE_MACHINE_EBC:
			return _T("IMAGE_FILE_MACHINE_EBC");
		case IMAGE_FILE_MACHINE_I386:
			return _T("IMAGE_FILE_MACHINE_I386");
		case IMAGE_FILE_MACHINE_IA64:
			return _T("IMAGE_FILE_MACHINE_IA64");
		case IMAGE_FILE_MACHINE_M32R:
			return _T("IMAGE_FILE_MACHINE_M32R");
		default:
			return NULL;
	}
}

LPCTSTR
GetSubsystemString(
	_In_ WORD subsystem // the to be printed in human readable format.
)
{
	switch (subsystem)
	{
		case IMAGE_SUBSYSTEM_UNKNOWN:
			return _T("IMAGE_SUBSYSTEM_UNKNOWN");
		case IMAGE_SUBSYSTEM_NATIVE:
			return _T("IMAGE_SUBSYSTEM_NATIVE");
		case IMAGE_SUBSYSTEM_WINDOWS_GUI:
			return _T("IMAGE_SUBSYSTEM_WINDOWS_GUI");
		case IMAGE_SUBSYSTEM_WINDOWS_CUI:
			return _T("IMAGE_SUBSYSTEM_WINDOWS_CUI");
		case IMAGE_SUBSYSTEM_POSIX_CUI:
			return _T("IMAGE_SUBSYSTEM_POSIX_CUI");
		case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI:
			return _T("IMAGE_SUBSYSTEM_CE_GUI");
		case IMAGE_SUBSYSTEM_EFI_APPLICATION:
			return _T("IMAGE_SUBSYSTEM_EFI_APPLICATION");
		case IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER:
			return _T("IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER");
		case IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER:
			return _T("IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER");
		case IMAGE_SUBSYSTEM_EFI_ROM:
			return _T("IMAGE_SUBSYSTEM_EFI_ROM");
		case IMAGE_SUBSYSTEM_XBOX:
			return _T("IMAGE_SUBSYSTEM_XBOX");
		default:
			return NULL;
	}
}

VOID
PrintCharacString(
	_In_ WORD characteristic //characterisitc to be printed in human reabable format.
)
{
	if (characteristic & IMAGE_FILE_RELOCS_STRIPPED)
	{
		_tprintf(_T("IMAGE_FILE_RELOCS_STRIPPED\n"));
	}
	if (characteristic & IMAGE_FILE_EXECUTABLE_IMAGE)
	{
		_tprintf(_T("IMAGE_FILE_EXECUTABLE_IMAGE\n"));
	}
	if (characteristic & IMAGE_FILE_LINE_NUMS_STRIPPED)
	{
		_tprintf(_T("IMAGE_FILE_LINE_NUMS_STRIPPED\n"));
	}
	if (characteristic & IMAGE_FILE_LOCAL_SYMS_STRIPPED)
	{
		_tprintf(_T("IMAGE_FILE_LOCAL_SYSMS_STRIPPED\n"));
	}
	if (characteristic & IMAGE_FILE_AGGRESIVE_WS_TRIM)
	{
		_tprintf(_T("IMAGE_FILE_AGGRESSIVE_WS_TRIM\n"));
	}
	if (characteristic & IMAGE_FILE_LARGE_ADDRESS_AWARE)
	{
		_tprintf(_T("IMAGE_FILE_LARGE_ADDRESS_AWARE\n"));
	}
	if (characteristic & IMAGE_FILE_BYTES_REVERSED_LO)
	{
		_tprintf(_T("IMAGE_FILE_BYTES_REVERSED_LO\n"));
	}
	if (characteristic & IMAGE_FILE_32BIT_MACHINE)
	{
		_tprintf(_T("IMAGE_FILE_32BIT_MACHINE\n"));
	}
	if (characteristic & IMAGE_FILE_DEBUG_STRIPPED)
	{
		_tprintf(_T("IMAGE_FILE_DEBUG_STRIPPED\n"));
	}
	if (characteristic & IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP)
	{
		_tprintf(_T("IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP\n"));
	}
	if (characteristic & IMAGE_FILE_NET_RUN_FROM_SWAP)
	{
		_tprintf(_T("IMAGE_FILE_NET_RUN_FROM_SWAP\n"));
	}
	if (characteristic & IMAGE_FILE_SYSTEM)
	{
		_tprintf(_T("IMAGE_FILE_SYSTEM\n"));
	}
	if (characteristic & IMAGE_FILE_DLL)
	{
		_tprintf(_T("IMAGE_FILE_DLL\n"));
	}
	if (characteristic & IMAGE_FILE_UP_SYSTEM_ONLY)
	{
		_tprintf(_T("IMAGE_FILE_UP_SYSYTEM_ONLY\n"));
	}
	if (characteristic & IMAGE_FILE_BYTES_REVERSED_HI)
	{
		_tprintf(_T("IMAGE_FILE_BYTES_REVERSED_HI\n"));
	}
}

PIMAGE_SECTION_HEADER 
GetSectionHeader(
	_In_ PFILE_MAPPING pFileMapping, //address of memory mapped PE file
	_In_ DWORD i // number of section
)
{
	PIMAGE_NT_HEADERS pImageNtHeaders = 
	if (i >= pImageNtHeaders->FileHeader.NumberOfSections)
	{
		return NULL;
	}

	if (!CheckAddressRange())
	ULONGLONG ullToAdd = sizeof(IMAGE_NT_HEADERS) + i * sizeof(IMAGE_SECTION_HEADER);
	PVOID pvAddress = AddToPointer(pImageNtHeaders, uLL)
	return (PIMAGE_SECTION_HEADER)((ULONGLONG)pImageNtHeaders
}

ERROR_CODE
GetExportDirectoryInVA(
	_In_ PIMAGE_NT_HEADERS pNtHeaders, //NT headers mapped in memory
	_In_ PVOID pvMappingAddress, //mapping address
	_Out_ PEXPORT_DIR_VA pExportDirVa // where the translated data will be stored after successful operation
)
{
	PIMAGE_DATA_DIRECTORY pExportDataDirectory;

	if (pNtHeaders->OptionalHeader.NumberOfRvaAndSizes <= IMAGE_DIRECTORY_ENTRY_EXPORT)
	{
		return EXPORT_TABLE_MISSING;
	}

	pExportDataDirectory = &pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

	pExportDirVa->pExportDirectory = (PIMAGE_EXPORT_DIRECTORY)RvaToVa(
		pNtHeaders,
		pvMappingAddress,
		pExportDataDirectory->VirtualAddress
	);
	if (pExportDirVa->pExportDirectory == NULL)
	{
		return INVALID_TABLE_RVA;
	}

	pExportDirVa->pcName = (PCHAR)RvaToVa(
		pNtHeaders,
		pvMappingAddress,
		pExportDirVa->pExportDirectory->Name
	);
	if (pExportDirVa->pcName == NULL)
	{
		return INVALID_RVA_CODE;
	}

	pExportDirVa->pNameRVAs = (PDWORD)RvaToVa(
		pNtHeaders,
		pvMappingAddress,
		pExportDirVa->pExportDirectory->AddressOfNames
	);
	if (pExportDirVa->pNameRVAs == NULL)
	{
		return INVALID_RVA_CODE;
	}

	pExportDirVa->pAddresses = (PDWORD)RvaToVa(
		pNtHeaders,
		pvMappingAddress,
		pExportDirVa->pExportDirectory->AddressOfFunctions
	);
	if (pExportDirVa->pAddresses == NULL)
	{
		return INVALID_RVA_CODE;
	}

	pExportDirVa->pOrdinals = (PWORD)RvaToVa(
		pNtHeaders,
		pvMappingAddress,
		pExportDirVa->pExportDirectory->AddressOfNameOrdinals
	);
	if (pExportDirVa->pOrdinals == NULL)
	{
		return INVALID_RVA_CODE;
	}

	return SUCCESS;
}
