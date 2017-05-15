/*
 * Author: Ács Dávid
 * Version : 0.3
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

	return SUCCESS;
}