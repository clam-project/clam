#include "strdup.h"

char* strdup(const char* str)
{
	char* cpy = malloc(strlen(str)+1);
	if (str) 
	{
		strcpy(cpy,str);
	}
	return cpy;
}
