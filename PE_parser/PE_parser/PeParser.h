/*
 * Author: Ács Dávid
 * Version : 0.1
 * 
 * Description: Funcions for parsing a MZ-PE Windows executable.
 * Date of Creation: 2017-05-13
 * 
 * Change log:
 * 2017-05-13: File created
 * 2017-05-14: ParseMappedPEFile function added and commented.
 */
#ifndef _H_PE_PARSER_
#define _H_PE_PARSER_

#include "ErrorCodes.h"
#include "Everything.h"
#include "ParsingUtilities.h"

/*
 * Parses a file at the address specified.
 * Returns SUCCESS if the operation was successful.
*/
ERROR_CODE
ParseMappedPEFile(
	_In_ PFILE_MAPPING pFileMapping // the address of the Mapping of the file to be parsed.
);

#endif// _H_PE_PARSER_
