#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "includepaths.h"
#include "config_parser.h"
#include "dsp_parser.h"

int main(int argc,char** argv)
{
	if (argc!=3)
	{
		fprintf(stderr,"Usage srcdeps SETTINGSFILE DSPFILE\n");
		exit(-1);
	}
	
	gendepend = 0;
	recursesrcs = 1;
	
	config_init();
	config_parse(argv[1]);

	parser_init();

	config_check();

	{
		item* i = guessed_sources->first;
		while (i)
		{
			parser_run(i->str);
			
			i = i->next;
		}
	}

	dsp_parse(argv[2]);

	parser_exit();
	config_exit();

	return 0;
}
