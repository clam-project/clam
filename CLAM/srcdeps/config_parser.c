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

typedef struct config_data
{
	char* in;
	char* out_start;
	char* out;
	int n;
	const char* filename;
	int line;
} config_data;

#define COPYINOUT(d) { *d->out++ = *d->in++; d->n--; if (d->n==0) { fprintf(stderr,"Error: parsing space exceeded in %s:%d\n",d->filename,d->line); exit(-1); } }
#define CHAROUT(d,c) { *d->out++ = c; d->n--; if (d->n==0) { fprintf(stderr,"Error: parsing space exceeded in %s:%d\n",d->filename,d->line); exit(-1); } }
#define ENDOUT(d) { *d->out = 0; }

void config_parse_line_sub(config_data* d,int insidecond,int cond)
{
	char* var = 0;
	*d->out = 0;

	while (*d->in)
	{
		/* if we are inside a condition, and not parsing a variable, we should
		** return if we encounter a ':' or a ')'
		*/ 
		if (!var && insidecond && (*d->in==':' || *d->in==')')) return;

		if (*d->in=='$')
		{
			/* encountering a variable, starting with $( */
			var = d->out;
			d->in++;
			if (*d->in!='(')
			{
				fprintf(stderr,
				"Expected '(' after '$' in line %s:%d\n",d->filename,d->line);
				exit(-1);
			}
			d->in++;
		}

		if (var)
		{
			/* we are inside a variable */

			if (*d->in == ')')
			{
				/* encountered the end of the variable */
				listkey* k;
				d->in++;	
				
				/* terminate variable */
				ENDOUT(d);

				/* search for the variable */
				k = listhash_find(config,var);
				if (k==0)
				{
					fprintf(stderr,
						"Variable \"%s\" not found in line %s:%d\n",
						var,d->filename,d->line);
					exit(-1);
				}
				
				/* marked the variable as used */
				list_add_str_once(used_vars,k->str);

				/* reset the token string pointer to where we encountered the 
				** variable so we will overwrite it with it's value(s)
				*/
				d->out = var;
				var = 0;

				if (cond) 
				{
					/* only if we are inside a true evalution */
					/* copy the variable's value(s) to the token string */
					if (k->l)
					{
						item* i = k->l->first;
						while (i)
						{
							char* c = i->str;
							while (*c)
							{
								CHAROUT(d,*c++)
							}
							i = i->next;
							if (i) {
								CHAROUT(d,0)
							}
						}
					}
				}
			}else if (*d->in == '?')
			{
				/* we are inside a condition in the from $(VARIABLE?IF_TRUE:IF_FALSE) */
				int subcond;
				/* terminate variable */
				ENDOUT(d);
				d->in++;

				/* evuluate variable */				
				subcond = var_true(var,d->filename,d->line);

				/* reset the token string pointer to where we encountered the 
				** variable so we will overwrite it with the result of the condition
				** expansion
				*/
				d->out = var;
				var = 0;

				/* recursively evaluate left and right hand side. depending on the
				** condition, either one will be added to the token list
				*/
				config_parse_line_sub(d,1,cond&subcond);
				if (*d->in==':')
				{
					d->in++;
					config_parse_line_sub(d,1,cond&(!subcond));
				}
				if (*d->in==')')
				{
					d->in++;
				}
			}else
			{
				/* still copying variable name */
				COPYINOUT(d);
			}
		}else{
			/* not in a variable - just copying the value to the token list */
			if (cond)
			{
				/* if true, we copy the value char by char to the token list */
				
				/* encountering space, which means we have to insert a token seperation */
				if (*d->in == ' ' || *d->in == '\t')
				{
					/* skip all extra spaces */
					while (*d->in && (*d->in == ' ' || *d->in == '\t')) d->in++;

					/* insert token seperation if necesary */
					if (d->out!=d->out_start && *(d->out-1)) CHAROUT(d,0);
				}else{
					/* '=' is a special case: it is not necesarily seperated with spaces,
					** so we insert token seperations just in case
					*/
					if (*d->in == '=')
					{
						if (d->out!=d->out_start && *(d->out-1)) CHAROUT(d,0);
						COPYINOUT(d);
						CHAROUT(d,0);
					}else{
						COPYINOUT(d);
					}
				}
			}else{
				/* if false, we just skip the value char by char */
				d->in++;
			}
		}
	}
	/* terminate token list */
	CHAROUT(d,0);
	CHAROUT(d,0);
}

void config_parse_line(char* ptr,const char* filename,int line)
{
	char tmp[4096];
	config_data d;
	d.filename = filename;
	d.line = line;
	d.in = ptr;
	d.out = d.out_start = tmp;
	d.n = 4096;

	/* parse the line and start the tokens, \0 seperated in \0\0 terminated
	** in tmp */	
	config_parse_line_sub(&d,0,1);

	/* we now have a token list */

	{
		int k = 0;
		char* ptr = tmp;
		char* key = ptr;
		listkey* i = 0;
		int isinclude = 0;

		/* special case: the first token is include */
		if (!strcmp(key,"include"))
		{
			isinclude = 1;
		}
		
		while (*ptr)
		{
			if (isinclude)
			{
				if (k>0) config_parse(ptr);
			}
			else
			{
				/* if not, the first token is a variable name,
				** and the second token should be "=" */
				if (k==1)
				{
					if (!strcmp(ptr,"="))
					{
						i = listhash_add_key_once(config,key);
						if (i->l)
						{
							list_free(i->l);
							i->l = list_new();
						}
					}
				}
				if (k>1)
				{
					if (i) listkey_add_item_str(i,ptr);
				}
			}
			while (*ptr) ptr++;
			ptr++;
			k++;
		}
	}
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
