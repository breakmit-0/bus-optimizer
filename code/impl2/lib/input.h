#ifndef INCLUDE_INPUT_H
#define INCLUDE_INPUT_H

#include "exn.h"
#include "prim.h"
#include "graph.h"

#include <stdlib.h>

/*
note: the 'dest' pointers in 'arete' are NOT strings but reference to strings from the 'names'
table, they can be compared with 'strcmp' but should be compared literally using '==' with
elements of 'names'
*/
namespace input
{
    struct arete {
        double len;
        size_t dest;
    };

    struct sommet {
        int arite;
        arete* aretes;
    };

    struct raw {
        size_t size;
        sommet* sommets;
        char** names;
    };

    static raw data;
    void destroy(void);
    size_t read_graph(const char* fname);
    adj_data extern_constr(const size_t i);
}




#endif