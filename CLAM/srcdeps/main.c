#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "hash.h"
#include "stack.h"
#include "strfuncs.h"
#include "recursedir.h"
#include "conditions.h"

/* for mkdir */
#include <sys/stat.h>
#include <sys/types.h>

int verbose = 0;

/* hash of header extension -> source extension,
** used to estimate required source for included
** headers if recursesrc is set
*/
hash* extmap = 0;

void extmap_init(void)
{
	extmap = hash_new();
	
	/* these are the most logical */
	hash_add(extmap,".h",".c");
	hash_add(extmap,".hh",".cc");
	hash_add(extmap,".hxx",".cxx");
	hash_add(extmap,".hpp",".cpp");
	hash_add(extmap,".H",".C");
	
	/* these are weird, but also occur */
	hash_add(extmap,".h",".cc");
	hash_add(extmap,".h",".cxx");
	hash_add(extmap,".h",".cpp");
	hash_add(extmap,".h",".C");

	hash_add(extmap,".H",".cc");
	hash_add(extmap,".H",".cxx");
	hash_add(extmap,".H",".cpp");
	hash_add(extmap,".H",".C");

	hash_add(extmap,".hpp",".cxx");
	hash_add(extmap,".hxx",".cpp");
}

/* hash with all defines during a parser_run */
hash *defines = 0;

/* list of include files found during a parser_run */
list *includes = 0;

/* list of files to pre-include in each parser_run, 
** set with command line option -include 
*/
list *preincludes = 0;

/* list of paths to search include files.
** set with command line options -I (add a single path)
** and -R (add a path and all its subdirectories)
*/
list *includepaths = 0;

/* list of sources to parse. specified on the command
** line. if the recursesrc (command line option -r)
** is set, all the estimated sources are added to this
** list, and parsed as well
*/
list *sources = 0;

/* list of include paths that are actually needed to 
** compile the sources
*/
list *needed_includepaths = 0;

/* list of all includes checked when building the
** needed_includepaths, for efficiency.
*/
list *includes_checked = 0;

/* stack of the files visited during recursion, used for
** error reporting
*/
stack* filenamestack = 0;

/* flag whether to estimate required source for included headers
*/
int recursesrcs = 0;

/* flag whether to generate object (.o) and depend (.d) file on source/header
** dependencies */
int gendepend = 1;

const char* strptr_skip_end(const char* ptr)
{
	while (*ptr++);
	return ptr;
}

const char* strptr_skip_spaces(const char* ptr)
{
	while (*ptr && (*ptr==' ' || *ptr=='\t' || *ptr=='\n' || *ptr=='\r'))
		ptr++;
	return ptr;
}

const char* strptr_copy_nonspaces(const char* ptr,char* tgt,int max)
{
	int i = 0;
	char* start = tgt;
	while (*ptr && *ptr!=' ' && *ptr!='\t' && *ptr!='\n' && *ptr!='\r')
	{
		i++;
		if (i==max) {
			*tgt = 0;
			fprintf(stderr,"Error: not enough space in target string to copy %s...\n",start);
		}
		*tgt++ = *ptr++;
	}
	*tgt = 0;
	return ptr;
}

const char* strptr_copy_until(const char* ptr,char term,char* tgt,int max)
{
	int i = 0;
	char* start = tgt;
	while (*ptr && (*ptr!=term))
	{
		i++;
		if (i==max) {
			*tgt = 0;
			fprintf(stderr,"Error: not enough space in target string to copy %s...\n",start);
		}
		*tgt++ = *ptr++;
	}
	*tgt = 0;
	return ptr;
}

/* add a path to the include paths */
/* Don't add /CVS paths! */
void includepaths_add(const char* str)
{
	const char* ptr = str;
	ptr = strptr_skip_end(ptr);
	while (ptr!=str && *ptr!='/') ptr--;
	if (*ptr=='/' && !strcmp(ptr,"/CVS")) return;

	list_add_str_once(includepaths,str);
}


