#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "listhash.h"
#include "tree.h"
#include "stack.h"
#include "list.h"
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

void winstyle(char* str)
{
	while (*str)
	{
		if (*str=='/') *str='\\';
		str++;
	}
}

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

FILE* infile = 0;
FILE* outfile = 0;

void dsp_parse_add_needed_includepaths(void)
{
	item* i = needed_includepaths->first;
	while (i)
	{
		if (i->str && i->str[0]!=0)
		{
			char tmp[1024];
			strncpy(tmp,i->str,1024);
			winstyle(tmp);
			stradd(" /I \"");
			stradd(tmp);
			stradd("\"" );
		}
		i = i->next;
	}
}

void dsp_parse_add_pre_includes(void)
{
	item* i = pre_includes->first;
	/* visual is stupid. /FI accepts a filename only, not a path,
	** so we have to split this in /I and /FI
	while (i)
	{
		char tmp[1024];
		strncpy(tmp,i->str,1024);
		winstyle(tmp);
		stradd(" /FI\"");
		stradd(tmp);
		stradd("\"" );
		i = i->next;
	}
	*/
	while (i)
	{
		if (i->str && i->str[0]!=0)
		{
			char tmp[1024];
			char* ptr;
			char* filename = tmp;
			strncpy(tmp,i->str,1024);
			winstyle(tmp);
			ptr = tmp;
			while (*ptr)
			{
				if (*ptr=='\\') filename = ptr;
				ptr++;
			}
			if (*filename=='\\')
			{
				*filename = 0;
				filename++;
				stradd(" /I \"");
				stradd(tmp);
				stradd("\"" );
			}
			stradd(" /FI\"");
			stradd(filename);
			stradd("\"" );
		}
		i = i->next;
	}
}

void dsp_parse_add_defines(void)
{
	item* i = defines->first;
	while (i)
	{
		if (i->str && i->str[0]!=0)
		{
			char tmp[1024];
			strncpy(tmp,i->str,1024);
			stradd(" /D \"");
			stradd(tmp);
			stradd("\"" );
		}
		i = i->next;
	}
}

void dsp_parse_insert_recurse(tree* t,list* repeatcheck,int type)
{
	node * n = t->first;
	char* typestr = type ? "Headers" : "Sources";
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
			dsp_parse_insert_recurse(n->sub,repeatcheck,type);
			fprintf(outfile,"# End Group\n");
		}else{
			char tmp[1024];
			strncpy(tmp,n->str,1024);
			winstyle(tmp);
			fprintf(outfile,"# Begin Source File\n\n");
			fprintf(outfile,"SOURCE=%s\n",tmp);
			fprintf(outfile,"# End Source File\n");
		}
		n = n->next;
	}
}


void dsp_parse_insert(int type)
{
	tree* t = tree_new();
	item* i = type ? guessed_headers->first : guessed_sources->first;
	list* repeatcheck = list_new();

	char* typestr = type ? "Header Files" : "Source Files";

	while (i)
	{
		char tmp[1024];
		char* ptr;
		char* start = 0;

		tree* c = t;

		strncpy(tmp,i->str,1024);
		
		ptr = tmp;

		while (*ptr)
		{
			if (start==0) start = ptr;
			if (*ptr=='/' || *ptr=='\\')
			{
				*ptr = 0;
				if (strcmp(start,"..") && strcmp(start,"src"))
				{
					node* n = tree_add_str_once(c,start);
					if (!n->sub) n->sub = tree_new();
					c = n->sub;
				}
				start = 0;
			}
			ptr++;
		}

		tree_add_str_once(c,i->str);
		
		i = i->next;
	}

	fprintf(outfile,"# Begin Group \"%s\"\n\n",typestr);
	dsp_parse_insert_recurse(t,repeatcheck,type);
	fprintf(outfile,"# End Group\n");

	list_free(repeatcheck);
	tree_free(t);
}

void dsp_parse_insert_sources()
{
	dsp_parse_insert(0);
}

void dsp_parse_insert_headers()
{
	dsp_parse_insert(1);
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
		else if (!strcmp_begin(src,"/D \""))
		{
			src += 4;
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
		}else{
			*tgt++ = *src++;
		}
	}
	*tgt = 0;
}

void dsp_parse_line(const char* buf,int line)
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
		stack_push(groupstack,groupname);
		if (!strcmp(groupname,"Source Files"))
		{
			skip = 1;
		}
		if (!strcmp(groupname,"Header Files"))
		{
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

			dsp_parse_add_defines();
			dsp_parse_add_pre_includes();
			dsp_parse_add_needed_includepaths();

			stradd("\n");
			
			strend();
			fputs(tmp,outfile);
		}
		else
		{
			if (!strcmp_eol(buf,"# End Target"))
			{
				dsp_parse_insert_sources();
				dsp_parse_insert_headers();
			}	
			fputs(buf,outfile);
		}
	}
	
	if (!strcmp_eol(buf,endgroup))
	{	
		if (skip && !strcmp(stack_top(groupstack),"Source Files")) skip = 0;
		if (skip && !strcmp(stack_top(groupstack),"Header Files")) skip = 0;
		stack_pop(groupstack);
	}

	if (!strcmp_eol(buf,"# End Source File"))
	{
		sourcefile = 0;
		if (skipsource) skipsource = 0;
	}

}

void dsp_parse(const char* filename)
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
		dsp_parse_line(buf,line);
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
