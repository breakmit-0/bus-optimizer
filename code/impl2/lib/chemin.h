#ifndef INCLUDE_CHEMIN_H
#define INCLUDE_CHEMIN_H

#include "prim.h"

struct list {
public:
    size_t sommet;
    list* suiv;
    list* prec;

    void rec_free();
};

struct chemin {
public:
    size_t nSommets;
    list* head;
    list* tail;

    static chemin new_chemin(size_t, size_t*);
    static void free_chemin(chemin);

    void rm_head();
    void rm_tail();

    void add_head(size_t);
    void add_tail(size_t);

    void insere_hd(const chemin, size_t);
    void insere_tl(const chemin, size_t);

    chemin copy() const;
    
    void print(const char*) const;
    void print_dbg(const char*) const;
    bool check() const;
};

#endif