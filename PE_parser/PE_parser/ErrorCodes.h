/*
 * Author: Ács Dávid
 * Version : 0.3
 * 
 * Description: Error codes (self explanatory).
 * Date of Creation: 2017-05-13
 * 
 * Change log:
 * 2017-05-13: File created
 * 2017-05-14: Export and import table erros added.
 * 2017-05-14: Support for 64 and 32 bit error codes added.
 */

#ifndef _H_ERROR_CODES_
#define _H_ERROR_CODES_

#ifdef _WIN64
#define ARCH_BIT_STRING "64"
#else 
#define ARCH_BIT_STRING "32"
#endif

#include "Everything.h"

typedef enum _ERROR_CODE {
	SUCCESS, INVALID_ARGS, 
	FILE_OPENING_ERROR, FILE_MAPPING_ERROR, FILE_UNMAPPING_ERROR, MAP_VIEW_ERROR,
	INVALID_PE_FILE, INVALID_MACHINE_CODE, INVALID_SUBSYSTEM_CODE, INVALID_RVA_CODE, 
	EXPORT_TABLE_MISSING, IMPORT_TABLE_MISSING, INVALID_TABLE_RVA,
}ERROR_CODE;

/*
 * Prints an error code to stdout in human readable format
 */
VOID
PrintErrorCode(
	_In_ ERROR_CODE errorCode //error code to be printed
);

#endif// _H_ERROR_CODES_
