#include <stdio.h>
#include "tree.h"
#include "list.h"
#include "hash.h"
#include "strfuncs.h"
#include "includepaths.h"


#include "parser.h"


/* we use a tree for the config, though we'll just use it as a
** list of lists, never decending deeper.
*/

tree* config = 0;
list* used_vars = 0;

int config_parse(const char* filename);

char* config_parse_var(char* b,int line)
{
	char subst[4096];
	char left[4096];
	char right[4096];
	int n = 4096;
	char* c = subst;
	char* s = b;
	int cond = 0;
	left[0] = 0;
	right[0] = 0;

	b++;
	if (*b!='(')
	{ 
		fprintf(stderr,"Expected '(' after '$' in line %d\n",line);
		exit(-1);
	}
	b++;
	while (n-- && *b && *b!=')')
	{
		if (*b=='$') b = config_parse_var(b,line);
		else
		if (*b=='?')
		{
			*c = 0;
			cond = 1;
			c = left;
			n = 4096;
			b++;
		}
		else
		if (*b==':')
		{
			*c = 0;
			c = right;
			n = 4096;
			b++;
		}else{
			*c++ = *b++;
		}
	}
	if (*b!=')') {
		fprintf(stderr,"Expected ')' after \"%s\" in line %d\n",s,line);
		exit(-1);
	}
	b++;
	*c = 0;

	{
		node* n = tree_find(config,subst);
		if (n==0)
		{
			fprintf(stderr,"Variable \"%s\" not found in line %d\n",subst,line);
			exit(-1);
		}
		list_add_str_once(used_vars,n->str);
		if (cond)
		{
			if (n->sub && n->sub->first && 
				(
					!strcmp(n->sub->first->str,"1") ||
					!strcmp(n->sub->first->str,"yes") ||
					!strcmp(n->sub->first->str,"YES") ||
					!strcmp(n->sub->first->str,"true") ||
					!strcmp(n->sub->first->str,"TRUE")
				)
			)
			{
				if (left[0]!=0)
					stradd(left);
			}else{
				if (right[0]!=0)
					stradd(right);
			}
		}
		else
		{
			if (n->sub)
			{
				n = n->sub->first;
				while (n)
				{
					stradd(n->str);
					n = n->next;
					if (n) stradd(" ");
				}
			}
		}
	}
	
	return b;
}

void config_parse_handle(char* key,char** val,int nvals,int line,int is_include)
{
	int k;
	node* i = 0;
	if (!is_include)
		i = node_new(key);
	for (k=0;k<nvals;k++)
	{
		char tmp[4096];
		char* b = val[k];
		strstart(tmp,4096);
		while (*b)
		{	
			char tmp2[4096];
			char* a = tmp2;

			while (*b && *b!='$')
			{
				*a++ = *b++;
			}
			*a = 0;
				
			stradd(tmp2);
		
			if (*b=='$')
			{
				b = config_parse_var(b,line);
			}
		}
		strend();

		if (!is_include)
			node_add_sub_str(i,tmp);
		else
			config_parse(tmp);
	}
	if (!is_include)
		tree_add(config,i);
}

void config_parse_line(char* ptr,int line)
{
	char* key = 0;
	char* val[1024];
	int nvals = 0;
	char* end = 0;
	int is_include = 0;
		
	while (*ptr && (*ptr==' ' || *ptr=='\t')) ptr++;
	if (!*ptr) return;
	key = ptr;
	while (*ptr && *ptr!=' ' && *ptr!='\t' && *ptr!='=') ptr++;
	end = ptr;
	while (*ptr && (*ptr==' ' || *ptr=='\t')) ptr++;
	if (*ptr!='=')
	{
		char tmp = *end;
		*end = 0;
		if (strcmp(key,"include")==0)
		{
			is_include = 1;
			*end = tmp;
		}
		else
		{
			*end = tmp;
			*ptr = 0;
			fprintf(stderr,"Expected '=' in line %d after \"%s\"\n",line,key);
			exit(-1);
		}
	}
	
	
	if (!is_include)
	{
		if (*ptr!='=') {
			*ptr = 0;
			fprintf(stderr,"Expected '=' in line %d after \"%s\"\n",line,key);
			exit(-1);
		}else{
			*end = 0;
			ptr++;
		}
	}
	end = 0;
	while (*ptr)
	{
		char* ptr2;
		while (*ptr && (*ptr==' ' || *ptr=='\t')) ptr++;
		if (end) *end = 0;
		if (*ptr) val[nvals++] = ptr;
		if (nvals==1024)
		{
			fprintf(stderr,"Error: limit of 1024 values per key reached\n");
			exit(-1);
		}
		ptr2 = ptr;
		if (*ptr=='"')
		{
			char* start;
			ptr++;
			start = ptr;
			while (*ptr) 
			{
				if (*ptr=='"') break;
				if (*ptr=='\\') ptr++;
				*ptr2++ = *ptr++;
			}
			if (*ptr!='"')
			{
				*ptr2 = 0;
				fprintf(stderr,"Expected '\"' in line %d after \"%s\"\n",line,start);
				exit(-1);
			}
			end = ptr2;
			ptr++;
		}
		else
		{
			while (*ptr) 
			{
				if (*ptr==' ' || *ptr=='\t') break;
				if (*ptr=='\\') ptr++;
				*ptr2++ = *ptr++;
			}
			end = ptr2;
		}
	}
	if (end) *end = 0;

	config_parse_handle(key,val,nvals,line,is_include);
}

