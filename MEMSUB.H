#include <stdio.h>



#ifndef flag
	#define flag unsigned char
#endif

#ifndef byte
	#define byte unsigned char
#endif

#define bGarbage 0xCC

flag fNewMemory(void **ppv,size_t size);
void FreeMemory(void *pv);

#ifdef DEBUG
unsigned long ulNoBlocks(void);
#endif