/* Callback for recursedir to call on each directory, see includepaths_add_rec */
int cb_includepaths_add(const char* dir,void*dum)
{
	includepaths_add(dir);
	return 0;
}

/* Recurse a directory tree and add all subdirs to the includepaths list */
void includepaths_add_rec(const char* str)
{
	recursedir(str,-1,0,cb_includepaths_add,0);
}

/* Look for a filename in all includepaths, and open it if found.
** Returns NULL or the filehandle. pathret is set to the path where
** the file was found.
*/
FILE* fopen_in_includepaths(const char* filename,const char** pathret)
{
	item* i = includepaths->first;
	while (i)
	{
		FILE* ret;
		char tmp[2048];
		strstart(tmp,2048);
		stradd(i->str);
		stradd("/");
		stradd(filename);
		strend();
		ret = fopen(tmp,"r");
		if (ret) {
			*pathret = i->str;
			return ret;
		}
		i = i->next;
	}

	return 0;
}

/* Return a pointer to the extension in a filename (the last dot) 
** or NULL if extension was found
*/
char* extension(char* filename)
{
	char* ptr = filename;
	ptr = (char*)(strptr_skip_end(ptr));
	while (ptr!=filename)
	{
		ptr--;
		if (*ptr=='.') return ptr;
	}
	return 0;
}

void usage(void)
{
	fprintf(stderr,"TODO: write usage info\n");
}

/* Check is a file exists */
int file_exists(const char* filename)
{
	FILE* f = fopen(filename,"r");
	if (f) {
		fclose(f);
		return 1;
	}
	return 0;
}

int parser_recurse(const char* filename);

/* Handle the filename as parsed from a #include directive:
** - look for the file in all includepaths
** - parse it recursively
** - add the path where it was found to the list of needed includepaths
** - if recursesrcs flag is set, look for the implementation source
**   trying different extensions occurding to extmap, and add that
**   source to the sources list
*/
int parser_include(const char* filename)
{
	const char* path;
	char tmp[2048];
	char tmp2[2048];
	
	FILE* f = fopen_in_includepaths(filename,&path);
		
	if (!f) 
	{
		return 0;
	}else{
		fclose(f);
	}

	strstart(tmp,2032); /* leave room for possible extension change */
	if (strcmp(path,""))
	{
		stradd(path);
		stradd("/");
	}
	stradd(filename);
	strend();	

	parser_recurse(tmp);

	list_add_str_once(includes,tmp);

	/* we used to add tmp to includes_checked, but
	** this went wrong when the a different path and 
	** a different include let to the same file,
	** for example PATH a/b , INCLUDE c and
	** PATH a INCLUDE b/c
	*/
	strstart(tmp2,2048);
	stradd(path);
	stradd("/:/");
	stradd(filename);
	strend();	

	if (!list_find(includes_checked,tmp2))
	{
		item* i = extmap->l->first;
		
		list* possible_impl_files = list_new();

		list_add_str_once(needed_includepaths,path);

		list_add_str(includes_checked,tmp2);

		if (recursesrcs)
		{
			char* ext = extension(tmp);
			char  headerext[16];
			strncpy(headerext,ext,16);

			while (i)
			{
				/* try various extensions */
				const char* map = hash_item_value(extmap,i,headerext);
				if (map)
				{	
					strncpy(ext,map,16);
					if (file_exists(tmp))
					{
						list_add_str_once(possible_impl_files,tmp);
					}
				}
				i = i->next;
			}
	
			strcpy(ext,headerext); /* reset filename to header */

			{
				int n = list_size(possible_impl_files);
				if (n>1)
				{
					item* k = possible_impl_files->first;
					fprintf(stderr,"Multiple possible implementations for %s:\n",tmp);
					while (k)
					{
						fprintf(stderr,"  %s\n",k->str);
						k = k->next;
					}
				}
				if (n==1)
				{
					list_add_str_once(sources,possible_impl_files->first->str);
				}
			}
		}						
		list_free(possible_impl_files);
	}
	return 1;
}

