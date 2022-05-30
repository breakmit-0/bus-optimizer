#ifndef INCLUDE_OPTI_H
#define INLCUDE_OPTI_H

#include "graph.h"
#include "chemin.h"
#include <stdint.h>

namespace opti
{
    //cout supplementaire par ligne
    constexpr uint64_t k_ligne = 1500;
    //cout supplementaire par aret
    constexpr uint64_t k_aret = 10;
    //cout par unite de distance de trajet
    constexpr uint64_t k_dist = 1;
    //cout par unite de distance de ne pas atteindre un objectif
    constexpr uint64_t k_proxi = 200;
    //coefficient de cout des tests pratiques
    constexpr uint64_t k_trajet = 50;

    typedef chemin (*voisinage)(const graphe* g, const chemin ch);

    uint64_t cout(const graphe* g, const chemin* chemins, size_t n_ch, const size_t* points, size_t n_s);

    chemin bouge_terminus(const graphe* g, const chemin ch);
    chemin change_interne(const graphe* g, const chemin ch);
}




#endif