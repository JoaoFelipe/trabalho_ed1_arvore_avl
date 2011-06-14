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
/*
void imprime_lista(int* list, int tamanho)
{
    int i;
    for (i = 0; i < tamanho; i++){
        printf("%i ", list[i]);
    }

}

void append(int* list, int dado)
{
    int i = 0;
    while (list[i] != -1){
        i++;
    }
    list[i] = dado;
}

void to_list(arvore *t, int* list)
{
    if (t->esq != NULL){
        to_list(t->esq, list);
    } 
    append(list, t->dado);
    if (t->dir != NULL){
        to_list(t->dir, list);
    } 
    free(t);
}

void remove_elemento(arvore** t, int elemento, int* existe)
{
    int *lista;
    int tamanho;
    int i = 0;

    if (*t == NULL){
        *existe = 0;        
        return;
    }
    
    tamanho = count(*t);
    lista = malloc(sizeof(int)*tamanho);
    for (i = 0; i<tamanho; i++){
        lista[i] = -1;
    }
    to_list(*t, lista);
    
    *t = NULL;

    *existe = 0;
    for (i = 0; i < tamanho; i++){
        if (lista[i] != elemento){
//            incluir(t, elemento);
        }else{
            *existe = 1;
        }
    }

    

    free(lista);
} 


int count(arvore *t)
{
    if (t == NULL)
        return 0;
    return count(t->esq) + count(t->dir) + 1;
}


int ed1(void)
{
    return 1;
}
*/

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

int inserir_simples(arvore** t, int elemento){ 
    arvore* temp = *t;
    arvore* pai = NULL;
    arvore* novo;
    int encontrou = busca(&temp, &pai, elemento);
    
    novo = malloc(sizeof(arvore));
    novo->esq = NULL; novo->dir = NULL; novo->dado = elemento;

    if (encontrou == 0)
        *t = novo;
    else if (encontrou == 1){
        printf("Já existe este elemento na arvore!");
        free(novo);
    } else if (encontrou == 2)
        temp->esq = novo;
    else //if (encontrou == 3)
        temp->dir = novo; 
    return (encontrou != 1);
}


void novo_no(arvore** t, int valor){
    *t = malloc(sizeof(arvore));
    (*t)->dado = valor;
    (*t)->esq = NULL; (*t)->dir = NULL; 
    (*t)->bal = 0;
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


int inserir(arvore** t, int valor){ 
    if (*t == NULL){    
        novo_no(t, valor);
        return 1;
    }else if ((*t)->dado == valor){
        printf("Já existe este elemento na arvore!");
        return 0;
    }else if ((*t)->dado > valor){
        if (inserir(&((*t)->esq), valor)){
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
        if (inserir(&((*t)->dir), valor)){
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

void desocupar(arvore* t){
    if (t != NULL){
        desocupar(t->dir);
        desocupar(t->esq);
        free(t);
    }
}


//    lista[0] = t;
//    while (i<tamanho){
//        if (lista[i]->esq != NULL){
//            lista[i+1] = lista[i]->esq;
//            if (lista[i]->dir != NULL){
//                lista[i+2] = lista[i]->dir;
//            }
//        }else if (lista[i]->dir != NULL){
//            lista[i+1] = lista[i]->dir;
//            if (lista[i]->esq != NULL){
//                lista[i+2] = lista[i]->esq;
//            }
//        }
//        i++;
//
//    }






































