#include <stdio.h>
#include <stdlib.h>

typedef struct _arvore arvore;
struct _arvore {
    arvore* dir;
    arvore* esq;
    int dado;
    int bal;
};

typedef struct no {
	arvore* dado;
	struct no *prox;
} Elemento;

typedef struct {
	Elemento* inicio;
	Elemento* fim;
} Fila;

void push(Fila** fila, arvore* elem) {
	if ((*fila) == NULL) {
		(*fila) = (Fila*)malloc(sizeof(Fila));
	}
	Elemento* novo = (Elemento*)malloc(sizeof(Elemento));
	novo->dado = elem;
	novo->prox = NULL;
	if ((*fila)->inicio == NULL) {
		(*fila)->inicio = novo;
	}
	else {
		(*fila)->fim->prox = novo;
	}
	(*fila)->fim = novo;
}

void pop(Fila** fila, arvore** t) {
	if ((*fila) != NULL && (*fila)->inicio != NULL) {
		*t = (*fila)->inicio->dado;
		(*fila)->inicio = (*fila)->inicio->prox;
	}
}
