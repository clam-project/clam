#ifndef __configparser__
#define __configparser__

#include "listhash.h"

extern listhash* config;
extern list* used_vars;

extern list* cxxflags_debug;
extern list* cxxflags_release;
extern list* link_flags;
extern list* libraries_debug;
extern list* libraries_release;
extern list* library_paths;

int config_parse(const char* filename);
void config_apply(void);
void config_init(void);
void config_check(void);
void config_exit(void);

#endif
