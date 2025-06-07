#include "debug.h"

#include "btree.h"
#include "memsub.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


flag fAddXNewItems(Node **ppNode,
			    void *pvItem,
			    long lCount,
			    int (*iCompare)(const void *, const void *))
{
	int iCompResult;

	assert( ppNode != NULL );
	assert( pvItem != NULL );
	assert( iCompare != NULL );

/* Check if node pointer is null and create a new node if so*/
	if ( NULL == *ppNode ) {
		if (!fNewMemory((void *)ppNode,sizeof(Node))) {
			fputs("Memory allocation failure",stderr);
			exit(EXIT_FAILURE);
		}
		(*ppNode)->pNodeLeft=NULL;
		(*ppNode)->pNodeRight=NULL;
		(*ppNode)->pvItem=pvItem;
		(*ppNode)->lItemCount=lCount;
		return(1);
	}

/* Compare the new item with the node item */
	iCompResult=iCompare(pvItem,(*ppNode)->pvItem);

	if ( iCompResult < 0 )
		return( fAddXNewItems( &(*ppNode)->pNodeLeft,pvItem,lCount,iCompare) );

	else if ( iCompResult > 0 )
		return( fAddXNewItems( &(*ppNode)->pNodeRight,pvItem,lCount,iCompare) );

	else {
		(*ppNode)->lItemCount+=lCount;
		return(0);
	}

} /* fAddXNewItems() */

void vSpoolTree(FILE *fp,
			 Node *pNode,
			 void (*vPrintNode)(FILE *fp,const void *),
			 void(*vPrintCount)(FILE *fp,long))
{
	assert( fp != NULL);

	if ( NULL != pNode) {
		vSpoolTree( fp,pNode->pNodeLeft,vPrintNode,vPrintCount);
		vPrintNode(fp,pNode->pvItem);
		vPrintCount(fp,pNode->lItemCount);
		vSpoolTree( fp,pNode->pNodeRight,vPrintNode,vPrintCount);
	}
} /* vSpoolTree */

void vChopDownTree(Node *pNode,void (*vFreeItem)(void *))
{
	if ( NULL !=pNode) {
		vChopDownTree(pNode->pNodeLeft,vFreeItem);
		vChopDownTree(pNode->pNodeRight,vFreeItem);
		vFreeItem(pNode->pvItem);
		FreeMemory(pNode);
	}
} /* vChopDownTree */
