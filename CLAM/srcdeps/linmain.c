#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "includepaths.h"
#include "config_parser.h"
#include "objdepname.h"
#include "config_parser.h"

void usage(void)
{
	fprintf(stderr,"Usage srcdeps SETTINGSFILE\n");
	fprintf(stderr,"   or srcdeps -d SETTINGSFILE SRCFILE\n");
	exit(-1);
}

void makefilevars_generate(void)
{
	FILE* outfile = stdout;
/*
	if (!outfile)
	{
		fprintf(stderr,"Error: could not open %s\n",filename);
		exit(-1);
	}
*/
	{
		item* i = sources->first;
		fprintf(outfile,"SOURCES =");
		while (i)
		{
			if (i->str && i->str[0]!=0)
			{
				fprintf(outfile,"\\\n %s",i->str);
			}
			i = i->next;
		}
		fprintf(outfile,"\n\n");
	}

	{
		item* i = sources->first;
		fprintf(outfile,"OBJECTS =");
		while (i)
		{
			char objname[2048];
			convert_to_objname(objname,2048,i->str);
			fprintf(outfile,"\\\n %s",objname);

			i = i->next;
		}
		fprintf(outfile,"\n\n");
	}

	{
		item* i = sources->first;
		fprintf(outfile,"DEPENDS =");
		while (i)
		{
			char depname[2048];
			if (i->str && i->str[0]!=0)
			{
				convert_to_depname(depname,2048,i->str);
				fprintf(outfile,"\\\n %s",depname);
			}
			i = i->next;
		}
		fprintf(outfile,"\n\n");
	}

	{
		item* i = needed_includepaths->first;
		fprintf(outfile,"INCLUDES =");
		while (i)
		{
			if (i->str && i->str[0]!=0)
				fprintf(outfile,"\\\n -I%s",i->str);

			i = i->next;
		}
		fprintf(outfile,"\n\n");
	}

	{
		item* i = library_paths->first;
		fprintf(outfile,"LIBRARY_PATHS =");
		while (i)
		{
			if (i->str && i->str[0]!=0)
				fprintf(outfile,"\\\n -L%s",i->str);

			i = i->next;
		}
		fprintf(outfile,"\n\n");
	}

	{
		item* i = libraries->first;
		fprintf(outfile,"LIBRARIES =");
		while (i)
		{
			if (i->str && i->str[0]!=0)
				fprintf(outfile,"\\\n -l%s",i->str);

			i = i->next;
		}
		fprintf(outfile,"\n\n");
	}

	{
		item* i = predefines->first;
		fprintf(outfile,"DEFINES =");
		while (i)
		{
			if (i->str && i->str[0]!=0)
				fprintf(outfile,"\\\n -D%s",i->str);
			i = i->next;
		}
		fprintf(outfile,"\n\n");
	}

	fclose(outfile);
}

int main(int argc,char** argv)
{
	gendepend = 2;
	
	if (argc<2) usage();
	if (strcmp(argv[1],"-d")==0)
		gendepend=1;

	if (gendepend==1)
	{
		if (argc!=4) usage();
	}
	else
	{
		if (argc!=2) usage();
	}

	if (gendepend==1)
	{
		recursesrcs = 0;
		list_add_str(sources,argv[3]);
	}else{
		recursesrcs = 1;
	}
	
	parser_init();
	
	config_init();
	config_parse(argv[1]);
	config_apply();
	config_check();
	config_exit();

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
		makefilevars_generate();		
	}

	parser_exit();

	return 0;
}
