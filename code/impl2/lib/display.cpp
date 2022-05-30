#include "display.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//suppose un graphe carre 10 x 10

void disp::print_lignes(graphe* g, chemin* chemins, int n_ch, int* points, int n_pt)
{
    int n = roundl(sqrtl(g->sommets()));

    char* buffer = (char*) malloc((n * (n+1) + 1) * sizeof(char));

    for (int y=0; y<n; y++) {
        for (int x=0; x<n; x++)
        {
            buffer[(n+1)*y + x] = ' ';
        }
        buffer[(n+1)*y + n] = '\n';
    }

    for(int ligne=0; ligne<n_ch; ligne++)
    for(list* cur=chemins[ligne].head; cur!=NULL; cur=cur->suiv)
    {
        int x = cur->sommet % n;
        int y = cur->sommet / n;

        buffer[(n+1)*y + x] = 'o';
    }

    for (int i=0; i<n_pt; i++) {
        int x = points[i] % n;
        int y = points[i] / n;

        buffer[(n+1) * y + x] = (buffer[(n+1) * y + x] == 'o') ? '#' : 'x'; 
    }


    buffer[n*(n+1)] = '\0';
    printf("\n%s\n", buffer);
    free(buffer);
    return;
}


