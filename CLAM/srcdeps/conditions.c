#include <stdio.h>
#include <stdlib.h>


int condi = -1;
char cond[2048];

void conditions_start(void)
{
	condi = -1;
}

void conditions_end(void)
{
	if (condi!=-1)
	{
		fprintf(stderr,"Error: Unterminated preprocessor conditions\n");
	}
}

int conditions_total(void)
{
	int i;

	for (i=condi;i>=0;i--) {
		if (cond[condi]==0) {
			return 0;
		}
	}	
	return 1;
}

void conditions_push(int v)
{
	condi++;
	if (condi==2048) {
		fprintf(stderr,"Error: hard-coded limit of 2048 preprocessor conditions reached\n");
		exit(-1);
	}
	cond[condi] = v;
}

void conditions_pop(void)
{
	if (condi==-1) { 
		fprintf(stderr,"Error: Unmatched preprocessor directive\n");
		exit(-1);
	}
	condi--;
}

void conditions_invert(void)
{
	cond[condi] = (!cond[condi]);
}

