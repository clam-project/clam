#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "includepaths.h"
#include "config_parser.h"
#include "objdepname.h"

void usage(void)
{
	fprintf(stderr,"TODO: write usage info\n");
}

int main(int argc,char** argv)
{
	
	int i;

	gendepend = 1;

	parser_init();

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
					case 'f':
						gendepend = 2;
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
					case 'D':
						list_add_str_once(predefines,argv[i]+2);
						break;
					case 'c':
						config_init();
						config_parse(argv[i]+2);
						config_apply();
						config_check();
						config_exit();
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
	
	if (gendepend==2)
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
			item* i = includepaths->first;
			printf("INCLUDES =");
			while (i)
			{
				if (list_find(needed_include_paths,i->str))
				{
					printf("\\\n -I%s",i->str);
				}
				i = i->next;
			}
			printf("\n\n");
		}
	}
				
	parser_exit();

	return 0;
}
