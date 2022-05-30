#ifndef INCLUDE_PRIM_H
#define INCLUDE_PRIM_H

#include <stdlib.h>

#define MIN(x,y) (x < y ? x : y)
#define MAX(x,y) (x > y ? x : y)

struct sommet
{
    size_t val;
    double mapx;
    double mapy;
};


struct arete
{
    double length;
    size_t dest;
};

#endif