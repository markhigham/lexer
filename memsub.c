#include "debug.h"

#include "memsub.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>


#ifdef DEBUG
	unsigned long ulBlocks=0;
#endif

flag fNewMemory(void **ppv,size_t size)
{

	byte **ppb = (byte **)ppv;

	assert(ppv != NULL);
	assert(size !=0);

	*ppb = (byte *)malloc(size);

	#ifdef DEBUG
		if ( *ppb != NULL)
			memset(*ppb,bGarbage,size);
		ulBlocks++;
	#endif

	return( *ppb != NULL);
}


void FreeMemory(void *pv)
{
	assert(pv != NULL);

	#ifdef DEBUG
		ulBlocks--;
	#endif

	free(pv);
}

/* This function is only accessable in the DEBUG version */
#ifdef DEBUG
unsigned long ulNoBlocks(void)
{
	return(ulBlocks);
}
#endif
