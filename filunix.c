#include "debug.h"
#include "filesub.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <dir.h>
/* #include <io.h> */

#ifndef MAXPATH
	#define MAXPATH 255
#endif

/**********************************************************************

fGetLowercaseWord

Gets a Null terminated string of lowercase alphabetic characters from a file.
(Characters other than 'A' - 'Z' and 'a' - 'z' are ignored.)

Declaration:
 flag fGetLowercaseWord( FILE *TextFile, char *strWord, size_t MaxLen )

Returns:
 TRUE on success
 FALSE at EOF

Usage:
 while (fGetLowercaseWord(fp,strGetWord,1000)
	puts(strGetWord);

Remarks:
Ensure that you have allocated enough space in strGetWord to accomodate
 MaxLen characters.
Encountering a word of greater than MaxLen characters will not cause
 a failure but will truncate the word to MaxLen characters (including null
 terminator) this will leave the file pointer mid-word and on the next call
 the word may commence with the next character


***********************************************************************/


flag fGetLowercaseWord( FILE *TextFile, char *strWord, size_t MaxLen )
{

	int iChar,iCount=0;

	assert ( TextFile != NULL );
	assert ( strWord != NULL );
	assert ( MaxLen > 1 );

	/* Lose any initial whitespace */
	while ( !isalpha( iChar=fgetc(TextFile) ) /*&& !isdigit(iChar)*/ ) {
		if ( iChar == EOF )
			return(FALSE);
	}

	/* Get all alpha characters */
	do {
		strWord[iCount++]= iChar; /*tolower(iChar);*/
	} while ( (isalpha( iChar=fgetc(TextFile)  ) /*|| isdigit(iChar)*/)
			&& iChar!=EOF
			&& iCount < MaxLen );

	/* Add terminating null character */
	strWord[iCount]='\0';
	return(TRUE);
}



/********************************************************************

fGetAbsFilename

Creates the absolute filename of a file i.e. path plus filename

Declaration:
 flag fGetAbsFilename(char *strFile, char *strAbsFilename)

Returns:
 TRUE on success
 FALSE on failure

Usage:
 if (!fGetAbsFilename(strRelFilename,strAbsFilename)
	{ exit program }

Remarks:
NB It is very important that you allocate sufficent space to strAbsFilename
to avoid overwriting the end of the string
MAXPATH (defined in BC++ dir.h) should be sufficient



********************************************************************/


flag fGetAbsFilename(char *strFile,char *strAbsFilename, size_t buflen)
{

	assert(strAbsFilename != NULL);
	assert(strFile != NULL);

	if ( NULL == getcwd(strAbsFilename,buflen))
		return(0);

	strcat(strAbsFilename,"/");
	strcat(strAbsFilename,strFile);
	return(1);
}

