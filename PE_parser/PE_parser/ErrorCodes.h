/*
 * Author: Ács Dávid
 * Version : 0.2
 * 
 * Description: Error codes (self explanatory).
 * Date of Creation: 2017-05-13
 * 
 * Change log:
 * 2017-05-13: File created
 * 2017-05-14: Export and import table erros added.
 */

#ifndef _H_ERROR_CODES_
#define _H_ERROR_CODES_

#include "Everything.h"

typedef enum _ERROR_CODE {
	SUCCESS, INVALID_ARGS, FILE_OPENING_ERROR, FILE_MAPPING_ERROR, MAP_VIEW_ERROR,
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
