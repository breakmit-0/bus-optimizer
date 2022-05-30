#include "prim.h"
#include "opti.h"
#include "exn.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <math.h>

uint64_t opti::cout(const graphe* g, const chemin* chemins, size_t n_ch, const size_t* sommets, size_t n_s)
{
    uint64_t somme = 0;
    size_t n = (size_t)floor(sqrtl(g->sommets()));

    for (size_t ligne=0; ligne<n_ch; ligne++)
    {
        somme += opti::k_ligne;
        somme += opti::k_dist * chemins[ligne].nSommets;
    }

    for (size_t point=0; point<n_s; point++)
    {
        size_t k = sommets[point];
        double px = g->get_meta(k).mapx;
        double py = g->get_meta(k).mapy;

        double dmin2 = +INFINITY;

        for (size_t ligne=0; ligne<n_ch; ligne++)
        for (list* pt=chemins[ligne].head; pt!=NULL; pt=pt->suiv)
        {
            double cx = g->get_meta(pt->sommet).mapx;
            double cy = g->get_meta(pt->sommet).mapy;

            double d = (px-cx)*(px-cx) + (py-cy)*(py-cy);

            dmin2 = MIN(dmin2, d);
        }

        if (dmin2 == +INFINITY) {
            return UINT64_MAX;
        }
        somme += (uint64_t) opti::k_proxi * dmin2 * 10000;
    }
    return somme;
}


chemin opti::bouge_terminus(const graphe* g, const chemin ch)
{

    if (ch.nSommets == 0)  {
        size_t pt[1] = {rand() % g->sommets()};
        return chemin::new_chemin(1, pt);
    }

    chemin nouv_chemin = ch.copy();
    assert(nouv_chemin.check());

    if (rand() % 2)
    {

        list* target = nouv_chemin.head;
        int opts = g->arite(target->sommet);

        //le chemin est a un point detache ->impossible??
        if (opts == 0) {
            TRACE();
            FAIL(1, "Erreur dans opti::bouge_terminus / tail, sommet %llu n'a aucun voisins ?\n", 
                target->sommet
            );
        }
        //incompatible :  len(aretes) > 0 mais aretes == NULL ??
        if (g->aretes(target->sommet) == NULL) {
            TRACE();
            FAIL(1, "Erreur dans opti::bouge_termins / tail, (g->aretes(%llu) == NULL) inattendu\n",
                target->sommet
            );
        }

        size_t nouv = (opts == 1)
            ? g->aretes(target->sommet)[0].dest
            : g->aretes(target->sommet)[rand() % opts].dest;

        if (target->suiv == NULL || nouv != target->suiv->sommet) {
            nouv_chemin.add_head(nouv);
            assert(nouv_chemin.check());
        }
        else {
            nouv_chemin.rm_head();
            assert(nouv_chemin.check());
        }
    }
    else
    {

        list* target = nouv_chemin.tail;
        int opts = g->arite(target->sommet);
        
        //le chemin est a un point detache ->impossible??
        if (opts == 0) {
            TRACE();
            FAIL(1, "Erreur dans opti::bouge_terminus / tail, sommet %llu n'a aucun voisins ?\n", 
                target->sommet
            );
        }
        //incompatible :  len(aretes) > 0 mais aretes == NULL ??
        if (g->aretes(target->sommet) == NULL) {
            TRACE();
            FAIL(1, "Erreur dans opti::bouge_termins / tail, (g->aretes(%llu) == NULL) inattendu\n",
                target->sommet
            );
        }

        size_t nouv = (opts == 1) 
            ? g->aretes(target->sommet)[0].dest
            : g->aretes(target->sommet)[rand() % opts].dest;

        if (target->prec == NULL || nouv != target->prec->sommet) {
            nouv_chemin.add_tail(nouv);
            assert(nouv_chemin.check());
        }
        else {
            nouv_chemin.rm_tail();
            assert(nouv_chemin.check());
        }
    }
    return nouv_chemin;
}
