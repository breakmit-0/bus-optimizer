/*
le graphe est guaranti planaire (reseau routier), on applique A* aux 26k somets
pour precalculer les distances minimales entre les sommets
on pourrait appliquer floyd-marshall mais il faudrait quelques GB de RAM

*/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <set>
#include <map>

struct sommet {
    size_t id;
    double x;
    double y;
};

struct adj_data {
    size_t arite;
    size_t* voisins;
    double* dist;
};

struct graphe
{
    size_t n;
    sommet** sommets;
    adj_data* adj;
};



////////////////////////////////////////////////////////////////////////////////////
///////////////////   implementation de la file de priorite du A*




struct pqueue {

    size_t size;
    size_t max_size;
    size_t* data;

private:
    inline static size_t parent(const size_t x) {
        return (x-1)/2;
    }
    inline static size_t gauche(const size_t x) {
        return 2*x + 1;
    }
    inline static size_t droite(const size_t x) {
        return 2*x + 2;
    }

public:

    static pqueue create(const size_t n) {
        return (pqueue) {
            .size = 0,
            .max_size = n,
            .data = (size_t*) malloc(n * sizeof(size_t))
        };
    }
    static void destroy(const pqueue* queue) {
        free(queue->data);
        return;
    }

    void add(const graphe* g, size_t s)
    {
        if (size >= max_size)
        {
            max_size *= 2;
            size_t* new_data = (size_t*) malloc(max_size * sizeof(size_t));
            
            for (size_t i=0; i<size; i++) {
                new_data[i] = data[i];
            }

            free(data);
            data = new_data;
        }
        size += 1;

        size_t idx = size-1;
        data[idx] = s;

        while(idx > 0)
        {
            if (g->sommets[data[idx]]->heuristique >= 
                g->sommets[data[parent(idx)]]->heuristique) 
            {
                break;
            }

            s = data[idx];
            data[idx] = data[parent(idx)];
            data[parent(idx)] = s;

            idx = parent(idx);
        }
    }

    bool has(size_t x) {
        for (size_t i=0; i<size; i++) {
            if (data[i] == x) {
                return true;
            }
        }
        return false;
    }

    size_t get(const graphe* g)
    {
        size_t res = data[0];

        size -= 1;
        data[0] = data[size];

        size_t idx = 0;
        size_t temp;
        
        while(gauche(idx) < size)
        {
            //cas particulier, un seul fils disponible
            if (droite(idx) >= size)
            {
                if (
                    g->sommets[data[idx]]->heuristique > 
                    g->sommets[data[gauche(idx)]]->heuristique)
                {
                    temp = data[idx];
                    data[idx] = data[gauche(idx)];
                    data[gauche(idx)] = data[idx];
                }
                break;
            }

            if (g->sommets[data[droite(idx)]]->heuristique < 
                g->sommets[data[gauche(idx)]]->heuristique)
            {
                //echange a droite si necessaire
                if (g->sommets[data[idx]]->heuristique < 
                    g->sommets[data[droite(idx)]]->heuristique)
                {
                    break;
                }
                temp = data[idx];
                data[idx] = data[droite(idx)];
                data[droite(idx)] = temp;

                idx = droite(idx);
                continue;
            }
            else
            {
                //echange a gauche si necessaire
                if (g->sommets[data[idx]]->heuristique < 
                    g->sommets[data[gauche(idx)]]->heuristique)
                {
                    break;
                }
                temp = data[idx];
                data[idx] = data[gauche(idx)];
                data[gauche(idx)] = temp;

                idx = gauche(idx);
                continue;
            }
        }

        return res;
    }
};


////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////  implementation principale



double h(graphe* g, size_t a, size_t b)
{
    double dx = g->sommets[a]->x - g->sommets[b]->x;
    double dy = g->sommets[a]->y - g->sommets[b]->y;
    return dx*dx + dy*dy;
}


void astar(graphe* g, size_t start, size_t goal)
{
    pqueue open = pqueue::create(10);
    std::map<size_t,size_t> from = std::map<size_t,size_t>();
    std::map<size_t, double> best_to = std::map<size_t,double>();
    std::map<size_t, double> best_with = std::map<size_t,double>();

    open.add(g, start);
    best_to[start] = 0;
    best_with[start] = h(g, start, goal);

    while (open.size > 0)
    {
        size_t cur  = open.get(g);

        if (cur == goal) {
            //found
        }

        for(size_t i=0; i<g->adj[cur].arite; i++)
        {
            double try_best = best_to[cur] + g->adj[cur].dist[i];
            size_t v = g->adj[cur].voisins[i];

            if (try_best < best_to[v])
            {
                from[v] = cur;
                best_to[v] = try_best;
                best_with[v] = try_best + h(g, v, goal);

                if (!open.has(v)) {
                    open.add(g, v);
                }
            }
        }
    }

}


















////////////////////////////////////////////////////////////////////
////////////////// contructeur du graphe


graphe* graphe_create()
{
    FILE* file = fopen("../rues-graphe.gph", "r");
    char buffer[100];
    graphe* g = (graphe*) malloc(sizeof(graphe));

    if (fgets(buffer, 100, file) == NULL) {
        printf("err 1\n");
        exit(1);
    }

    g->n = strtoull(buffer, NULL, 10);

    if (g->n == 0) {
        printf("err 2\n");
        exit(1);
    }
    
    g->sommets = (sommet**) malloc(g->n * sizeof(sommet*));
    g->adj = (adj_data*) malloc(g->n * sizeof(adj_data));

    for (size_t i=0; i<g->n; i++)
    {
        char* ptr = buffer;
        fgets(buffer, 100, file);

        g->sommets[i]->id = i;
        g->sommets[i]->x = strtod(buffer, &ptr);
        g->sommets[i]->y = strtod(ptr+1,  NULL);
    }

    for (size_t i=0; i<g->n; i++)
    {
        fgets(buffer, 100, file);

        g->adj[i].arite = strtoull(buffer, NULL, 10);

        if (g->adj[i].arite == 0) {
            printf("err 3");
            exit(1);
        }

        g->adj[i].dist = (double*) malloc(g->adj[i].arite * sizeof(double));
        g->adj[i].voisins = (size_t*) malloc(g->adj[i].arite * sizeof(size_t));

        for (size_t j=0; j<g->adj[i].arite; j++)
        {
            fgets(buffer, 100, file);

            char* ptr = buffer;
            char dbuff[23];

            for (int i=0; i<22; i++) {
                dbuff[i] = buffer[i];
            }
            dbuff[22] = '\0';

            g->adj[i].dist[j] = strtod(dbuff, NULL);

            double dx = strtod(buffer+22, &ptr);
            double dy = strtod(ptr+1, NULL);

            bool found = false;
            for (size_t k=0; k<g->n; k++) {
                if (g->sommets[i]->x == dx && g->sommets[i]->y == dy) {
                    g->adj[i].voisins[j] = k;
                    found = true;
                    break;
                }
            }
            if (!found) {
                printf("err 4");
                exit(1);
            }
        }
    }

    return g;
}



/***************************************************************/
// test


int main(void)
{
    graphe* g = graphe_create();
}