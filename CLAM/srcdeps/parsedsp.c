#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "tree.h"
#include "strfuncs.h"

stack* groupstack = 0;

static int skip = 0;
static char* endgroup = "# End Group";
static char* begingroup = "# Begin Group \"";

static int sourcefile = 0; 
/* 1 = inside Begin..End Source File, before SOURCE=
** 2 = inside Begin..End Source File, after SOURCE= 
*/

static int skipsource = 0;
static int begingroupl = 0;

extern tree* config;

int strcmp_begin(const char* a,const char* b)
{
	while (*a && *b && *a==*b)
	{
		a++; b++;
	}
	if (*b==0) return 0;
	
	return strcmp(a,b);
}

int strcmp_eol(const char* a,const char* b)
{
	while (*a && *b && *a==*b)
	{
		a++; b++;
	}
	if (
		(*a==0 || *a=='\r' || *a=='\n') &&
		(*b==0 || *b=='\r' || *b=='\n') ) return 0;
	
	return strcmp(a,b);
}

void parse_dsp_add_needed_includepaths(void)
{
	node* n = tree_find(config,"RECURSE_INCLUDE_PATHS");
	if (n && n->sub)
	{
		n = n->sub->first;
		while (n)
		{
			stradd(" /I \"");
			stradd(n->str);
			stradd("\"" );
			n = n->next;
		}
	}
}

void parse_dsp_add_preincludes(void)
{
	node* n = tree_find(config,"PRE_INCLUDES");
	if (n && n->sub)
	{
		n = n->sub->first;
		while (n)
		{
			stradd(" /FI\"");
			stradd(n->str);
			stradd("\"" );
			n = n->next;
		}
	}
}

void parse_dsp_insert_sources()
{

}

void parse_dsp_insert_headers()
{

}

void copy_without_includes(char* tgt,const char* src)
{
	while (*src && *src!='\r' && *src!='\n')
	{
		if (!strcmp_begin(src,"/I \""))
		{
			src += 4;
			while (*src && *src!='"') src++;
			if (*src=='"') src++;
			if (*src==' ') src++;
		}
		else if (!strcmp_begin(src,"/I\""))
		{
			src += 3;
			while (*src && *src!='"') src++;
			if (*src=='"') src++;
			if (*src==' ') src++;
		}
		else if (!strcmp_begin(src,"/FI\""))
		{
			src += 4;
			while (*src && *src!='"') src++;
			if (*src=='"') src++;
			if (*src==' ') src++;
		}
		else if (!strcmp_begin(src,"/D \"CLAM_USE_XML\""))
		{
			src += strlen("/D \"CLAM_USE_XML\"");
			while (*src && *src!='"') src++;
			if (*src=='"') src++;
			if (*src==' ') src++;
		}else{
			*tgt++ = *src++;
		}
	}
	*tgt = 0;
}

FILE* infile = 0;
FILE* outfile = 0;

void parse_dsp_line(const char* buf,int line)
{
	if (sourcefile==1)
	{
		if (strcmp_eol(buf,""))  /* ignore empty lines */
		{
			if (strncmp(buf,"SOURCE=",7))
			{
				fprintf(stderr,
	"Error: expected SOURCE=... after # Begin Source File in line %d\n",line);
				fprintf(stderr,buf);
				exit(-1);
			}
			sourcefile = 2;
			if (!strcmp_eol(buf+7,"settings.cfg"))
			{
				skipsource = 1;
			}
			else
			{
				fprintf(outfile,"# Begin Source File\n\n");
			}
		}
	}

	if (!skip)
	{
		if (!strcmp_eol(buf,"# Begin Source File"))
		{
			sourcefile = 1;
		}
	}

	if (!strcmp_begin(buf,begingroup))
	{
		char groupname[1024];
		const char* a = buf;
		char* b = groupname;
		char* q = 0;
		if (begingroupl==0) begingroupl=strlen(begingroup);
		a += begingroupl;
		while (*a)
		{
			if (*a=='"') q = b;
			*b++ = *a++;
		}
		if (!q) {
			fprintf(stderr,"Did not find terminating '\"' in line %d\n",line);
			exit(-1);
		}
		*q = 0;
		fprintf(stderr,"GROUPNAME=[%s]\n",groupname);
		stack_push(groupstack,groupname);
		if (!strcmp(groupname,"Source Files"))
		{
			printf("SKIP\n");
			skip = 1;
		}
		if (!strcmp(groupname,"Header Files"))
		{
			printf("SKIP\n");
			skip = 1;
		}
	}
	
	if (!skip && !skipsource && sourcefile!=1) {

		if (
			!strcmp_begin(buf,"# ADD CPP ") || 
			!strcmp_begin(buf,"# ADD BASE CPP ")
		) {
			char tmp[4096];
			char tmp2[4096];
			copy_without_includes(tmp2,buf);
			strstart(tmp,4096);
			stradd(tmp2);
			/** add all needed_includepaths here **/

			parse_dsp_add_needed_includepaths();
			parse_dsp_add_preincludes();

			stradd("\n");
			
			strend();
			fputs(tmp,outfile);
		}
		else
		{
			fputs(buf,outfile);
		}
	}
	
	if (!strcmp_eol(buf,endgroup))
	{	
		fprintf(stderr,"> %s\n",endgroup);
		if (skip && !strcmp(stack_top(groupstack),"Source Files")) skip = 0;
		if (skip && !strcmp(stack_top(groupstack),"Header Files")) skip = 0;
		stack_pop(groupstack);
	}

	if (!strcmp_eol(buf,"# End Target"))
	{
		parse_dsp_insert_sources();
		parse_dsp_insert_headers();
	}	

	if (!strcmp_eol(buf,"# End Source File"))
	{
		sourcefile = 0;
		if (skipsource) skipsource = 0;
	}

}

void parse_dsp(const char* filename)
{
	char buf[4096];
	int line = 0;

	groupstack = stack_new();

	infile = fopen(filename,"r");
	if (!infile) {
		fprintf(stderr,"Could not open %s for reading\n",filename);
		exit(-1);
	}
	outfile = fopen("PARSETMP.dsp","w");
	if (!outfile)
	{
		fprintf(stderr,"Could not open PARSETMP.dsp for writing\n");
		exit(-1);
	}
	while (fgets(buf,4096,infile))
	{
		line++;
		parse_dsp_line(buf,line);
	}
	
	fclose(infile);
	fclose(outfile);

	infile = fopen("PARSETMP.dsp","r");
	if (!infile) {
		fprintf(stderr,"Could not open PARSETMP.dsp for reading\n");
		exit(-1);
	}
	outfile = fopen(filename,"w");
	if (!outfile)
	{
		fprintf(stderr,"Could not open %s.dsp for writing\n",filename);
		exit(-1);
	}

	while (fgets(buf,4096,infile))
	{
		fputs(buf,outfile);
	}

	fclose(infile);
	fclose(outfile);
	
	remove("PARSETMP.dsp");
}

int main()
{	
	config_parse("settings.cfg");
	parse_dsp("SimpleMain.dsp");

	return 0;
}
