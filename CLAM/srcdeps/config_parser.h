#ifndef __configparser__
#define __configparser__

#include "tree.h"

extern tree* config;

int config_parse(const char* filename);
void config_apply(void);
void config_init(void);
void config_check(void);
void config_exit(void);

#endif
