/*
 * Author: Ács Dávid
 * Version : 0.3
 * 
 * Description: Functions for parsing a MZ-PE Windows executable.
 * Date of Creation: 2017-05-13
 * 
 * Change log:
 * 2017-05-13: File created
 * 2017-05-14: ParseMappedPEFile function added and commented.
 * 2017-05-14: ParsingUtilities.h included
 */
#ifndef _H_PE_PARSER_
#define _H_PE_PARSER_

#include "ErrorCodes.h"
#include "Everything.h"
#include "ParsingUtilities.h"

/*
 * Parses the IMAGE_FILE_HEADER of the memory mapped executable.
 * Prints the machine code, number of sections and characterisitics both in hexa and
 * human readable form.
 * Returns an ERROR_CODE
 */
ERROR_CODE
ParseImageFileHeader(
	_In_ PIMAGE_FILE_HEADER pImageFileHeader //pointer to the header to be parsed.
);

/*
 * Parses the IMAGE_OPTIONAL_HEADER of the memory mapped executable.
 * Returns an ERROR_CODE
 */
ERROR_CODE
ParseImageOptionalHeader(
	_In_ PIMAGE_OPTIONAL_HEADER pImageOptionalHeader // pointer to header to be parsed.
);

/*
 * Parses a section header, prints the name and important entries in the header.
 */
VOID
ParseSectionHeader(
	_In_ PIMAGE_SECTION_HEADER pSectionHeader // the address of the section to be parsed.
);

/*
 * Parses all section headers in a PE file. See ParseSectionHeader for more detail.
 */
ERROR_CODE
ParseSectionHeaders(
	_In_ PFILE_MAPPING pFileMapping, // pointer to file mapping
	_In_ WORD wNrSections // number of sections to parse
);

/*
 * Prints the name, ordinal and address of the exported functions in the memory mapped PE file.
 * 
 */
ERROR_CODE
ParseExportedFunctions(
	_In_ PFILE_MAPPING pFileMapping
);

/*
 * Parses an IMAGE_THUNK_DATA struct. 
 * Prints the name or ordinal of all imported functions in this module.
 */
ERROR_CODE
ParseThunkData(
	PFILE_MAPPING pFileMapping,
	PIMAGE_THUNK_DATA pThunkData
);

/*
 * Parses all the imports of the memory mapped PE file.
 * The imports are groupped by module, for each module, the list of fucntion names or ordinals is printed.
 */
ERROR_CODE
ParseImports(
	_In_ PFILE_MAPPING pFileMapping
);

ERROR_CODE
MapPEFileInMemory(
	_In_ LPCTSTR pszFilePath, // file path, where the executable is stored
	_Out_ PFILE_MAPPING pFileMapping // where the file mapping will be stored, if operation successful.
);

/*
 * Parses a file at the address specified.
 * Returns SUCCESS if the operation was successful.
*/
ERROR_CODE
ParseMappedPEFile(
	_In_ PFILE_MAPPING pFileMapping // the address of the Mapping of the file to be parsed.
);

#endif// _H_PE_PARSER_
