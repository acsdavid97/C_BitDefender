/*
 * Author: Ács Dávid
 * Version : 0.1
 * 
 * Description: Funcions for parsing a MZ-PE Windows executable.
 * Date of Creation: 2017-05-13
 * 
 * Change log:
 * 2017-05-13: File created
 */
#pragma once

#include "ErrorCodes.h"
#include "Everything.h"


/*
 * Parses a file at the address specified.
 * Returns SUCCESS if the operation was successful.
*/
ERROR_CODE
ParseMappedPEFile(
	_In_ LPVOID lpvMappedAddr // the address of the Mapping of the file to be parsed.
);

