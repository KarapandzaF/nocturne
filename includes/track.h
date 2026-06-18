#ifndef TRACK_H
#define TRACK_H
#include "dynamic_array.h"

typedef struct{
    char path[256];
    
    char title[128];
    char artist[128];
    char album[128];

}Track;

DEFINE_ARRAY(Track);
DEFINE_ARRAY(i32);

#endif

