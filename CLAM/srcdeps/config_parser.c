#include <stdio.h>
#include "list.h"
#include "listhash.h"
#include "hash.h"
#include "strfuncs.h"

#include "parser.h"
#include "config_parser.h"

listhash* config = 0;
list* used_vars = 0;

int config_parse(const char* filename);

int var_true(char* subst,const char* filename,int line)
{
	listkey* n = listhash_find(config,subst);
	if (n==0)
	{
		fprintf(stderr,
		"Variable \"%s\" not found in line %s:%d\n",subst,filename,line);
		exit(-1);
	}
	list_add_str_once(used_vars,n->str);


	if (n->l && n->l->first && 
		(
			!strcmp(n->l->first->str,"1") ||
			!strcmp(n->l->first->str,"yes") ||
			!strcmp(n->l->first->str,"YES") ||
			!strcmp(n->l->first->str,"true") ||
			!strcmp(n->l->first->str,"TRUE")
		)
	)
	{
		return 1;
	}
	return 0;
}

int decn(int n,const char* filename,int line)
{
	n--;
	if (n<=0)
	{
		fprintf(stderr,
		"Maximum variable length exceeded in line %s:%d\n",filename,line);
		exit(-1);
	}
	return n;
}

char* config_parse_var(char* b,const char* filename,int line,int insidecond,int cond,char** resptr,int n)
{
	int iscond = 0;
	char* res = *resptr;
	while (*b && (!insidecond || (*b!=':' && *b!=')')))
	{
		if (*b=='$') {
			char var[256];
			char* a = var;
			b++;
			if (*b!='(')
			{ 
				fprintf(stderr,
				"Expected '(' after '$' in line %s:%d\n",filename,line);
				exit(-1);
			}
			b++;
			while (*b && *b!=')')
			{
				if (*b=='?')
				{
					int subcond = 0;
					b++;
					*a = 0;
					iscond = 1;
					subcond = var_true(var,filename,line);
					b = config_parse_var(b,filename,line,1,cond&subcond,&res,n);
					if (*b==':')
					{
						b++;
						b = config_parse_var(b,filename,line,1,cond&(!subcond),&res,n);
					}
				}else{
					*a++ = *b++;
				}
			}
			if (*b!=')')
			{
				fprintf(stderr,
				"Expected ')' after %s in line %s:%d\n",var,filename,line);
				exit(-1);
			}
			*a = 0;
			if (!iscond)
			{
				listkey* k = listhash_find(config,var);
				if (k==0)
				{
					fprintf(stderr,
						"Variable \"%s\" not found in line %s:%d\n",
						var,filename,line);
					exit(-1);
				}
				list_add_str_once(used_vars,k->str);
				if (cond)
				{
					if (k->l)
					{
						item* i = k->l->first;
						while (i)
						{
							char* c = i->str;
							while (*c)
							{
								*res++ = *c++;
								n=decn(n,filename,line);
							}
							i = i->next;
							if (i) {
								*res++=' ';
								n=decn(n,filename,line);
							}
						}
					}
				}
			}
			b++;
		}
		else
		{
			if (cond)
			{
				*res++ = *b++;
				n=decn(n,filename,line);
				*res = 0;
			}else{
				b++;
			}
		}
	}
	*res = 0;
	*resptr = res;
	return b;
}

void config_parse_handle(char* keystr,char** val,int nvals,const char* filename,int line,int is_include)
{
	int k;
	listkey* i = 0;
	if (!is_include)
		i = listhash_add_key_once(config,keystr);
	for (k=0;k<nvals;k++)
	{
		char tmp[4096];
		char* b = val[k];
		while (*b)
		{	
			char* res = tmp;
			b = config_parse_var(b,filename,line,0,1,&res,4096);
		}
		if (!is_include)
			listkey_add_item_str(i,tmp);
		else
			config_parse(tmp);
	}
}

void config_parse_line(char* ptr,const char* filename,int line)
{
	char* keystr = 0;
	char* val[1024];
	int nvals = 0;
	char* end = 0;
	int is_include = 0;

	while (*ptr && (*ptr==' ' || *ptr=='\t')) ptr++;
	if (!*ptr) return;
	keystr = ptr;
	while (*ptr && *ptr!=' ' && *ptr!='\t' && *ptr!='=') ptr++;
	end = ptr;
	while (*ptr && (*ptr==' ' || *ptr=='\t')) ptr++;
	if (*ptr!='=')
	{
		char tmp = *end;
		*end = 0;
		if (strcmp(keystr,"include")==0)
		{
			is_include = 1;
			*end = tmp;
		}
		else
		{
			*end = tmp;
			*ptr = 0;
			fprintf(stderr,"Expected '=' in line %s:%d after \"%s\"\n",filename,line,keystr);
			exit(-1);
		}
	}
	
	
	if (!is_include)
	{
		if (*ptr!='=') {
			*ptr = 0;
			fprintf(stderr,"Expected '=' in line %s:%d after \"%s\"\n",filename,line,keystr);
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
			fprintf(stderr,"Error: limit of 1024 values per key reached in line %s:%d\n",filename,line);
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
				fprintf(stderr,"Expected '\"' in line %s:%d after \"%s\"\n",filename,line,start);
				exit(-1);
			}
			end = ptr2;
			ptr++;
		}
		else
		{
			int brackets = 0;
			while (*ptr) 
			{
				if (*ptr=='(') brackets++;
				if (*ptr==')') brackets--;
				if (brackets == 0 && (*ptr==' ' || *ptr=='\t'))
					break;
				if (*ptr=='\\') ptr++;
				*ptr2++ = *ptr++;
			}
			if (brackets!=0)
			{
				fprintf(stderr,"Mismatch '(' ')' in line %s:%d\n",filename,line);
				exit(-1);
			}
			end = ptr2;
		}
	}
	if (end) *end = 0;

	config_parse_handle(keystr,val,nvals,filename,line,is_include);
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
			fprintf(stderr,"Error: maximum (multi)line length reached in line %s:%d\n",
				filename,line);
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
				config_parse_line(buf,filename,line);
			n = 0;
		}
	}
	if (n)
	{
		buf[n] = 0;
		if (buf[0]!='#')
			config_parse_line(buf,filename,line);
	}
	fclose(f);

	return 0;
}

void config_init(void)
{
	used_vars = list_new();

	config = listhash_new();

	listhash_add_key_once(config,"LIBRARIES")->l = list_new();
	listhash_add_key_once(config,"LIBRARY_PATHS")->l = list_new();
	listhash_add_key_once(config,"CXXFLAGS")->l = list_new();
	listhash_add_key_once(config,"SOURCES")->l = list_new();
	listhash_add_key_once(config,"PRE_INCLUDES")->l = list_new();
	listhash_add_key_once(config,"DEFINES")->l = list_new();
	listhash_add_key_once(config,"SEARCH_INCLUDES")->l = list_new();
	listhash_add_key_once(config,"SEARCH_RECURSE_INCLUDES")->l = list_new();

	list_add_str_once(used_vars,"LIBRARIES");
	list_add_str_once(used_vars,"LIBRARY_PATHS");
	list_add_str_once(used_vars,"CXXFLAGS");
	list_add_str_once(used_vars,"SOURCES");
	list_add_str_once(used_vars,"PRE_INCLUDES");
	list_add_str_once(used_vars,"DEFINES");
	list_add_str_once(used_vars,"SEARCH_INCLUDES");
	list_add_str_once(used_vars,"SEARCH_RECURSE_INCLUDES");
}

void config_check(void)
{

	listkey* n = config->first;
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
	listhash_free(config);
	list_free(used_vars);
}