/* parse a preprocessor directive. ptr is at the start of the directive */
const char* parser_directive(const char* ptr)
{
	if (conditions_total()) 
	/* only do includes, defines and undefs if the 
	** conditions total is true */
	{
		if (strncmp(ptr,"include",7)==0)
		{
			char term;
			char tmp[2048];

			ptr+=7;

parse_include_filename: 
			ptr = strptr_skip_spaces(ptr);
			term = *ptr;
			if (term=='"') {
				term='"'; ptr++;
			}
			else if (term=='<') {
				term='>'; ptr++;
			}
			else term = 0;
			if (term==0)
			{
				ptr = strptr_copy_nonspaces(ptr,tmp,2048);

				{
					/* this goto is a bit of a kludge to handing defines in includes */
					const char* val = hash_value(defines,tmp);
					if (val)
					{
						strncpy(tmp,val,2048);
						ptr = tmp; 
						goto parse_include_filename;
					}else{
						fprintf(stderr,"Error: include %s syntax error\n",tmp);
					}
				}
			}
			else
			{
				ptr = strptr_copy_until(ptr,term,tmp,2048);
			}

			if (verbose) { fprintf(stderr,"including %s\n",tmp); }

			if (parser_include(tmp)==0)
			{
				if (term=='>') {
					/* a system include was not found, but that's okay, because
					** we don't generate dependencies for system includes anyway
					*/
				}
				else
				{
					char* i = 0;
					fprintf(stderr,"Error: include \"%s\" not found\n",tmp);

					while ((i==stack_top(filenamestack)))
					{
						fprintf(stderr,"  from %s\n",i);
						stack_pop(filenamestack);
					}
					
					fprintf(stderr,"Checked in:\n");
					{
						item* i = includepaths->first;
						while (i)
						{
							fprintf(stderr,"  %s\n",i->str);
							i = i->next;
						}
					
					}
					exit(-1);
				}
			}
			return ptr;
		}
		if (strncmp(ptr,"define",6)==0)
		{
			char key[256];
			char val[256];
			ptr+=6;
			ptr = strptr_skip_spaces(ptr);
			ptr = strptr_copy_nonspaces(ptr,key,256);
			ptr = strptr_skip_spaces(ptr);
			ptr = strptr_copy_nonspaces(ptr,val,256);
			if (verbose) fprintf(stderr,"defining %s = %s\n",key,val);
			hash_set(defines,key,val);
			return ptr;
		}
		if (strncmp(ptr,"undef",5)==0)
		{
			char key[256];
			ptr+=5;
			ptr = strptr_skip_spaces(ptr);
			ptr = strptr_copy_nonspaces(ptr,key,256);
			hash_rmv(defines,key);
			return ptr;
		}
	}
	if (strncmp(ptr,"ifdef",5)==0)
	{
		char key[256];
		int c;
		ptr+=5;
		ptr = strptr_skip_spaces(ptr);
		ptr = strptr_copy_nonspaces(ptr,key,256);
		
		c = hash_value(defines,key)!=0;
		conditions_push(c);
		if (verbose) fprintf(stderr,"ifdef %s = %d\n",key,c);
		return ptr;
	}
	if (strncmp(ptr,"ifndef",6)==0)
	{
		char key[256];
		int c;
		ptr+=6;
		ptr = strptr_skip_spaces(ptr);
		ptr = strptr_copy_nonspaces(ptr,key,256);

		c = hash_value(defines,key)==0;
		conditions_push(c);
		if (verbose) fprintf(stderr,"ifndef %s = %d\n",key,c);
		return ptr;
	}
	if (strncmp(ptr,"if",2)==0)
	{
		ptr+=2;
do_if:
		/* TODO! We should parse the #if preprop.directive as well */
		conditions_push(0);
	}
	if (strncmp(ptr,"elif",4)==0)
	{
		ptr+=4;
		conditions_pop();
		goto do_if;
	}
	if (strncmp(ptr,"else",4)==0)
	{
		ptr+=4;
		if (verbose) fprintf(stderr,"else\n");
		conditions_invert();
		return ptr;
	}
	if (strncmp(ptr,"endif",5)==0)
	{
		ptr+=5;
		if (verbose) fprintf(stderr,"endif\n");
		conditions_pop();
		return ptr;
	}
	return ptr;
}

