#include <stdio.h>

#ifndef flag
	#define flag unsigned char
#endif

#ifndef byte
	#define byte unsigned char
#endif

typedef struct node {
	struct node *pNodeLeft;
	struct node *pNodeRight;
	void *pvItem;
	long lItemCount;
} Node;
flag fAddXNewItems(Node **, void *, long,int (*iCompare)(const void *, const void *));

#ifndef fAddNewItem
	#define fAddNewItem(node,item,funct) fAddXNewItems(node,item,1,funct)
#endif

void vSpoolTree(FILE *fp,
			 Node *pNode,
			 void (*vPrintNode)(FILE *fp,const void *),
			 void(*vPrintCount)(FILE *fp,long));

#ifndef vPrintTree
	#define vPrintTree(node,funct1,funct2) vSpoolTree(stdout,node,funct1,funct2)
#endif

void vChopDownTree(Node *pNode,void (*vFreeItem)(void *));

