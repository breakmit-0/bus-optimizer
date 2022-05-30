#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <time.h>

#include "lib/exn.h"
#include "lib/graph.h"
#include "lib/chemin.h"
#include "lib/opti.h"
#include "lib/display.h"
#include "lib/gui.h"
#include "lib/input.h"

int main()
{
    srand((unsigned)time(0));

    size_t nodes = input::read_graph("rues-graphe.gph");
    graphe* g = graphe::new_graphe(nodes, input::extern_constr);
    input::destroy();
    
    printf("g a %llu sommets.\n", g->sommets());

    srand((unsigned) time(0));

    size_t points[10];
    for (int i=0; i<10; i++) {
        points[i] = rand() % nodes;
    }

    chemin ch1 = chemin::new_chemin(0, NULL);
    chemin ch2 = chemin::new_chemin(0, NULL);

    chemin lignes[2] = {ch1, ch2};

    assert(ch1.check());
    assert(ch2.check());

    uint64_t cout1 = opti::cout(g, lignes, 2, points, 10);
    uint64_t cout2;
    time_t start_time = time(0);
    int n =0;
    int stable = 0;

    //algo glouton pour l'instant

    while(n < 20000 && stable < 2000)
    {
        assert(lignes[0].check());
        assert(lignes[1].check());

        chemin nouv[2];
        nouv[0] = opti::bouge_terminus(g, lignes[0]);
        nouv[1] = opti::bouge_terminus(g, lignes[1]);

        assert(nouv[0].check());
        assert(nouv[1].check());

        cout2 = opti::cout(g, nouv, 2, points, 10);

        stable = (cout2 >= cout1 ? stable + 1 : 0);

        if (cout2 <= cout1 || (rand() % n) >= n-1)
        {
            chemin::free_chemin(lignes[0]);
            chemin::free_chemin(lignes[1]);
            lignes[0] = nouv[0];
            lignes[1] = nouv[1];
            cout1 = cout2;
        }

        printf("\rmvt #%i | cost = %lli | ", n, cout1);
        printf("[...]        ");

        n++;
    }

    printf("\nstable sur %i iterations, %i total\n\n", stable, n);
    //disp::print_lignes(g, lignes, 2, points, 10);

    disp::gui_lignes(g, lignes, 2, points, 10);

    return 0;
};