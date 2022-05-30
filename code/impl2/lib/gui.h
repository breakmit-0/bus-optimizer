#ifndef INCLUDE_GUI_H
#define INCLUDE_GUI_H

#include "exn.h"
#include "prim.h"
#include "graph.h"
#include "chemin.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

namespace disp
{
    void gui_lignes(const graphe*, const chemin*, const size_t, const size_t* ,const size_t);  
}





#endif