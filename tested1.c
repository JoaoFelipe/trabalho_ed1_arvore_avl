#include "simplectest/tests.h"
#include "ed1.c"

START_TESTS()

START_TEST("Testar Busca"){
//      Arvore:
//          50
//      30      90
//    20  40      100


    arvore* a20 = malloc(sizeof(arvore));
    a20->esq = NULL; a20->dir = NULL; a20->dado = 20;
    
    arvore* a40 = malloc(sizeof(arvore));
    a40->esq = NULL; a40->dir = NULL; a40->dado = 40;
    
    arvore* a100 = malloc(sizeof(arvore));
    a100->esq = NULL; a100->dir = NULL; a100->dado = 100;
        
    arvore* a90 = malloc(sizeof(arvore));
    a90->esq = NULL; a90->dir = a100; a90->dado = 90;

    arvore* a30 = malloc(sizeof(arvore));
    a30->esq = a20; a30->dir = a40; a30->dado = 30;

    arvore* a50 = malloc(sizeof(arvore));
    a50->esq = a30; a50->dir = a90; a50->dado = 50;

    TEST("Busca 50 deve retornar 1, com ponteiro = Raiz, e pai Nulo"){
        arvore* temp = a50;
        arvore* pai = NULL;
        int retorno = busca(&temp, &pai, 50);
        ASSERT(retorno == 1 && temp == a50 && pai == NULL);
    }

    TEST("Busca 30 deve retornar 1, com ponteiro = a30, e pai a50"){
        arvore* temp = a50;
        arvore* pai = NULL;
        int retorno = busca(&temp, &pai, 30);
        ASSERT(retorno == 1 && temp == a30 && pai == a50);
    }

    TEST("Busca 20 deve retornar 1, com ponteiro = a20, e pai a30"){
        arvore* temp = a50;
        arvore* pai = NULL;
        int retorno = busca(&temp, &pai, 20);
        ASSERT(retorno == 1 && temp == a20 && pai == a30);
    }

    TEST("Busca 40 deve retornar 1, com ponteiro = a240, e pai a30"){
        arvore* temp = a50;
        arvore* pai = NULL;
        int retorno = busca(&temp, &pai, 40);
        ASSERT(retorno == 1 && temp == a40 && pai == a30);
    }

    TEST("Busca 90 deve retornar 1, com ponteiro = a90, e pai a50"){
        arvore* temp = a50;
        arvore* pai = NULL;
        int retorno = busca(&temp, &pai, 90);
        ASSERT(retorno == 1 && temp == a90 && pai == a50);
    }

    TEST("Busca 100 deve retornar 1, com ponteiro = a100, e pai a90"){
        arvore* temp = a50;
        arvore* pai = NULL;
        int retorno = busca(&temp, &pai, 100);
        ASSERT(retorno == 1 && temp == a100 && pai == a90);
    }

    TEST("Busca 10 deve retornar 2, com ponteiro = a20, e pai a30"){
        arvore* temp = a50;
        arvore* pai = NULL;
        int retorno = busca(&temp, &pai, 10);
        ASSERT(retorno == 2 && temp == a20 && pai == a30);
    }

    TEST("Busca 25 deve retornar 3, com ponteiro = a20, e pai a30"){
        arvore* temp = a50;
        arvore* pai = NULL;
        int retorno = busca(&temp, &pai, 25);
        ASSERT(retorno == 3 && temp == a20 && pai == a30);
    }

    TEST("Busca 35 deve retornar 2, com ponteiro = a40, e pai a30"){
        arvore* temp = a50;
        arvore* pai = NULL;
        int retorno = busca(&temp, &pai, 35);
        ASSERT(retorno == 2 && temp == a40 && pai == a30);
    }

    TEST("Busca em arvore vazia deve retornar 0"){
        arvore* temp = NULL;
        arvore* pai = NULL;
        int retorno = busca(&temp, &pai, 45);
        ASSERT(retorno == 0 && temp == NULL && pai == NULL);
    }

    free(a20);
    free(a40);
    free(a100);
    free(a90);
    free(a30);
    free(a50);
}END_TEST()

START_TEST("Testar Append da lista") {
    lista* l = NULL;
    

    TEST("Insere elemento 10 na lista vazia");
    append(&l, 10);
    ASSERT(l->dado == 10);

    TEST("Insere elemento 27 na lista com elemento 10")
    append(&l, 27);
    ASSERT(l->dado == 27 && l->prox->dado == 10);

    desocupar_lista(l);
    
} END_TEST()