/* Parse ptr and return after the end of a C-style comment, 
** or the end of the line, or NULL of we are still in the comment
*/
const char* parser_comment(const char* ptr)
{
	while (*ptr)
	{
		if (*ptr=='*' && *(ptr+1)=='/') return ptr+2;
		ptr++;
	}
	return 0;
}

/* parse a line. return 1 if after parsing the line we are still inside 
** a c-style comment */
int parser_line(const char* ptr)
{
restart:
	ptr = strptr_skip_spaces(ptr);
	if (*ptr=='/' && *(ptr+1)=='/') return 0;
	if (*ptr=='/' && *(ptr+1)=='*') 
	{
		ptr = parser_comment(ptr+2);
		if (ptr==0) {
			return 1;
		}
		goto restart;
	}
	if (*ptr=='#')
	{
		ptr++;
		ptr = strptr_skip_spaces(ptr);
		if (*ptr)
		{
			ptr = parser_directive(ptr);
		}
	}
	return 0;
}

int parser_recurse(const char* filename)
{
	FILE* f = fopen(filename,"r");
	int in_comment = 0;
	static char buf[2048];

	if (f==0) return 0;
		
	stack_push(filenamestack,filename);

	while (fgets(buf, 2048, f))
	{
		const char* ptr = buf;
		if (in_comment)
		{
			ptr = parser_comment(ptr);
			if (ptr) in_comment = 0;
		}
		if (!in_comment)
		{
			in_comment = parser_line(ptr);
		}
	}
	
	fclose(f);

	stack_pop(filenamestack);

	return 1;
}

void convert_to_objname(char* objname,int maxlen,const char* filename)
{
	const char* ptr = filename;
	ptr = strptr_skip_end(ptr);
	while (ptr!=filename)
	{
		ptr--;
		if (*ptr=='/') { ptr++; break; }
	}
	strstart(objname,maxlen-3); /* leave space for extension */
	stradd("obj/");
	stradd(ptr);
	strend();

	{
		char *ext = extension(objname);
		strncpy(ext,".o",3);
	}
}

void convert_to_depname(char* depname,int maxlen,const char* filename)
{
	const char* ptr = filename;
	ptr = strptr_skip_end(ptr);
	while (ptr!=filename)
	{
		ptr--;
		if (*ptr=='/') { ptr++; break; }
	}
	strstart(depname,maxlen-3); /* leave space for extension */
	stradd("dep/");
	stradd(ptr);
	strend();

	{
		char *ext = extension(depname);
		strncpy(ext,".d",3);
	}
}

