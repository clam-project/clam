#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "config_parser.h"
#include "list.h"
#include "strfuncs.h"

extern char* empty_vcproj_lines[];

static FILE* outfile = 0;

void vcproj_parse_add_needed_includepaths(void)
{
	item* i = includepaths->first;
	int first = 1;
	while (i)
	{
		// we actually are traversing "needed_includepaths" but we do it this
		// strange way because we want to preserve the order as it is in "includepaths"
		if (i->str && i->str[0]!=0 && list_find(needed_includepaths,i->str) )
		{
			char tmp[1024];
			
			if (!first) stradd(",");
			first = 0;
			strncpy(tmp,i->str,1024);
			winstyle(tmp);
			stradd(tmp);
		}
		i = i->next;
	}
}


void vcproj_parse_add_release_libraries(void)
{
	item* i = libraries_release->first;
	int first = 1;
	while (i)
	{
		if (i->str && i->str[0]!=0)
		{
			if (!first) stradd(" ");
			first = 0;
			stradd(i->str);
			stradd(".lib");
		}
		i = i->next;
	}
}

void vcproj_parse_add_debug_libraries(void)
{
	item* i = libraries_debug->first;
	int first = 1;
	while (i)
	{
		if (i->str && i->str[0]!=0)
		{
			if (!first) stradd(" ");
			first = 0;
			stradd(i->str);
			stradd(".lib");
		}
		i = i->next;
	}
}

void vcproj_parse_add_library_paths(void)
{
	item* i = library_paths->first;
	while (i)
	{
		if (i->str && i->str[0]!=0)
		{
			char tmp[1024];
			strncpy(tmp,i->str,1024);
			winstyle(tmp);

			stradd(" /libpath:\"");
			stradd(tmp);
			stradd("\"");
		}
		i = i->next;
	}
}

/** The public function to be called from the main */
extern void vcproj_parse(const char* outFilename)
{
	outfile = fopen(outFilename, "w");
	typedef enum { header, configRelease, configDebug, files, theRest } ParserStates;
	ParserStates state;

	state = header;
	int nline = 0;
	while(empty_vcproj_lines[nline])
	{
		const char* line = empty_vcproj_lines[nline]; 
		if(state==header)
		{
			if ( strstr(line, "\tName=") )
			{
				fprintf(outfile, line, program->first->str );
			}
			else
			{
				fprintf(outfile, line);
				if ( strstr(line, "<Platforms>") )
					state = configRelease;
			}
		}
		else if(state==configRelease)
		{
			if (strstr(line, "\tAdditionalIncludeDirectories="))
			{
				char includesbuf[4096];
				strstart(includesbuf, 4096);
				vcproj_parse_add_needed_includepaths( );
				fprintf(outfile, line, includesbuf);
				strend();
			}
			else if(strstr(line,"\tForcedIncludeFiles=" ) )
			{ 
				//TODO get it from the proper place (not hardwired!)
				fprintf(outfile, line, "preinclude.hxx");
			}
			else if ( strstr(line,"\tAdditionalDependencies=") )
			{
				char libsbuf[4096];
				strstart(libsbuf, 4096);
				vcproj_parse_add_release_libraries();
				fprintf(outfile, line, libsbuf);
				strend();
			}
			else if ( strstr(line, "\tOutputFile=") )
			{
				fprintf( outfile, line, program->first->str );
			}
			else if ( strstr(line, "\tAdditionalLibraryDirectories=") )
			{
				char libdirsbuf[4096];
				strstart(libdirsbuf, 4096);
				vcproj_parse_add_library_paths();
				fprintf( outfile, line, libdirsbuf );
				strend();
			}
			else  // tag not found. maybe should change state?
			{
				fprintf(outfile, line);
				if ( strstr(line, "Name=\"Debug|Win32\"") )
						state = configDebug;
			}
		}
		else if(state == configDebug)
		{
			if (strstr(line, "\tAdditionalIncludeDirectories="))
			{
				char includesbuf[4096];
				strstart(includesbuf, 4096);
				vcproj_parse_add_needed_includepaths( );
				fprintf(outfile, line, includesbuf);
				strend();
			}
			else if ( strstr(line,"\tAdditionalDependencies=") )
			{
				char libsbuf[4096];
				strstart(libsbuf, 4096);
				vcproj_parse_add_debug_libraries();
				fprintf(outfile, line, libsbuf);
				strend();
			}
			else if ( strstr(line, "\tOutputFile=") )
			{
				fprintf( outfile, line, program->first->str );
			}

			else if ( strstr(line, "\tAdditionalLibraryDirectories=") )
			{
				char libdirsbuf[4096];
				strstart(libdirsbuf, 4096);
				vcproj_parse_add_library_paths();
				fprintf( outfile, line, libdirsbuf );
				strend();
			}

			else
			{
				fprintf(outfile, line);
				if ( strstr(line, "<Files>"))						
					state = files;
			}	
		}
		else if(state == files)
		{
			if(0)
			{
			}
			else
			{
				fprintf(outfile, line);
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
