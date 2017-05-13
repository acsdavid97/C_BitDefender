#include "ErrorCodes.h"

VOID
printErrorCode(
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
		default:
			ReportError(_T("Unkown error code"), 0, FALSE);
			break;
	}
}
