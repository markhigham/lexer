#include <stdio.h>

#ifndef TRUE
	#define TRUE 1
#endif

#ifndef FALSE
	#define FALSE 0
#endif


#ifndef flag
	#define flag unsigned char
#endif


flag fGetLowercaseWord( FILE *, char *, size_t );
flag fGetAbsFilename(char *,char *, size_t);
