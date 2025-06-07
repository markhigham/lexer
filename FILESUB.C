#include "debug.h"
#include "filesub.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <dir.h>
#include <io.h>

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
	while ( !isalpha( iChar=fgetc(TextFile) ) ) {
		if ( iChar == EOF )
			return(FALSE);
	}

	/* Get all alpha characters */
	do {
		strWord[iCount++]=tolower(iChar);
	} while ( isalpha( iChar=fgetc(TextFile) )
			&& iChar!=EOF
			&& iCount < MaxLen );

	/* Add terminating null character */
	strWord[iCount]='\0';
	return(TRUE);
}

/********************************************************************

fGetAbsFilename

######## NB This function is DOS specific ###########

Creates the absolute filename of a file i.e. including drive letter,
directory and filename

Declaration:
 flag fGetAbsFilename(char *strFile, char *strAbsFilename)

Returns:
 TRUE on success
 FALSE on failure - strFile does not exist

Usage:
 if (!fGetAbsFilename(strRelFilename,strAbsFilename)
	{ exit program }

Remarks:
NB It is very important that you allocate sufficent space to strAbsFilename
to avoid overwriting the end of the string
MAXPATH (defined in BC++ dir.h) should be sufficient

********************************************************************/


flag fGetAbsFilename(char *strFile,char *strAbsFilename)
{

	char *strTemp;

	char strDrive[MAXDRIVE];	/* All four char[]'s contain the full path info */
	char strDir[MAXDIR];       /* for strFile. Used by fnsplit and fnmerge */
	char strFilename[MAXFILE]; /* Defined in dir.h	*/
	char strExt[MAXEXT];       /* See Borland On-line help 	*/

	char strCwdDrive[MAXDRIVE];/* Corresponding char[]'s for the cwd */
	char strCwdDir[MAXDIR];
	char strCwdFilename[MAXFILE];	char strCwdExt[MAXEXT];

	int iFlags,i,iDotCount=0,iSlashCount=0;

	assert(strFile != NULL);
	assert(strAbsFilename != NULL);
	/* Check whether strFile exists
	 NB Will not fail if strFile points to a directory */
	if(access(strFile,0x00))
			return(0);

/* Get current working directory */
	if ( NULL == (strTemp=getcwd(NULL,MAXPATH) )) {
			fputs("Memory allocation failure",stderr);
			exit(EXIT_FAILURE);
	}

/* Add a dummy filename to strTemp so that fnsplit does not read the current
	subdirectory as a filename */
	strcat(strTemp,"\\xerrorx");

/*	Split the current working directory into its component parts */
	fnsplit(strTemp,strCwdDrive,strCwdDir,strCwdFilename,strCwdExt);
	free(strTemp);

/* Split the filename - strFile to its component parts */
	iFlags=fnsplit(strFile,strDrive,strDir,strFilename,strExt);

/* If there is no filename specified then return a failure */
	if ( !(iFlags & FILENAME))
		return(0);

/* If there is no drive letter in strFile use one from cwd */
	if (!(iFlags & DRIVE) )
		strcpy(strDrive,strCwdDrive);

/* If there is no path in strFile use cwd */
	if (!(iFlags & DIRECTORY))
		strcpy(strDir,strCwdDir);
	else {
	/* Check whether path is a relative one ie ..\mark\* */

		for (i=0; strDir[i] && strDir[i]!='\\' ; i++)
			if (strDir[i]=='.')
				iDotCount++;
/* If there are any leading dots in strFile count number of sub dirs in strCwdDir */
		if (iDotCount)	{
			for (i=0; strCwdDir[i]; i++)
				if (strCwdDir[i]=='\\')
					iSlashCount++;

/* If there are the same amount of dots as slashes we are looking at root */
			if (iSlashCount == iDotCount) {
				strcpy(strDir,"\\");
			}
			else {
/* Strip away one subdirectory for every dot */
				for (i=strlen(strCwdDir); i>=0 && iDotCount ; i--)
					if (strCwdDir[i]=='\\') {
						strCwdDir[i]='\0';
						iDotCount--;
				}
			strcpy(strDir,strCwdDir);
			}
		}
	}
/* Combine all the parts to make the full path name */
	fnmerge(strAbsFilename,strDrive,strDir,strFilename,strExt);
	return(1);
}

