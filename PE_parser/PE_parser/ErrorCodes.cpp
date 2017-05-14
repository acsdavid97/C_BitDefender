/*
 * Author: Ács Dávid
 * Version : 0.2
 * 
 * Description: Error codes translated into strings and printed to stdout.
 * Date of Creation: 2017-05-13
 * 
 * Change log:
 * 2017-05-13: File created
 * 2017-05-14: Export and import table errors added.
 */

#include "ErrorCodes.h"

VOID
PrintErrorCode(
	_In_ ERROR_CODE errorCode //error code to be printed
)
{
	switch (errorCode)
	{
		case SUCCESS:
			ReportError(_T("Operation sucessful"), 0, FALSE);
			break;
		case INVALID_ARGS:
			ReportError(_T("Invalid arguments"), 0, FALSE);
			break;
		case FILE_OPENING_ERROR:
			ReportError(_T("Could not open file"), 0, FALSE);
			break;
		case FILE_MAPPING_ERROR:
			ReportError(_T("Could not map file."), 0, FALSE);
			break;
		case MAP_VIEW_ERROR:
			ReportError(_T("Could not map view of file"), 0, FALSE);
			break;
		case INVALID_PE_FILE:
			ReportError(_T("File is not a valid PE file."), 0, FALSE);
			break;
		case INVALID_MACHINE_CODE:
			ReportError(_T("Machine code is not a valid one"), 0, FALSE);
			break;
		case INVALID_SUBSYSTEM_CODE:
			ReportError(_T("Subsystem code is not a valid one"), 0, FALSE);
			break;
		case INVALID_TABLE_RVA:
			ReportError(_T("Table address contains an invalid RVA address, probably it is missing"), 0, FALSE);
			break;
		case EXPORT_TABLE_MISSING:
			ReportError(_T("Export table is missing"), 0, FALSE);
			//fall through
		case IMPORT_TABLE_MISSING:
			ReportError(_T("Import table is missing"), 0, FALSE);
			break;
		case INVALID_RVA_CODE:
			ReportError(_T("Invalid RVA code found while parsing the PE file"), 0, FALSE);
			break;
		default:
			ReportError(_T("Unkown error code"), 0, FALSE);
			break;
	}
}