int config_parse(const char* filename)
{
	char buf[4096];
	int n;
	int line = 0;
		
	FILE* f = fopen(filename,"r");
	n = 0;

	while (fgets(buf+n,4096-n,f))
	{
		int l = strlen(buf+n);
		line++;
		if (buf[n+l-1]=='\n') l--;
		else{
			fprintf(stderr,"Error: maximum (multi)line length reached in line %d\n",line);
			exit(-1);
		}
		if (buf[n+l-1]=='\r') l--;
		if (buf[n+l-1]=='\\')
		{
			l--;
			n+=l;
		}else{
			n+=l;
			buf[n] = 0;
			if (buf[0]!='#')
				config_parse_line(buf,line);
			n = 0;
		}
	}
	if (n)
	{
		buf[n] = 0;
		if (buf[0]!='#')
			config_parse_line(buf,line);
	}
	fclose(f);

	return 0;
}


void config_apply(void)
{
	node* n;
	
	n = tree_find(config,"RECURSE_INCLUDE_PATHS");
	if (n) list_add_str_once(used_vars,n->str);
	if (n && n->sub)
	{
		n = n->sub->first;
		while (n)
		{
			includepaths_add_rec(n->str);
			n = n->next;
		}
	}

	n = tree_find(config,"INCLUDE_PATHS");
	if (n) list_add_str_once(used_vars,n->str);
	if (n && n->sub)
	{
		n = n->sub->first;
		while (n)
		{
			includepaths_add(n->str);
			n = n->next;
		}
	}
	
	n = tree_find(config,"PRE_INCLUDES");
	if (n) list_add_str_once(used_vars,n->str);
	if (n && n->sub)
	{
		n = n->sub->first;
		while (n)
		{
			list_add_str_once(preincludes,n->str);
			n = n->next;
		}
	}

	n = tree_find(config,"INCLUDE_PATHS");
	if (n) list_add_str_once(used_vars,n->str);
	if (n && n->sub)
	{
		n = n->sub->first;
		while (n)
		{
			includepaths_add(n->str);
			n = n->next;
		}
	}
	
	n = tree_find(config,"SOURCES");
	if (n) list_add_str_once(used_vars,n->str);
	if (n && n->sub)
	{
		n = n->sub->first;
		while (n)
		{
			list_add_str_once(sources,n->str);
			n = n->next;
		}
	}

	n = tree_find(config,"DEFINES");
	if (n) list_add_str_once(used_vars,n->str);
	if (n && n->sub)
	{
		n = n->sub->first;
		while (n)
		{
			list_add_str_once(predefines,n->str);
			n = n->next;
		}
	}

	n = tree_find(config,"LIBRARIES");
	if (n) list_add_str_once(used_vars,n->str);
	if (n && n->sub)
	{
		n = n->sub->first;
		while (n)
		{
			list_add_str_once(libraries,n->str);
			n = n->next;
		}
	}

	n = tree_find(config,"LIBRARY_PATHS");
	if (n) list_add_str_once(used_vars,n->str);
	if (n && n->sub)
	{
		n = n->sub->first;
		while (n)
		{
			list_add_str_once(library_paths,n->str);
			n = n->next;
		}
	}
	
}

void config_init(void)
{
	config = tree_new();
	used_vars = list_new();
}

void config_check(void)
{
	node* n = config->first;
	while (n)
	{
		if (!list_find(used_vars,n->str))
		{
			fprintf(stderr,"Warning: unused variable %s\n",n->str);
		}
		n = n->next;
	}
}

void config_exit(void)
{
	tree_free(config);
	list_free(used_vars);
}