void parser_run(const char* filename)
{
	if (verbose) fprintf(stderr,"create defines hash\n");
	defines = hash_new();

	includes = list_new();
	filenamestack = stack_new();

	conditions_start();

	{
		item* i = preincludes->first;
		while (i)
		{
			parser_recurse(i->str);
			i = i->next;
		}
	}
	
	if (parser_recurse(filename))
	{
		/* {	
			item* i = includes->first;
			while (i)
			{
				parser_recurse(i->str,1);
				i = i->next;		
			}
		} */

		if (gendepend)
		{
			item* i;
			char objname[2048];
			char depname[2048];

			i = includes->first;

			convert_to_objname(objname,2048,filename);

			printf("%s: %s ",objname,filename);
			while (i)
			{
				printf("\\\n %s",i->str);

				i = i->next;
			}
			printf("\n\n");

			i = includes->first;

			convert_to_depname(depname,2048,filename);

			printf("%s:",depname);
			while (i)
			{
				printf("\\\n %s",i->str);

				i = i->next;
			}
			printf("\n\n");
		}
		else
		{
			char depname[2048];
			FILE* df;
			convert_to_depname(depname,2048,filename);

			printf("%s: %s \n\n",depname,filename);
			
			mkdir("dep",0775);
			
			df = fopen(depname,"w");
			
			if (df == 0)
			{
				fprintf(stderr,"Failed to open %s for writing\n",depname);
				exit(-1);
			}
			else
			{
				item* i;
				char objname[2048];
				char depname[2048];

				i = includes->first;

				convert_to_objname(objname,2048,filename);

				fprintf(df,"%s: %s ",objname,filename);
				while (i)
				{
					fprintf(df,"\\\n %s",i->str);

					i = i->next;
				}
				fprintf(df,"\n\n");

				i = includes->first;

				convert_to_depname(depname,2048,filename);

				fprintf(df,"%s:",depname);
				while (i)
				{
					fprintf(df,"\\\n %s",i->str);

					i = i->next;
				}
				fprintf(df,"\n\n");
			}
			
			fclose(df);
			
		}
	}else{
		fprintf(stderr,"Could not open %s\n",filename);
	}
	if (verbose) fprintf(stderr,"free defines hash\n");
	hash_free(defines);
	
	list_free(includes);
	stack_free(filenamestack);

	conditions_end();
}

int main(int argc,char** argv)
{
	
	int i;

	extmap_init();

	sources = list_new();

	includepaths = list_new();

	preincludes = list_new();

	includes_checked = list_new();

	needed_includepaths = list_new();

	for (i=1;i<argc;i++)
	{
		if (argv[i][0]=='-')
		{
			int correct_option = 1;
			if (argv[i][1]=='\0') correct_option = 0;
			else if (argv[i][2]=='\0')
			{
				switch (argv[i][1])
				{
					case 'h':
						usage();
						exit(-1);
					case 'r':
						recursesrcs = 1;
						break;
					case 'D':
						gendepend = 0;
						break;
					default:
						correct_option = 0;
						break;
				}
			}
			else
			{
				switch (argv[i][1])
				{
					case 'R':
						includepaths_add_rec(argv[i]+2);
						break;
					case 'I':
						includepaths_add(argv[i]+2);
						break;
					default:
						if (strcmp(argv[i],"-include")==0)
						{
							i++;
							list_add_str_once(preincludes,argv[i]);
						}
						else
						{
							correct_option = 0;
						}
						break;
				}
			}

			if (!correct_option)
			{
				fprintf(stderr,"Error: unknown option '%s'\n",argv[i]);
				usage();
				exit(-1);
			}
		}else{
			list_add_str(sources,argv[i]);
		}
	}

	{
		item* i = sources->first;
		while (i)
		{
			parser_run(i->str);
			
			i = i->next;
		}
	}
	
	if (!gendepend)
	{
		/* note that parser_run may have added more sources */
		{
			item* i = sources->first;
			printf("SOURCES =");
			while (i)
			{
				printf("\\\n %s",i->str);
				i = i->next;
			}
			printf("\n\n");
		}

		{
			item* i = sources->first;
			printf("OBJECTS =");
			while (i)
			{
				char objname[2048];
				convert_to_objname(objname,2048,i->str);
				printf("\\\n %s",objname);

				i = i->next;
			}
			printf("\n\n");
		}

		{
			item* i = sources->first;
			printf("DEPENDS =");
			while (i)
			{
				char depname[2048];
				convert_to_depname(depname,2048,i->str);
				printf("\\\n %s",depname);

				i = i->next;
			}
			printf("\n\n");
		}

		{
			item* i = needed_includepaths->first;
			printf("INCLUDES =");
			while (i)
			{
				printf("\\\n -I%s",i->str);

				i = i->next;
			}
			printf("\n\n");
		}
	}
				
	list_free(sources);

	list_free(includepaths);

	list_free(preincludes);

	list_free(includes_checked);

	list_free(needed_includepaths);
	

	return 0;
}
