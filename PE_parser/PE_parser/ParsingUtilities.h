/*	
 * Author: Ács Dávid
 * Version : 0.2
 * 
 * Description: Utility functions and constants for parsing a MZ-PE Windows executable.
 * Date of Creation: 2017-05-14
 * 
 * Change log:
 * 2017-05-14: File created
 * 2017-05-14: GetExportDirectoryInVA function added.
 */

#ifndef _H_PARSING_UTILITIES_
#define _H_PARSING_UTILITIES_

#include "Everything.h"
#include "ErrorCodes.h"

// export directory translated in VA
typedef struct _EXPORT_DIR_VA{
	PIMAGE_EXPORT_DIRECTORY pExportDirectory;
	PCHAR pcName;
	PDWORD pNameRVAs;
	PDWORD pAddresses;
	PWORD pOrdinals;
}EXPORT_DIR_VA, *PEXPORT_DIR_VA;

typedef struct _FILE_MAPPING{
	PVOID pvMappingAddress;
	ULONGLONG ullSize;
}FILE_MAPPING, *PFILE_MAPPING;

/*
 * Returns pvPointer + ullToAdd
 */
PVOID
AddToPointer(
	_In_ PVOID pvPointer,
	_In_ ULONGLONG ullToAdd
);

/*
 * Check if the given address and size is inside the mapping area.
 */
BOOL 
CheckAddressRange(
	_In_ PFILE_MAPPING pFileMapping,
	_In_ PVOID pvStart,
	_In_ ULONGLONG ullSize
);

/*
 * Returns the positon of NtHeaders, NULL if it does not exist.
 */
PIMAGE_NT_HEADERS
GetNtHeaders(
	_In_ PFILE_MAPPING pFileMapping
);

/*
 * Converts an Relative Virtual Address (rva) into a memory mapped address.
 * If the rva does not correspond to either of the sections, then the return value is NULL.
 * The formula is: mappingAddress + rva - section->VirtualAddress + section->PointerToRawData.
 */
PVOID
RvaToVa(
	_In_ PIMAGE_NT_HEADERS pImageNtHeaders, //pointer to NT_HEADERS
	_In_ PVOID pvMappingAddress, // where the file is mapped
	_In_ DWORD rva // relative virtual address
);


/*
 * Returns the code of the machine in a human readable string
 * If the code does not correspond to a existing value, then the return value is NULL.
 */
LPCTSTR
GetMachineString(
	_In_ WORD machine // the code to be printed in human readable format.
);

/*
 * Returns the code of the subsystem in a human readable string
 * If the code does not correspond to a existing value, then the return value is NULL.
 */
LPCTSTR
GetSubsystemString(
	_In_ WORD subsystem // the to be printed in human readable format.
);

/*
 * Prints to the standart output the human readable format of the caracteristic field.
 */
VOID
PrintCharacString(
	_In_ WORD characteristic //characterisitc to be printed in human reabable format.
);

/*
 * Returns a pointer to the header specified by number i.
 * If the header does not exist, returns NULL
 */
PIMAGE_SECTION_HEADER
GetSectionHeader(
	_In_ PFILE_MAPPING pFileMapping, //address of memory mapped PE file
	_In_ DWORD i // number of section
);



/*
 * Gets and translates the Export directory entry to VA
 */
ERROR_CODE
GetExportDirectoryInVA(
	_In_ PIMAGE_NT_HEADERS pNtHeaders, //NT headers mapped in memory
	_In_ PVOID pvMappingAddress, //mapping address
	_Out_ PEXPORT_DIR_VA pExportDirVa // where the translated data will be stored after successful operation
);


#endif// _H_PARSING_UTILITIES_

