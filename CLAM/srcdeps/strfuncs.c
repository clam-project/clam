#include "strfuncs.h"
#include <stdio.h>

char* cur;
int curn;

void strstart(char* tgt,int n)
{
	if (cur)
	{
		fprintf(stderr,"error: can only stradd one string at a time\n");
		exit(-1);
	}
	cur = tgt;
	curn = n;
	if (n)
		*cur = 0;
}

void stradd(const char* a)
{
	if (a) while  (*a && curn--) { *cur++ = *a++; }
	*cur = 0;
}

void strend(void)
{
	*cur = 0;
	cur = 0;
}
