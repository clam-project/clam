#ifndef __parser__
#define __parser__

#include "list.h"

extern list* preincludes;
extern list* sources;
extern list* headers;
extern list* predefines;
extern list *includepaths;
extern list *needed_includepaths;
extern list* libraries;
extern list* library_paths;

extern int gendepend;
extern int recursesrcs;

void parser_init(void);
void parser_exit(void);

void parser_run(const char* filename);



#endif