START_TEST("Testar remover da arvore") {
    arvore* t = NULL;
    int* i = NULL;
    

    TEST("remove elemento 1 da arvore com elemento 1");
    inserir(&t, 1, 1);
    remove_elemento(&t, 1, i);
    ASSERT(t == NULL);

    TEST("remove elemento 1 da arvore com elementos 1 e 2")
    inserir(&t, 1, 1);
    inserir(&t, 2, 1);
    remove_elemento(&t, 1, i);
    ASSERT(t->dado == 2);

    TEST("remove elemento 3 da arvore com elementos 1 e 2 e 3 e 4")
    inserir(&t, 2, 1);
    inserir(&t, 1, 1);
    inserir(&t, 3, 1);
    inserir(&t, 4, 1);
    remove_elemento(&t, 3, i);
    ASSERT(
        t->dado == 2 &&
        t->esq->dado == 1 && 
        t->dir->dado == 4 
);

    desocupar_arvore(t);
    
} END_TEST()

START_TEST("Testar Inserção AVL"){
    arvore* arvore = NULL;

    TEST("Insere elemento 50 na arvore vazia");
    inserir(&arvore, 50, 1);
    ASSERT(arvore->dado == 50 && arvore->bal == 0);

    TEST("Insere elemento 40 na arvore resultante");
    inserir(&arvore, 40, 1);
    ASSERT(
        arvore->dado == 50 && arvore->bal == -1 &&
        arvore->esq->dado == 40 && arvore->esq->bal == 0
    );

    TEST("Insere elemento 60 na arvore resultante");
    inserir(&arvore, 60, 1);
    ASSERT(
        arvore->dado == 50 && arvore->bal == 0 && 
        arvore->esq->dado == 40 && arvore->esq->bal == 0 && 
        arvore->dir->dado == 60 && arvore->dir->bal == 0 &&
    1);

    TEST("Insere elemento 45 na arvore resultante");
    inserir(&arvore, 45, 1);
    ASSERT(
        arvore->dado == 50 && arvore->bal == -1 && 
        arvore->esq->dado == 40 && arvore->esq->bal == 1 && 
        arvore->dir->dado == 60 && arvore->dir->bal == 0 &&
        arvore->esq->dir->dado == 45 && arvore->esq->dir->bal == 0 && 
    1);

    TEST("Insere elemento 35 na arvore resultante");
    inserir(&arvore, 35, 1);
    ASSERT(
        arvore->dado == 50 && arvore->bal == -1 && 
        arvore->esq->dado == 40 && arvore->esq->bal == 0 && 
        arvore->dir->dado == 60 && arvore->dir->bal == 0 &&
        arvore->esq->dir->dado == 45 && arvore->esq->dir->bal == 0 &&
        arvore->esq->esq->dado == 35 && arvore->esq->esq->bal == 0 &&  
    1);
    
    TEST("Insere elemento 25 na arvore resultante (Rotação direita)");
    inserir(&arvore, 25, 1);
    ASSERT(
        arvore->dado == 40 && arvore->bal == 0 && 
        arvore->dir->dado == 50 && arvore->dir->bal == 0 &&
        arvore->dir->dir->dado == 60 && arvore->dir->dir->bal == 0 &&
        arvore->dir->esq->dado == 45 && arvore->dir->esq->bal == 0 &&
        arvore->esq->dado == 35 && arvore->esq->bal == -1 &&
        arvore->esq->esq->dado == 25 && arvore->esq->esq->bal == 0 &&  
    1);

    
    desocupar_arvore(arvore);

    arvore = NULL;

    TEST("Refaz arvore na ordem 40,35,50,45,60");
    inserir(&arvore, 40, 1);
    inserir(&arvore, 35, 1);
    inserir(&arvore, 50, 1);
    inserir(&arvore, 45, 1);
    inserir(&arvore, 60, 1);
    ASSERT(
        arvore->dado == 40 && arvore->bal == 1 && 
        arvore->dir->dado == 50 && arvore->dir->bal == 0 &&
        arvore->dir->dir->dado == 60 && arvore->dir->dir->bal == 0 &&
        arvore->dir->esq->dado == 45 && arvore->dir->esq->bal == 0 &&
        arvore->esq->dado == 35 && arvore->esq->bal == 0 &&
    1);

    TEST("Insere elemento 70 na arvore resultante (Rotação esquerda)");
    inserir(&arvore, 70, 1);
    ASSERT(
        arvore->dado == 50 && arvore->bal == 0 && 
        arvore->dir->dado == 60 && arvore->dir->bal == 1 &&
        arvore->dir->dir->dado == 70 && arvore->dir->dir->bal == 0 &&
        arvore->esq->dado == 40 && arvore->esq->bal == 0 &&
        arvore->esq->dir->dado == 45 && arvore->esq->dir->bal == 0 &&
        arvore->esq->esq->dado == 35 && arvore->esq->esq->bal == 0 &&  
    1);    

    desocupar_arvore(arvore);

    arvore = NULL;

    TEST("Refaz arvore na ordem 50,40,60,45,35");
    inserir(&arvore, 50, 1);
    inserir(&arvore, 40, 1);
    inserir(&arvore, 60, 1);
    inserir(&arvore, 45, 1);
    inserir(&arvore, 35, 1);
    ASSERT(
        arvore->dado == 50 && arvore->bal == -1 && 
        arvore->esq->dado == 40 && arvore->esq->bal == 0 && 
        arvore->dir->dado == 60 && arvore->dir->bal == 0 &&
        arvore->esq->dir->dado == 45 && arvore->esq->dir->bal == 0 &&
        arvore->esq->esq->dado == 35 && arvore->esq->esq->bal == 0 &&  
    1);

    TEST("Insere elemento 42 na arvore resultante (Rotação dupla direita)");
    inserir(&arvore, 42, 1);
    ASSERT(
        arvore->dado == 45 && arvore->bal == 0 && 
        arvore->dir->dado == 50 && arvore->dir->bal == 1 &&
        arvore->dir->dir->dado == 60 && arvore->dir->dir->bal == 0 &&
        arvore->esq->dado == 40 && arvore->esq->bal == 0 &&
        arvore->esq->dir->dado == 42 && arvore->esq->dir->bal == 0 &&
        arvore->esq->esq->dado == 35 && arvore->esq->esq->bal == 0 &&  
    1);  
     
    desocupar_arvore(arvore);

    arvore = NULL;

    TEST("Refaz arvore na ordem 40,35,50,45,60");
    inserir(&arvore, 40, 1);
    inserir(&arvore, 35, 1);
    inserir(&arvore, 50, 1);
    inserir(&arvore, 45, 1);
    inserir(&arvore, 60, 1);
    ASSERT(
        arvore->dado == 40 && arvore->bal == 1 && 
        arvore->dir->dado == 50 && arvore->dir->bal == 0 &&
        arvore->dir->dir->dado == 60 && arvore->dir->dir->bal == 0 &&
        arvore->dir->esq->dado == 45 && arvore->dir->esq->bal == 0 &&
        arvore->esq->dado == 35 && arvore->esq->bal == 0 &&
    1);

    TEST("Insere elemento 47 na arvore resultante (Rotação dupla esquerda)");
    inserir(&arvore, 47, 1);
    ASSERT(
        arvore->dado == 45 && arvore->bal == 0 && 
        arvore->dir->dado == 50 && arvore->dir->bal == 0 &&
        arvore->dir->dir->dado == 60 && arvore->dir->dir->bal == 0 &&
        arvore->dir->esq->dado == 47 && arvore->dir->esq->bal == 0 &&
        arvore->esq->dado == 40 && arvore->esq->bal == -1 &&
        arvore->esq->esq->dado == 35 && arvore->esq->esq->bal == 0 &&  
    1);  
     
    desocupar_arvore(arvore);

}END_TEST()


