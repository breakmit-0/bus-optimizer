/*
*@:valerie.le_blanc@ac-strasbourg.fr
*@:mp2i.2021@besson.link
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <random>

struct arete {
    arete* next;
    double dist;
    int dest;

    static arete* add(arete** cur, int dest, double dist)
    {
        arete* n = (arete*) malloc(sizeof(arete));
        n->dist = dist;
        n->dest = dest;
        n->next = NULL;
        
        if (*cur == NULL) {
            *cur = n;
        }
        else {
            (*cur)->next = n;
        }
        
        return n;
    }

    void del() {

        if (this == NULL) {
            return;
        }

        if (this->next != NULL) {
            this->next->del();
        }
        free(this);
    }

    bool has(int sommet)
    {
        if (this == NULL)  {
            return false;
        }

        if (this->dest == sommet) {
            return true;
        }
        
        return this->next->has(sommet);
    }
};


struct graphe
{
    int n;
    arete** liste;

    graphe(int sommets)
    {
        this->n = sommets;
        liste = (arete**) malloc(n * sizeof(arete*));

        for (int i=0; i<n; i++) {
            liste[i] = NULL;
        }
    }

    void del() {
        for (int i=0; i<n; i++) {
            liste[i]->del();
        }
        free(liste);
    }

    arete*& operator[](int k) {
        return liste[k];
    }
};


graphe gc(int n)
{
    graphe g = graphe(n * n);
    arete** cur;

    for (int i=0; i<g.n; i++)
    {
        cur = g.liste + i;

        if (i / n != 0) {
            arete::add(cur, i-n, 1.0);
            cur = &((*cur)->next);
        }

        if (i / n != n-1) {
            arete::add(cur, i+n, 1.0);
            cur = &((*cur)->next);
        }

        if (i % n != 0) {
            arete::add(cur, i-1, 1.0);
            cur = &((*cur)->next);
        }

        if (i % n != n-1) {
            arete::add(cur, i+1, 1.0);
            cur = &((*cur)->next);
        }
    }
    return g;
}

struct chemin
{
    chemin* next;
    int sommet;

    static chemin* construit(int* sommets, int n)
    {
        chemin* cur = (chemin*) malloc(sizeof(chemin));
        chemin* c0 = cur;
        cur->sommet = sommets[0];
        cur->next = NULL;

        for (int i=1; i<n; i++)
        {
            cur->next = (chemin*) malloc(sizeof(chemin));
            cur = cur->next;
            cur->sommet = sommets[i];
            cur->next = NULL;            
        }

        return c0;
    }

    void del() {
        if (this->next != NULL) {
            this->next->del();
        }
        free(this);
    }

    bool est_valide(graphe g)
    {
        chemin* ch = this;

        if (ch == NULL) {
            return true;
        }

        while(ch->next != NULL)
        {
            if (g[ch->sommet]->has(ch->next->sommet)) {
                ch = ch->next;
            }
            else {
                return false;
            }
        }

        return true;
    }
};


chemin* clean_chemin(chemin* ch)
{
    chemin* chOrg = ch;
    if (ch == NULL) {return NULL;}
    if (ch->next == NULL) {return chOrg;}
    
    while(ch->next->next != NULL)
    {
        if (ch->sommet == ch->next->next->sommet)
        {
            chemin* next = ch->next->next->next;
            free(ch->next->next);
            free(ch->next);
            ch->next = next;

            if (ch->next == NULL) {break;}
            if (ch->next->next == NULL) {break;}
        }
        ch = ch->next;
    }
    return chOrg;
}


chemin* bouge_terminus(chemin* ch, graphe g)
{
    chemin* chOrg = ch;
    int dir = std::rand() % 4;
    int term = std::rand() % 2;

    //a la fin
    if (term == 1) {
        while(ch->next != NULL) {
            ch = ch->next;
        }
    }
    
    
    arete* voisins = g[ch->sommet];
    while (dir != 0) {
        if (voisins->next != NULL) {
            voisins = voisins->next;
        }
        else {
            voisins = g[ch->sommet];
        }
        dir -= 1;
    }
    chemin* nch = (chemin*) malloc(sizeof(chemin));
    nch->sommet = voisins->dest;
    
    if (term == 0) {
        //au debut
        nch->next = ch;
        return clean_chemin(nch);
    }
    else {
        //a la fin
        ch->next = nch;
        nch->next = NULL;
        return chOrg; //clean_chemin(chOrg);
    }
}


void print_chemin(chemin* ch)
{
    while (ch != NULL) {
        printf("%i, ", ch->sommet);
        ch = ch->next;
    }
    printf("\n");
}


int dist(int x1, int y1, int x2, int y2)
{
    return (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1);
}

#define min(x, y) (x < y ? x : y)

int cout(chemin* ch, graphe g)
{
    int x1 = 1; int y1 = 1;
    int x2 = 7; int y2 = 3;
    int x3 = 9; int y3 = 6;

    int k_len = 20;
    int k_dist = 20;

    int c_len = 0;
    int c_d1 = INT_MAX;
    int c_d2 = INT_MAX;
    int c_d3 = INT_MAX;

    while(ch != NULL) 
    {
        c_len += 1;
        int x = ch->sommet % 10;
        int y = ch->sommet / 10;
        c_d1 = min(c_d1, dist(x, y, x1, y1));
        c_d2 = min(c_d2, dist(x, y, x2, y2));
        c_d3 = min(c_d3, dist(x, y, x3, y3));

        ch = ch->next;
    }
    return c_len * k_len + (c_d1 + c_d2 + c_d3) * k_dist;
}

chemin* cpy_chemin(chemin* ch) {
    chemin* res = NULL;
    chemin* prev = NULL;
    chemin* nouv = NULL;

    if (ch == NULL) {
        return NULL;
    }
    res = (chemin*) malloc(sizeof(chemin));
    res->sommet = ch->sommet;
    res->next = NULL;
    prev = res;
    ch = ch->next;

    while (ch != NULL) {
        nouv = (chemin*) malloc(sizeof(chemin));
        nouv->sommet = ch->sommet;
        nouv->next = ch->next;
        prev->next = nouv;
        ch = ch->next;
        prev = nouv;
    }

    return res;
}

void affiche_chemin(chemin* ch)
{
    char* aff = (char*) malloc((10 * (10 + 1) + 1) * sizeof(char));

    for (int y=0; y<10; y++) {
        for (int x=0; x<10; x++) {
            aff[11*y + x] = ' ';
        }
        aff[11*y + 10] = '\n';
    }

    while (ch != NULL) {
        int x = ch->sommet % 10;
        int y = ch->sommet / 10;

        aff[11*y + x] = 'O';
        ch = ch->next;
    }

    int p1 = 1*11 + 1;
    int p2 = 3*11 + 7;
    int p3 = 6*11 + 9;

    aff[p1] = aff[p1] == 'O' ? '@' : 'a';
    aff[p2] = aff[p2] == 'O' ? '@' : 'a';
    aff[p3] = aff[p3] == 'O' ? '@' : 'a';

    aff[9*11 + 10] = '\0';

    printf("\n%s\n", aff);
}


int main() {
    time_t t;
    std::srand((unsigned) time(&t));
    
    graphe g1 = gc(10);

    int ch1[2] = {60, 61};
    chemin* chemin1 = chemin::construit(ch1, 2);

    int cout1 = cout(chemin1, g1);

    for (int i=0; i<1000; i++)
    {
        chemin* chemin2 = cpy_chemin(chemin1);
        chemin2 = bouge_terminus(chemin2, g1);
        int cout2 = cout(chemin2, g1);

        if (cout2 <= cout1 || ((rand() % 500) / 10 > i)) {
            chemin1->del();
            chemin1 = chemin2;
            cout1 = cout2;
        }
        else {
            chemin2->del();
        }

        printf("%i |", cout1);
        print_chemin(chemin1);
    }

    if (chemin1->est_valide(g1)) {
        printf("succes!\n");
    }

    affiche_chemin(chemin1);

    g1.del();
    chemin1->del();

    return 0;
}


