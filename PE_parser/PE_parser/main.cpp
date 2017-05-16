/*
 * Author: Ács Dávid
 * Version : 0.3
 * 
 * Description: Program parsing a MZ-PE Windows executable.
 * The program is capable of parsing both 32 and 64 bit executables if compiled correctly.
 * In order to parse 64 bit executables, the program needs to be compiled with _WIN64. 
 * In this case the program is not capable of parsing 32 bit executables.
 * If the program is compiled without _WIN64, then only 32 bit executables can be parsed.
 * 
 * Date of Creation: 2017-05-13
 * 
 * Change log:
 * 2017-05-13: File created
 * 2017-05-15: MapPEFileInMemory function usage.
 * 
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
	FILE_MAPPING fileMapping;
	ERROR_CODE errorCode;
	if (argc != 2)
	{
		PrintUsage();
		ReportError(_T("Invalid arguments, see usage above."), INVALID_ARGS, FALSE);
	}

	errorCode = MapPEFileInMemory(argv[1], &fileMapping);
	_tprintf(_T("RESULT OF LOADING:\n"));
	PrintErrorCode(errorCode);
	if (errorCode != SUCCESS)
	{
		return errorCode;
	}

	errorCode = ParseMappedPEFile(&fileMapping);
	_tprintf(_T("\nRESULT OF PARSING:\n"));
	PrintErrorCode(errorCode);

	errorCode = UnMapPEFileInMemory(&fileMapping);
	if (errorCode != SUCCESS)
	{
		PrintErrorCode(errorCode);
	}

	return SUCCESS;
}