/*
 * Author: Ács Dávid
 * Version : 0.1
 * 
 * Description: Program parsing a MZ-PE Windows executable.
 * Date of Creation: 2017-05-13
 * 
 * Change log:
 * 2017-05-13: File created
 */

#include "PeParser.h"

VOID 
PrintUsage()
{
	_tprintf(_T("Usage: PE_parser.exe <file_path>\n"));
}

INT 
_tmain(INT argc, PTCHAR argv[])
{
	HANDLE hFile;
	HANDLE hFileMapping;
	FILE_MAPPING fileMapping;

	if (argc != 2)
	{
		PrintUsage();
		ReportError(_T("Invalid arguments, see usage above."), INVALID_ARGS, FALSE);
	}

	hFile = CreateFile(
		argv[1], //file name
		GENERIC_READ, //access
		0, // no sharing
		NULL, //default security attr
		OPEN_EXISTING, //exititing file
		FILE_ATTRIBUTE_NORMAL, //normal file
		NULL // no template file
	);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		ReportError(_T("Could not open file."), FILE_OPENING_ERROR, TRUE);
	}
	GetFileSizeEx(hFile, (PLARGE_INTEGER)&fileMapping.ullSize);
	
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
		ReportError(_T("Could not create file mapping."), MAP_VIEW_ERROR, TRUE);
	}

	fileMapping.pvMappingAddress = MapViewOfFile(
		hFileMapping, //file mapping
		FILE_MAP_READ, // read only access
		0,
		0, // map the whole file
		0
	);
	if (fileMapping.pvMappingAddress == NULL)
	{
		ReportError(_T("Could not map view of file."), 4, TRUE);
	}

	ERROR_CODE errorCode = ParseMappedPEFile(&fileMapping);
	PrintErrorCode(errorCode);

	return SUCCESS;
}