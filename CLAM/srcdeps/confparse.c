#include <stdio.h>
#include "tree.h"
#include "strfuncs.h"

/* we use a tree for the config, though we'll just use it as a
** list of lists, never decending deeper.
*/

tree* config = 0;

void config_parse_handle(char* key,char** val,int nvals,int line)
{
	int k;
	node* i = node_new(key);
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
				char subst[4096];
				int n = 4096;
				char* c = subst;
				char* s = b;
				b++;
				if (*b!='(')
				{ 
					fprintf(stderr,"Expected '(' after '$' in line %d\n",line);
					exit(-1);
				}
				b++;
				while (n-- && *b && *b!=')')
				{
					*c++ = *b++;
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
		}
		strend();

		node_add_sub_str(i,tmp);
	}
	tree_add(config,i);
}

void config_parse_line(char* ptr,int line)
{
	char* key = 0;
	char* val[1024];
	int nvals = 0;
	char* end = 0;
	while (*ptr && *ptr==' ') ptr++;
	if (!*ptr) return;
	key = ptr;
	while (*ptr && *ptr!=' ' && *ptr!='=') ptr++;
	end = ptr;
	while (*ptr && *ptr==' ') ptr++;
	if (*ptr!='=') {
		*ptr = 0;
		fprintf(stderr,"Expected '=' in line %d after \"%s\"\n",line,key);
		exit(-1);
	}else{
		*end = 0;
		ptr++;
	}
	end = 0;
	while (*ptr)
	{
		char* ptr2;
		while (*ptr && *ptr==' ') ptr++;
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
				if (*ptr==' ') break;
				if (*ptr=='\\') ptr++;
				*ptr2++ = *ptr++;
			}
			end = ptr2;
		}
	}
	if (end) *end = 0;

	config_parse_handle(key,val,nvals,line);
}

int config_parse(const char* filename)
{
	char buf[4096];
	int n;
	int line = 0;
	
	FILE* f = fopen(filename,"r");
	n = 0;

	config = tree_new();
	
	while (fgets(buf+n,4096-n,f))
	{
		int l = strlen(buf+n);
		line++;
		if (buf[n+l-1]=='\n') l--;
		else{
			fprintf(stderr,"Error: maximum variable length reached in line %d\n",line);
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
			config_parse_line(buf,line);
			n = 0;
		}
	}
	if (n)
	{
		buf[n] = 0;
		config_parse_line(buf,line);
	}
	fclose(f);
	
	return 0;
}
