#ifndef CONFIG_H
#define CONFIG_H 

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct{
    char music_dir[512];
} Config;

bool config_exsist();
void config_create(Config *config, const char *path);
void config_read(Config *config);

#endif
