#include <stdio.h>
#include "config_parser.h"

extern char* empty_vcproj_lines[];

static FILE* outfile = 0;


extern void vcproj_parse(const char* outFilename)
{
	outfile = fopen(outFilename, "w");
	typedef enum { header, config, files } ParserStates;
	ParserStates state;

	state = header;
	int nline = 0;
	while(empty_vcproj_lines[nline])
	{
		const char* line = empty_vcproj_lines[nline]; 
		if (state==header)
		{
			if( strstr(line, "\tName=") )
			{
				fprintf(outfile, line, program->first->str );
			}
			else
			{
				fprintf(outfile, line);
				if( strstr(line, "<Platforms>") )
					state = config;
			}
		}
		else
		{
			fprintf(outfile, line);
		}
		nline++;
	}
	fclose(outfile);
}
