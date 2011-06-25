#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

void push(Fila* fila, arvore* elem) {
	Elemento* novo = (Elemento*)malloc(sizeof(Elemento));
	novo->dado = elem;
	novo->prox = NULL;
	if (fila->inicio == NULL) {
		fila->inicio = novo;
	} else {
		fila->fim->prox = novo;
	}
	fila->fim = novo;
}

void pop(Fila* fila, arvore** t) {
	Elemento* temp;
	if (fila->inicio != NULL) {
		temp = fila->inicio;
		*t = temp->dado;
		fila->inicio = fila->inicio->prox;
		free(temp);
	}
}
