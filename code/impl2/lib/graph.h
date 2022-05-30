#ifndef INCLUDE_GRAPH_H
#define INCLUDE_GRAPH_H


#include "prim.h"

struct adj_data {
    int n;
    sommet meta;
    arete* liste;
};

class graphe
{
private:
    size_t nSommets;
    adj_data adj[];

public:

    static graphe* new_graphe(size_t, adj_data(*)(size_t));
    static void free_graphe(graphe*);

    size_t sommets() const;
    int arite(size_t) const;
    arete* aretes(size_t) const;
    sommet get_meta(size_t) const;
};

extern size_t constr_carre_size; //parametere supppelementaire
adj_data constr_carre(size_t);

#endif