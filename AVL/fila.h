#ifndef FILA_H_INCLUDED
#define FILA_H_INCLUDED
#include "avl.h"

typedef struct no {
	arvore* dado;
	struct no *prox;
} Elemento;

typedef struct {
	Elemento* inicio;
	Elemento* fim;
} Fila;


void push(Fila* fila, arvore* elem);
void pop(Fila* fila, arvore** t);

#endif // AVL_H_INCLUDED