/*
    TEST("Tamanho da arvore");
    {
        arvore* a = malloc(sizeof(arvore));
        a->esq = NULL;
        a->dir = NULL;
        a->dado = 2;

        ASSERT(count(a) == 1);

        arvore* b = malloc(sizeof(arvore));
        b->esq = a;
        b->dir = NULL;
        b->dado = 1;

        ASSERT(count(b) == 2);


        arvore* c = malloc(sizeof(arvore));
        c->esq = NULL;
        c->dir = b;
        c->dado = 0;

        ASSERT(count(c) == 3);

        arvore* d = malloc(sizeof(arvore));
        d->esq = a;
        d->dir = a;
        d->dado = 0;

        ASSERT(count(d) == 3);
    
        arvore* e;
        e = NULL;
        ASSERT(count(e) == 0);
        
        int existe;
        remove_elemento(a, 1, &existe); printf("\n");
        remove_elemento(b, 1, &existe); printf("\n");
        remove_elemento(c, 1, &existe); printf("\n");
        remove_elemento(d, 1, &existe); printf("\n");
        remove_elemento(e, 1, &existe); printf("\n");

        free(a);
        free(b);
        free(c);
        free(d);

        
    }
*/

//    ASSERT_EQUALS_FLOAT(1, 1);



END_TESTS()
