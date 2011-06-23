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


void desocupar_arvore(arvore* t){
    if (t != NULL){
        desocupar_arvore(t->dir);
        desocupar_arvore(t->esq);
        free(t);
    }
}

void clonar(arvore* t, arvore** clone) {
	if (t != NULL) {
		inserir(&clone, t->dado, 1);
		if ((t->esq) != NULL)
			clonar(t->esq, clone);
		if ((t->dir) != NULL)
			clonar(t->dir, clone);
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

int balancear_esq(arvore** t){
    int* balanco = &(*t)->bal;
    switch (*balanco){
        case 0: 
            *balanco = -1;
            return 1;  
        case 1: 
            *balanco = 0;
            return 0;  
        case -1: 
            rotacao_esquerda(t);
            return 0;
    }
} 

int balancear_dir(arvore** t){
    int* balanco = &(*t)->bal;
    switch (*balanco){
        case 0: 
            *balanco  = 1;
            return 1;  
        case -1: 
            *balanco = 0;
            return 0;  
        case 1: 
            rotacao_direita(t);
            return 0;
    }
} 

void novo_no_arvore(arvore** t, int valor){
    *t = malloc(sizeof(arvore));
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
            return balancear_esq(t);
    }else if ((*t)->dado < valor){
        if (inserir(&((*t)->dir), valor, texto))
            return balancear_dir(t);
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
            balancear_dir(&pai); 
        }        
        balancear_esq(t);
        
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
            balancear_dir(t);
            return 1;
        }
    } else {// if ((*t)->dado < valor){
        if (remove_elemento(&((*t)->dir), valor)){
            balancear_esq(t);
            return 1;
        } 
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
	printf("entrou\n");
    arvore* pai;
    arvore* aux;
    printf("lol1\n");
    if (outra != NULL){
    	printf("outra não é null\n");
        aux = *t;
        if (busca(&aux, &pai, outra->dado) == 1)
            remove_elemento(t, outra->dado);
        subtracao_recursiva(t, outra->esq);
        subtracao_recursiva(t, outra->dir);
    }
    printf("lol2\n");
}

void subtracao(arvore* t1, arvore* t2, arvore** resultado) {
	clonar(t1, resultado);
	subtracao_recursiva(resultado, t2);
	if ((*resultado) != NULL) {
		printf("%d\n", (*resultado)->dado);
		printf("wtf");
	}
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


