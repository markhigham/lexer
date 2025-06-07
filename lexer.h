/* lexer.h */
#define flag unsigned char
#define byte unsigned char

#define TRUE 1
#define FALSE 0

#define L_FLAG 0x01


#define MAXWORDLEN 255 /* No words from input file longer than this */



#define MAXABSPATH 255 /* The maximum length of the full pathname for the
								  textfile */


typedef struct linklist {
	struct linklist *next;
	char *strText;
} LinkList;
