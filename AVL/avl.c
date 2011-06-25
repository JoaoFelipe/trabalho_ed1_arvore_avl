#include "malloc.h"
#include "stdlib.h"
#include "stdio.h"
#include "avl.h"
#include "fila.h"



void desocupar_arvore(arvore* t){
    if (t != NULL){
        desocupar_arvore(t->dir);
        desocupar_arvore(t->esq);
        free(t);
    }
}

int busca(arvore** t, arvore** pai, int elemento){
    arvore** temp;
    if (*t == NULL)
        return 0;
    else{
        if ((*t)->dado == elemento)
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
}


void rotacao_esquerda(arvore** t){
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

void rotacao_direita(arvore** t){
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

int balancear_esq(arvore** t, int ins){
    int* balanco = &(*t)->bal;
    int retorno;
    switch (*balanco){
        case 0:
            *balanco = -1;
            return ins? 1: 0;
        case 1:
            *balanco = 0;
            return ins? 0: 1;
        case -1:
            retorno = ((!ins) && ((*t)->esq->bal != 0))? 1: 0;
            rotacao_esquerda(t);
            return retorno;


    }
}

int balancear_dir(arvore** t, int ins){
    int* balanco = &(*t)->bal;
    int retorno;
    switch (*balanco){
        case 0:
            *balanco  = 1;
            return ins? 1: 0;
        case -1:
            *balanco = 0;
            return ins? 0: 1;
        case 1:
            retorno = ((!ins) && ((*t)->dir->bal != 0))? 1: 0;
            rotacao_direita(t);
            return retorno;
    }
}

void novo_no_arvore(arvore** t, int valor){
    *t = (arvore*)malloc(sizeof(arvore));
    (*t)->dado = valor;
    (*t)->esq = NULL; (*t)->dir = NULL;
    (*t)->bal = 0;
}

int inserir(arvore** t, int valor, int texto){
    if (*t == NULL){
        novo_no_arvore(t, valor);
        return 1;
    }else if ((*t)->dado == valor){
        if (texto)
            printf("Já existe o elemento %d elemento na árvore!\n", valor);
        return 0;
    }else if ((*t)->dado > valor){
        if (inserir(&((*t)->esq), valor, texto))
            return balancear_esq(t, 1);
    }else if ((*t)->dado < valor){
        if (inserir(&((*t)->dir), valor, texto))
            return balancear_dir(t, 1);
    }
}

int remove_simples(arvore** t){
   if ((*t)->esq == NULL && (*t)->dir == NULL){
        free(*t);
        *t = NULL;
    } else if ((*t)->dir == NULL)
        *t = (*t)->esq;
    else if ((*t)->esq == NULL)
        *t = (*t)->dir;
    else {
        arvore* pai = NULL;
        arvore* filho = (*t)->dir;
        while (filho->esq != NULL){
            pai = filho;
            filho = filho->esq;
        }
        (*t)->dado = filho->dado;
        if (pai == NULL)
            (*t)->dir = filho->dir;
        else{
            pai->esq = filho->dir;
            balancear_dir(&pai, 0);
        }
        balancear_esq(t, 0);

        free(filho);
    }
}

int remove_elemento(arvore** t, int valor){
    if (*t == NULL){
        return 0;
    } else if ((*t)->dado == valor){
         remove_simples(t);
         return 1;
    } else if ((*t)->dado > valor){
        if (remove_elemento(&((*t)->esq), valor)){
            return balancear_dir(t, 0);
        }
    } else {// if ((*t)->dado < valor){
        if (remove_elemento(&((*t)->dir), valor)){
            return balancear_esq(t, 0);
        }
    }
}

void clonar(arvore* t, arvore** clone) {
	*clone = NULL;
	if (t != NULL) {
		Fila* fila = NULL;
		fila = (Fila*)malloc(sizeof(Fila));
		fila->inicio = NULL;
		fila->fim = NULL;
		push(fila, t);
		arvore* atual = NULL;
		do {
			atual = NULL;
			pop(fila, &atual);
			if (atual != NULL) {
				if (atual->esq != NULL)
					push(fila, atual->esq);
				if (atual->dir != NULL)
					push(fila, atual->dir);
				inserir(clone, atual->dado, 0);
			}
		} while (atual != NULL);
		free(fila);
		return;
	}
}

void uniao_recursiva(arvore** t, arvore* outra){
    if (outra != NULL){
        inserir(t, outra->dado, 0);
        uniao_recursiva(t, outra->esq);
        uniao_recursiva(t, outra->dir);
    }
}

void uniao(arvore* t1, arvore* t2, arvore** resultado) {
	clonar(t1, resultado);
	uniao_recursiva(resultado, t2);
}

void subtracao_recursiva(arvore** t, arvore* outra){
    arvore* pai;
    arvore* aux;
    if (outra != NULL){
        aux = *t;
        if (busca(&aux, &pai, outra->dado) == 1)
            remove_elemento(t, outra->dado);
        subtracao_recursiva(t, outra->esq);
        subtracao_recursiva(t, outra->dir);
    }
}

void subtracao(arvore* t1, arvore* t2, arvore** resultado) {
	clonar(t1, resultado);
	subtracao_recursiva(resultado, t2);
}

void intersecao_recursiva(arvore** t, arvore* outra){
    arvore* pai;
    arvore* aux;
    if (*t != NULL){
        intersecao_recursiva(&(*t)->esq, outra);
        intersecao_recursiva(&(*t)->dir, outra);
        aux = outra;
        if (busca(&aux, &pai, (*t)->dado) != 1)
            remove_elemento(t, (*t)->dado);
    }
}

void intersecao(arvore* t1, arvore* t2, arvore** resultado) {
	clonar(t1, resultado);
	intersecao_recursiva(resultado, t2);
}

int pegar_maior_altura(arvore* t, int atual) {
	int alt_esq = 0;
	int alt_dir = 0;
	if (t->esq != NULL)
		alt_esq = pegar_maior_altura(t->esq, atual + 1);
	if (t->dir != NULL)
		alt_dir = pegar_maior_altura(t->dir, atual + 1);
	int altura = alt_esq > alt_dir ? alt_esq : alt_dir;
	return altura > atual ? altura : atual;
}


