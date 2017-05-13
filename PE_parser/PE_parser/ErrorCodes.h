#pragma once

#include "Everything.h"

typedef enum _ERROR_CODE {
	SUCCESS, INVALID_ARGS, FILE_OPENING_ERROR, FILE_MAPPING_ERROR, MAP_VIEW_ERROR,
	INVALID_PE_FILE, INVALID_MACHINE_CODE
}ERROR_CODE;

/*
 * Prints an error code to stdout in human readable format
 */
VOID
printErrorCode(
	_In_ ERROR_CODE errorCode //error code to be printed
);
