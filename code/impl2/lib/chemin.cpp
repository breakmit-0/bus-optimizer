#include <stdlib.h>
#include <stdio.h>
#include "./exn.h"


#include "chemin.h"

void list::rec_free() {
    if (this->suiv != NULL) {
        this->suiv->rec_free();
    }
    free(this);
}

chemin chemin::new_chemin(size_t len, size_t* sommets)
{
    chemin res = {
        .nSommets = len, 
        .head = NULL,
        .tail = NULL
    };

    if (len == 0) {
        return res;
    }

    res.head = (list*)malloc(sizeof(list));
    res.head->sommet = sommets[0];
    res.head->prec = NULL;
    res.head->suiv = NULL;
    res.tail = res.head;
    

    list* cur = res.head;
    for (size_t i=1; i<len; i++)
    {

        res.tail = (list*)malloc(sizeof(list));
        res.tail->sommet = sommets[i];
        res.tail->suiv = NULL;
        res.tail->prec = cur;
        
        cur->suiv = res.tail;
        cur = res.tail;
    }
    return res;
}

void chemin::free_chemin(chemin c) {
    if (c.head != NULL) {
        c.head->rec_free();
    }
}

void chemin::rm_head() {
    if (nSommets == 0) {
        return;
    }
    if (nSommets == 1) {
        free(head);
        this->head = NULL;
        this->tail = NULL;
        this->nSommets = 0;
        return;
    }

    nSommets -= 1;

    list* mem = head;
    head = head->suiv;
    head->prec = NULL;

    free(mem);
    return;
}

void chemin::rm_tail() {
    if (nSommets == 0) {
        return;
    }
    if (nSommets == 1) {
        free(head);
        this->head = NULL;
        this->tail = NULL;
        this->nSommets = 0;
        return;
    }

    nSommets -= 1;

    list* mem = tail;
    tail = tail->prec;
    tail->suiv = NULL;

    free(mem);
    return;
}

void chemin::add_head(size_t s) {
    this->nSommets += 1;
    list* mem = (list*) malloc(sizeof(list));
    
    mem->sommet = s;
    mem->prec = NULL;
    mem->suiv = this->head;
    
    this->head->prec = mem;
    this->head = mem;
}

void chemin::add_tail(size_t s) {
    this->nSommets += 1;
    list* mem = (list*) malloc(sizeof(list));
    
    mem->sommet = s;
    mem->prec = this->tail;
    mem->suiv = NULL;

    this->tail->suiv = mem;
    this->tail = mem;
}

void chemin::insere_hd(const chemin _ch, size_t idx)
{    
    chemin ch = _ch.copy();


    if (ch.nSommets == 0) {}

    else if (this->nSommets == 0)
    {
        this->head = ch.head;
        this->tail = ch.tail;
        this->nSommets = ch.nSommets;
    }

    else if (idx == 0)
    {
        list* old_head = this->head;

        //novelle hd
        this->head = ch.head;
        
        //connexion
        ch.tail->suiv = old_head;
        old_head->prec = ch.tail;

        this->nSommets += ch.nSommets;
    }
    
    else {
        list* cur = this->head;

        while (idx-1 > 0)
        {
            if (cur == NULL) {
                TRACE();
                FAIL(1, "Error: chemin::insere_hd, index out of bounds (last trace)\n");
            }

            cur = cur->suiv;
            idx -= 1;
        }

        list* next = cur->suiv;

        next->prec = ch.tail;
        cur->suiv = ch.head;

        ch.head->prec = cur;
        ch.tail->suiv = next;

        this->nSommets += ch.nSommets;
    }
}

void chemin::insere_tl(const chemin _ch, size_t idx)
{    
    chemin ch = _ch.copy();

    if (ch.nSommets == 0) {}

    else if (this->nSommets == 0)
    {
        this->head = ch.head;
        this->tail = ch.tail;
        this->nSommets = ch.nSommets;
    }

    else if (idx == 0)
    {
        list* t = this->tail;
        this->tail = ch.tail;
        ch.head->prec = t;
        t->suiv = ch.head;
        this->nSommets += ch.nSommets;
    }
    
    else {
        list* cur = this->tail;

        while (idx-1 > 0)
        {
            if (cur == NULL) {
                TRACE();
                FAIL(1, "Error: chemin::insere_tl, index out of bounds (last trace)\n");
            }

            cur = cur->prec;
            idx -= 1;
        }

        list* pre = cur->prec;

        pre->suiv = ch.head;
        cur->prec = ch.tail;

        ch.head->prec = pre;
        ch.tail->suiv = cur;

        this->nSommets += ch.nSommets;
    }
}


chemin chemin::copy() const
{
    chemin res;
    res.nSommets = this->nSommets;

    if (res.nSommets == 0) {
        res.head = NULL;
        res.tail = NULL;
        return res;
    }


    list* cur = (list*)malloc(sizeof(list));
    list* read = this->head;
    list* next = cur;
    res.head = cur;   

    cur->sommet = read->sommet;
    cur->prec = NULL;
    cur->suiv = NULL;

    while(read->suiv != NULL)
    {
        read = read->suiv;        
        next = (list*)malloc(sizeof(list));
        
        next->sommet = read->sommet;
        next->prec = cur;
        cur->suiv = next;

        cur = next;
    }

    res.tail = next;
    res.tail->suiv = NULL;

    return res;
}


void chemin::print(const char* term) const {

    list* cur = this->head;

    printf("[ ");
    
    while (cur != NULL)
    {
        printf("%llu ", cur->sommet);
        cur = cur->suiv;
    }
    printf("]%s", term);
}

void chemin::print_dbg(const char* term) const {

    list* cur = this->head;

    printf("[ ");
    
    while (cur != NULL)
    {
        printf("%llX ", (long long unsigned)cur & 0xFFFF);
        cur = cur->suiv;
    }
    printf("]%s", term);    
}







bool chemin::check() const
{
    if (head == NULL || tail == NULL) {
        if (!(tail == NULL && head == NULL && nSommets == 0))
        {
            printf("mauvais pointeurs nuls: (hd nul ? %i), (tl nul ? %i), (taille : %llu)\n", head==NULL, tail==NULL, nSommets);
            return false;
        }
        return true;
    }

    if (head->prec != NULL || tail->suiv != NULL) {
        printf("fins de listes invalides: (hd est fin ? %i), (tl est fin ? %i)\n", head->prec==NULL, tail->suiv==NULL);
        return false;
    }

    size_t n1 = 1;
    size_t n2 = 1;
    list* target_tl = head;
    list* target_hd = tail;

    while (target_tl->suiv!=NULL) {
        n1 += 1;
        target_tl = target_tl->suiv;
    }
    while (target_hd->prec!=NULL) {
        n2 += 1;
        target_hd = target_hd->prec;
    }

    if (! ((n1 == nSommets) 
        && (n2 == nSommets) 
        && (target_hd == head)
        && (target_tl == tail)
    )) {
        printf("erreur de construction: (obj : %llu), (-> : %llu), (<- : %llu), (hd ok ? %i) (tl ok ? %i)\n",
            nSommets, n1, n2, target_hd==head, target_tl==tail);
        return false;
    }
    return true;
}