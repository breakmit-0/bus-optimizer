
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#include "graph.h"


size_t graphe::sommets() const{
    return this->nSommets;
}

int graphe::arite(size_t s) const{
    return this->adj[s].n;
}

arete* graphe::aretes(size_t s) const{
    assert(s <= this->nSommets);
    return this->adj[s].liste;
}

sommet graphe::get_meta(size_t s) const{
    return this->adj[s].meta;
}

graphe* graphe::new_graphe(size_t nSommets, adj_data(*constr)(size_t))
{
    graphe* g = (graphe*) malloc(sizeof(graphe) + nSommets*sizeof(adj_data));
    g->nSommets = nSommets;

    for (size_t i=0; i<nSommets; i++) {
        g->adj[i] = constr(i);
        assert(g->adj[i].liste != NULL);
    }
    return g;
}

void graphe::free_graphe(graphe* g)
{
    for (size_t i=0; i<g->nSommets; i++) {
        free(g->adj[i].liste);
    }
    free(g);
    return;
}

inline arete gen_arete(size_t sommet_num) {
    return (arete) {
        .length = 1,
        .dest = sommet_num
    };
}

size_t constr_carre_size = 10;
adj_data constr_carre(size_t i)
{
    const size_t SIZE = constr_carre_size;

    adj_data res = {0, (sommet) {
        .val = i,
        .mapx = 10.0 * (i % constr_carre_size),
        .mapy = 10.0 * (i / constr_carre_size)
    }, NULL};

    if (i == 0)
    {
        res.n = 2;
        res.liste = (arete*) malloc(2 * sizeof(arete));
        res.liste[0] = gen_arete(i + 1);
        res.liste[1] = gen_arete(i + SIZE);
        goto end;
    }
    if (i == SIZE-1)
    {
        res.n = 2;
        res.liste = (arete*) malloc(2 * sizeof(arete));
        res.liste[0] = gen_arete(i - 1);
        res.liste[1] = gen_arete(i + SIZE);
        goto end;
    }
    if (i == SIZE*(SIZE-1)) 
    {
        res.n = 2;
        res.liste = (arete*) malloc(2 * sizeof(arete));
        res.liste[0] = gen_arete(i + 1);
        res.liste[1] = gen_arete(i - SIZE);
        goto end;
    }
    if (i == SIZE*SIZE-1)
    {
        res.n = 2;
        res.liste = (arete*) malloc(2 * sizeof(arete));
        res.liste[0] = gen_arete(i - 1);
        res.liste[1] = gen_arete(i - SIZE);
        goto end;
    }
    if (i / SIZE == 0)
    {
        res.n = 3;
        res.liste = (arete*) malloc(3 * sizeof(arete));
        res.liste[0] = gen_arete(i + 1);
        res.liste[1] = gen_arete(i - 1);
        res.liste[2] = gen_arete(i + SIZE);
        goto end;
    }
    if (i / SIZE == SIZE-1)
    {
        res.n = 3;
        res.liste = (arete*) malloc(3 * sizeof(arete));
        res.liste[0] = gen_arete(i + 1);
        res.liste[1] = gen_arete(i - 1);
        res.liste[2] = gen_arete(i - SIZE);
        goto end;
    }
    if (i % SIZE == 0)
    {
        res.n = 3;
        res.liste = (arete*) malloc(3 * sizeof(arete));
        res.liste[0] = gen_arete(i + 1);
        res.liste[1] = gen_arete(i - SIZE);
        res.liste[2] = gen_arete(i + SIZE);
        goto end;
    }
    if (i % SIZE == SIZE-1)
    {
        res.n = 3;
        res.liste = (arete*) malloc(3 * sizeof(arete));
        res.liste[0] = gen_arete(i - 1);
        res.liste[1] = gen_arete(i + SIZE);
        res.liste[2] = gen_arete(i - SIZE);
        goto end;
    }

        res.n = 4;
        res.liste = (arete*) malloc(4 * sizeof(arete));
        res.liste[0] = gen_arete(i + 1);
        res.liste[1] = gen_arete(i - 1);
        res.liste[2] = gen_arete(i + SIZE);
        res.liste[3] = gen_arete(i - SIZE);

end:
    return res;
}