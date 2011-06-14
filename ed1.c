#include "malloc.h"
#include "stdlib.h"
#include "stdio.h"

typedef struct _arvore arvore;
struct _arvore {
    arvore* dir;
    arvore* esq;
    int dado;
    int bal;
};

typedef struct _lista lista;
struct _lista {
    lista* prox;
    int dado;
};



void desocupar_arvore(arvore* t){
    if (t != NULL){
        desocupar_arvore(t->dir);
        desocupar_arvore(t->esq);
        free(t);
    }
}

void desocupar_lista(lista* l) {
    if (l != NULL) {
        desocupar_lista(l->prox);
        free(l);
    }
}

void novo_no_arvore(arvore** t, int valor){
    *t = malloc(sizeof(arvore));
    (*t)->dado = valor;
    (*t)->esq = NULL; (*t)->dir = NULL; 
    (*t)->bal = 0;
}

void novo_no_lista(lista** l, int dado)
{
    *l = malloc(sizeof(lista));
    (*l)->dado = dado;
    (*l)->prox = NULL;
}

void append(lista** l, int dado)
{
    lista* novo;
    novo_no_lista(&novo, dado);
    novo->prox = (*l);
    (*l) = novo;
}

void to_list(arvore *t, lista** l)
{
    if (t != NULL){
        to_list(t->esq, l);
        append(l, t->dado);
        to_list(t->dir, l);
    }
}

int busca(arvore** t, arvore** pai, int elemento){
    arvore** temp;    
    if (*t == NULL)
        return 0;
    else if ((*t)->dado == elemento)
        return 1;
    else {
        if ((*t)->dado > elemento) {
            if ((*t)->esq == NULL)
                return 2;
            else {
                *pai = *t;
                *t = (*t)->esq;
            }           
        } else {
            if ((*t)->dir == NULL)
                return 3;
            else {
                *pai = *t;
                *t = (*t)->dir;
            } 
        }
        return busca(t, pai, elemento); 
    }

}


void caso1(arvore** t){
    arvore* ptu;
    arvore* ptv;
    ptu = (*t)->esq;
    if (ptu->bal == -1){
        (*t)->esq = ptu->dir; 
        ptu->dir = (*t);
        (*t)->bal = 0; (*t) = ptu;
    } else {
        ptv = ptu->dir;
        ptu->dir = ptv->esq; 
        ptv->esq = ptu;
        (*t)->esq = ptv->dir; 
        ptv->dir = (*t);
        
        (*t)->bal = (ptv->bal == -1)? 1: 0;
        ptu->bal = (ptv->bal == 1)? -1: 0;

        (*t) = ptv;
    }
    (*t)->bal = 0;
}

void caso2(arvore** t){
    arvore* ptu;
    arvore* ptv;
    ptu = (*t)->dir;
    if (ptu->bal == 1){
        (*t)->dir = ptu->esq; 
        ptu->esq = (*t);
        (*t)->bal = 0; (*t) = ptu;
    } else {
        ptv = ptu->esq;
        ptu->esq = ptv->dir; 
        ptv->dir = ptu;
        (*t)->dir = ptv->esq; 
        ptv->esq = (*t);
        
        (*t)->bal = (ptv->bal == 1)? -1: 0;
        ptu->bal = (ptv->bal == -1)? 1: 0;

        (*t) = ptv;
    }
    (*t)->bal = 0;
}


int inserir(arvore** t, int valor, int texto){ 
    if (*t == NULL){    
        novo_no_arvore(t, valor);
        return 1;
    }else if ((*t)->dado == valor){
        if (texto) {
            printf("Já existe este elemento na arvore!");
        }
        return 0;
    }else if ((*t)->dado > valor){
        if (inserir(&((*t)->esq), valor, texto)){
            switch ((*t)->bal){
                case 0: 
                    (*t)->bal = -1;
                    return 1;  
                case 1: 
                    (*t)->bal = 0;
                    return 0;  
                case -1: 
                    caso1(t);
                    return 0;
            }
        }
    }else if ((*t)->dado < valor){
        if (inserir(&((*t)->dir), valor, texto)){
            switch ((*t)->bal){
                case 0: 
                    (*t)->bal = 1;
                    return 1;  
                case -1: 
                    (*t)->bal = 0;
                    return 0;  
                case 1: 
                    caso2(t);
                    return 0;
            }
        } 
    }
}


void remove_elemento(arvore** t, int elemento, int* existe)
{
    lista *l = NULL; 
    int i = 0;

    if (*t == NULL){
        *existe = 0;        
        return;
    }

    to_list(*t, &l);
    desocupar_arvore(*t);
    *t = NULL;

    lista *temp = l;
 
    while(temp != NULL) {
        
        if (temp->dado != elemento)
            inserir(t, temp->dado, 0);
        temp = temp->prox;
    }

    desocupar_lista(l);
}

void uniao(arvore *t1, arvore *t2, arvore **u)
{
    lista *l1 = NULL;
    lista *l2 = NULL;

    to_list(t1, &l1);
    to_list(t2, &l2);
    
    
    lista *temp = l1;
    while(temp != NULL) {
        inserir(u, temp->dado, 0);
        temp = temp->prox;
    }

    temp = l2;
    while(temp != NULL) {
        inserir(u, temp->dado, 0);
        temp = temp->prox;
    }
    
}

int pertence(lista *l, int elem)
{
    while (l != NULL) {
        if (l->dado == elem)
            return 1;
    }
    return 0;
}

void intersecao(arvore *t1, arvore *t2, arvore **in)
{
    lista *l1 = NULL;
    lista *l2 = NULL;
    lista *in = NULL;

    to_list(t1, &l1);
    to_list(t2, &l2);

    lista *temp = l1;
    while(temp != NULL) {
        if (pertence(l2, temp->dado))
            inserir(in, temp->dado, 0);
        temp = temp->prox;
    }
}

void subtraca(arvore *t1, arvore *t2, arvore **s)
{
    
}





































