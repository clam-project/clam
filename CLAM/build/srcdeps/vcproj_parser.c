#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "parser.h"
#include "config_parser.h"
#include "list.h"
#include "strfuncs.h"


extern char* empty_vcproj_lines[];

static FILE* outfile = 0;

/* Main functions for mapping folder structure into virtual vc6 folders
 * structure. The filetype parameter indicates the kind of files we are
 * going to insert: 0 for regular c/c++ source files (.c, .C, .cpp, .cxx, etc. ),
 * 1 for c/c++ headers ( .h, .hxx, etc. ) and 2 for Qt ui files.
 * Now it this parameter is an enum (also conserving old int values)
 */

static void vcproj_parse_insert( FileType filetype );
static void vcproj_parse_insert_recurse( tree* t, list* repeatCheck, FileType type );

/* Shorthand functions for calling vcproj_parse_insert() function
 * passing the adequate value for type
 */
static void vcproj_parse_insert_sources();
static void vcproj_parse_insert_headers();
static void vcproj_parse_insert_ui_rules();


static void vcproj_parse_insert_regular_file( char* );
static void vcproj_parse_insert_ui_file( char* );
static void vcproj_parse_insert_mocable_header( char*);



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
			else first = 0;
			strncpy(tmp,i->str,1024);
			winstyle(tmp);
			stradd(tmp);
		}
		i = i->next;
	}
}

// private function used from release and debug versions
void vcproj_parse_add_libraries(int isDebug)
{
	int first = 1;
	item* i;
	if (isDebug)
		i = libraries_debug->first;
	else
		i = libraries_release->first;

	while (i)
	{
		if (i->str && i->str[0]!=0)
		{
			if (!first) stradd(" ");
			else first = 0;
			stradd(i->str);
			stradd(".lib");
		}
		i = i->next;
	}
}
	
void vcproj_parse_add_release_libraries(void)
{
	vcproj_parse_add_libraries(0/* is NOT debug*/);
}

void vcproj_parse_add_debug_libraries(void)
{
	vcproj_parse_add_libraries(1/* IS debug*/);
}

void vcproj_parse_add_library_paths(void)
{
	int first = 1;
	item* i = library_paths->first;
	while (i)
	{
		if (i->str && i->str[0]!=0)
		{
			char tmp[1024];
			strncpy(tmp,i->str,1024);
			winstyle(tmp);

			if (!first) stradd(",");
			else first = 0;
			stradd(tmp);
		}
		i = i->next;
	}
}

/** The public function to be called from the main */
extern void vcproj_parse(const char* outFilename)
{
	typedef enum { header, configRelease, configDebug, files, theRest } ParserStates;
	ParserStates state;
	int nline = 0;

	outfile = fopen(outFilename, "w");
	state = header;
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
			// TODO
			vcproj_parse_insert( source );
			//fprintf(outfile, line);

		}
		else
		{
			fprintf(outfile, line);
		}
		nline++;
	}
	fclose(outfile);
}

void vcproj_parse_insert_sources()
{
	vcproj_parse_insert(source);
}

void vcproj_parse_insert_headers()
{
	vcproj_parse_insert(header);
}

void vcproj_parse_insert_ui_rules()
{
	vcproj_parse_insert(qt);
}

void vcproj_parse_insert(FileType type)
{
	char* typestr = NULL;
	list* filelist;
	tree* filetree = tree_new();
	list* repeatcheck = list_new();

	if ( type == header ) 
	{
		typestr = "Header Files";
		filelist = guessed_headers;
	}
	else if ( type == source ) 
	{ 
		typestr = "Source Files";
		filelist = guessed_sources;
	}
	else if ( type == qt ) 
	{
		typestr = "Qt .ui Files";
		filelist = ui_files;
	}
	else
	{
		typestr = "Unknown files :o";
		filelist = NULL;
	}


	generate_files_tree(filelist, filetree);

	fprintf(outfile,"# Begin Group \"%s\"\n\n",typestr);
	vcproj_parse_insert_recurse(filetree,repeatcheck,type);
	fprintf(outfile,"# End Group\n");

	list_free(repeatcheck);
	tree_free(filetree);
}

void vcproj_parse_insert_recurse(tree* t,list* repeatcheck, FileType type)
{
	node * n = t->first;
	const char* typestr = filetype_str(type);
	while (n)
	{
		if (n->sub)
		{
			/* stupid visual cannot deal with same name at different levels */
			int cnt = 0;
			item* i = repeatcheck->first;
			while (i)
			{
				if (!strcmp(i->str,n->str)) cnt++;
				i = i->next;
			}
			list_add_str(repeatcheck,n->str);
			if (cnt>0)
			{
				fprintf(outfile,"# Begin Group \"%s %s No. %d\"\n\n",n->str,typestr,cnt);
			}else{
				fprintf(outfile,"# Begin Group \"%s %s\"\n\n",n->str,typestr);
			}
			vcproj_parse_insert_recurse(n->sub,repeatcheck,type);
			fprintf(outfile,"# End Group\n");
		}else{
			if ( type == header )
			{
				assert( mocable_headers != NULL );
				
				if ( list_find( mocable_headers, n->str ) )
					vcproj_parse_insert_mocable_header( n->str );
				else
					vcproj_parse_insert_regular_file( n->str );
			}
			else if ( type == qt )
			{
				vcproj_parse_insert_ui_file( n->str );
			}
			else
				vcproj_parse_insert_regular_file( n->str );
		}
		n = n->next;
	}
}


// BIG TODO
void vcproj_parse_insert_regular_file( char* a) {}
void vcproj_parse_insert_ui_file( char* a) {}
void vcproj_parse_insert_mocable_header( char*a) {}

