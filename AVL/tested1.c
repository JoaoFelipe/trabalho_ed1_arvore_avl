#include "simplectest/tests.h"
#include "fila.h"
#include "avl.c"
#include "fila.c"
#include <stdarg.h>

void monta_arvore(int tamanho, arvore** t, ...){
    int i;    
    va_list lista;
    *t = NULL;

    va_start(lista, tamanho);
    for (i=0; i<tamanho; i++){
        inserir(t, va_arg(lista,int), 0);
    }
    va_end(lista);
}

START_TESTS()

START_TEST("Testar Busca")
{
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
}
END_TEST()

START_TEST("Testar Remoção da arvore") {
    arvore* t = NULL;
    
    // 1 -> 1
    TEST("remove elemento 1 da arvore com elemento 1");
    {
        monta_arvore(1, &t, 1);
        remove_elemento(&t, 1);
        ASSERT(t == NULL);
        desocupar_arvore(t); 
    }

    //  7  -> 7
    //5
    TEST("remove elemento 5 da arvore com elementos 7 e 5")
    {    
        monta_arvore(2, &t, 7, 5);
        remove_elemento(&t, 5);
        ASSERT(t->dado == 7 && t->esq == NULL && t->dir == NULL);
        desocupar_arvore(t); 
    }


    //  7    ->  7
    //5   9    5
    TEST("remove elemento 9 da arvore com elementos 7 5 9")
    {
        monta_arvore(3, &t, 7, 5, 9);
        remove_elemento(&t, 9);
        ASSERT(
            t->dado == 7 &&
            t->esq->dado == 5 && 
            t->dir == NULL &&
        1);
        desocupar_arvore(t); 
    }


    //  7    ->  9
    //5   9    5
    TEST("remove elemento 7 da arvore com elementos 7 5 9")
    {
        monta_arvore(3, &t, 7, 5, 9);
        remove_elemento(&t, 7);
        ASSERT(t->dado == 9 && t->esq->dado == 5 && t->dir == NULL);
        desocupar_arvore(t);
    }

    //  7    ->  8
    //5   9    5   9
    //   8 11       11
    TEST("remove elemento 7 da arvore com elementos 7 5 9 8 11")
    {
        monta_arvore(5, &t, 7, 5, 9, 8, 11);
        remove_elemento(&t, 7);
        ASSERT(
            t->dado == 8 && 
            t->esq->dado == 5 && 
            t->dir->dado == 9 &&
            t->dir->dir->dado == 11 &&
            t->dir->esq == NULL &&
        1);

        desocupar_arvore(t);
    }

    //    7    ->  5
    //  5   9    3   7
    // 3        
    TEST("remove elemento 9 da arvore com elementos 7 5 9 3")
    {
        monta_arvore(4, &t, 7, 5, 9, 3);
        remove_elemento(&t, 9);
        ASSERT(
            t->dado == 5 && 
            t->bal == 0 &&
            t->esq->dado == 3 && 
            t->esq->bal == 0 &&
            t->dir->dado == 7 &&
            t->dir->bal == 0 &&
        1);
        desocupar_arvore(t);
    }

    //    7    ->  5
    //  5   9    3   9
    // 3        
    TEST("remove elemento 7 da arvore com elementos 7 5 9 3")
    {
        monta_arvore(4, &t, 7, 5, 9, 3);
        remove_elemento(&t, 7);
        ASSERT(
            t->dado == 5 && 
            t->bal == 0 &&
            t->esq->dado == 3 && 
            t->esq->bal == 0 &&
            t->dir->dado == 9 &&
            t->dir->bal == 0 &&
        1);
        desocupar_arvore(t);
    }

    //    7    ->  7
    //  5   9    3   9
    // 3        
    TEST("remove elemento 5 da arvore com elementos 7 5 9 3")
    {
        monta_arvore(4, &t, 7, 5, 9, 3);
        remove_elemento(&t, 5);
        ASSERT(
            t->dado == 7 && 
            t->bal == 0 &&
            t->esq->dado == 3 && 
            t->esq->bal == 0 &&
            t->dir->dado == 9 &&
            t->dir->bal == 0 &&
        1);
        desocupar_arvore(t);
    }

    //    7    ->  7
    //  5   9    5   9
    // 3        
    TEST("remove elemento 3 da arvore com elementos 7 5 9 3")
    {
        monta_arvore(4, &t, 7, 5, 9, 3);
        remove_elemento(&t, 3);
        ASSERT(
            t->dado == 7 && 
            t->bal == 0 &&
            t->esq->dado == 5 && 
            t->esq->bal == 0 &&
            t->dir->dado == 9 &&
            t->dir->bal == 0 &&
        1);
        desocupar_arvore(t);
    }

    //    7    ->  7
    //  5   9    5   9
    //       11        
    TEST("remove elemento 11 da arvore com elementos 7 5 9 11")
    {
        monta_arvore(4, &t, 7, 5, 9, 11);
        remove_elemento(&t, 11);
        ASSERT(
            t->dado == 7 && 
            t->bal == 0 &&
            t->esq->dado == 5 && 
            t->esq->bal == 0 &&
            t->dir->dado == 9 &&
            t->dir->bal == 0 &&
        1);
        desocupar_arvore(t);
    }

    //    7    ->  7
    //  5   9    5   11
    //       11        
    TEST("remove elemento 9 da arvore com elementos 7 5 9 11")
    {
        monta_arvore(4, &t, 7, 5, 9, 11);
        remove_elemento(&t, 9);
        ASSERT(
            t->dado == 7 && 
            t->bal == 0 &&
            t->esq->dado == 5 && 
            t->esq->bal == 0 &&
            t->dir->dado == 11 &&
            t->dir->bal == 0 &&
        1);
        desocupar_arvore(t);
    }
    
    //    7    ->  7
    //  5   8    5   8 
    // 4 6   9  4     9
    TEST("remove elemento 6 da arvore com elementos 7 5 8 4 6 9")
    {
        monta_arvore(6, &t, 7, 5, 8, 4, 6, 9);
        remove_elemento(&t, 6);
        ASSERT(
            t->dado == 7 && 
            t->bal == 0 &&
            t->esq->dado == 5 &&
            t->esq->bal == -1 &&
            t->esq->esq->dado == 4 &&
            t->esq->esq->bal == 0 &&
            t->dir->dado == 8 &&
            t->dir->bal == 1 &&
            t->dir->dir->dado == 9 &&
            t->dir->dir->bal == 0 &&
        1);
        desocupar_arvore(t);
    }    

    //      9     ->     6
    //  5       11    5     9
    //4   6   10    4     7   11 
    //     7        
    TEST("remove elemento 10 da arvore com elementos 9 5 11 4 6 10 7")
    {
        monta_arvore(7, &t, 9, 5, 11, 4, 6, 10, 7);
        remove_elemento(&t, 10);
        ASSERT(
            t->dado == 6 && 
            t->bal == 0 &&
            t->esq->dado == 5 && 
            t->esq->bal == -1 &&
            t->esq->esq->dado == 4 &&
            t->esq->esq->bal == 0 &&
            t->dir->dado == 9 &&
            t->dir->bal == 0 &&
            t->dir->esq->dado == 7 &&
            t->dir->esq->bal == 0 &&
            t->dir->dir->dado == 11 &&
            t->dir->dir->bal == 0 &&
        1);
        desocupar_arvore(t);
    }

    //        5     ->                     5
    //    3      8                   3           9
    //  2  4   7    11            2     4     7     11
    //1       6   9     12      1            6    10   12
    //              10                             
    TEST("remove elemento 8 da arvore com elementos 5, 3, 8, 2, 4, 7, 11, 1, 5, 9, 12, 10")
    {
        monta_arvore(12, &t, 5, 3, 8, 2, 4, 7, 11, 1, 6, 9, 12, 10);
        ASSERT(
            t->dado == 5 && 
            t->bal == 1 &&
            t->esq->dado == 3 && 
            t->esq->bal == -1 &&
            t->esq->esq->dado == 2 &&
            t->esq->esq->bal == -1 &&
            t->esq->esq->esq->dado == 1 &&
            t->esq->esq->esq->bal == 0 &&
            t->esq->dir->dado == 4 &&
            t->esq->dir->bal == 0 &&
            t->dir->dado == 8 &&
            t->dir->bal == 1 &&
            t->dir->esq->dado == 7 &&
            t->dir->esq->bal == -1 &&
            t->dir->esq->esq->dado == 6 &&
            t->dir->esq->esq->bal == 0 &&
            t->dir->dir->dado == 11 &&
            t->dir->dir->bal == -1 &&
            t->dir->dir->esq->dado == 9 &&
            t->dir->dir->esq->bal == 1 &&
            t->dir->dir->esq->dir->dado == 10 &&
            t->dir->dir->esq->dir->bal == 0 &&
            t->dir->dir->dir->dado == 12 &&
            t->dir->dir->dir->bal == 0 &&
        1);        
        remove_elemento(&t, 8);
        ASSERT(
            t->dado == 5 && 
            t->bal == 0 &&
            t->esq->dado == 3 && 
            t->esq->bal == -1 &&
            t->esq->esq->dado == 2 &&
            t->esq->esq->bal == -1 &&
            t->esq->esq->esq->dado == 1 &&
            t->esq->esq->esq->bal == 0 &&
            t->esq->dir->dado == 4 &&
            t->esq->dir->bal == 0 &&
            t->dir->dado == 9 &&
            t->dir->bal == 0 &&
            t->dir->esq->dado == 7 &&
            t->dir->esq->bal == -1 &&
            t->dir->esq->esq->dado == 6 &&
            t->dir->esq->esq->bal == 0 &&
            t->dir->dir->dado == 11 &&
            t->dir->dir->bal == 0 && 
            t->dir->dir->esq->dado == 10 &&
            t->dir->dir->esq->bal == 0 &&
            t->dir->dir->dir->dado == 12 &&
            t->dir->dir->dir->bal == 0 &&
        1);
        desocupar_arvore(t);
    }


    
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
    monta_arvore(5, &arvore, 40, 35, 50, 45, 60);
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
    monta_arvore(5, &arvore, 50, 40, 60, 45, 35);
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
    monta_arvore(5, &arvore, 40, 35, 50, 45, 60);
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

START_TEST("Testar clonagem de árvores"){

    arvore* a1;
    arvore* a2;

    TEST("Clonar árvore NULL deve retornar NULL")
    {
        monta_arvore(0, &a1);
        monta_arvore(0, &a2);
        clonar(a1, &a2);
        ASSERT(a2 == NULL);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }

    TEST("Clonar árvore com 1 elemento deve retornar árvore com 1 elemento")
    {
        monta_arvore(1, &a1, 1);
        monta_arvore(0, &a2);
        
        clonar(a1, &a2);
        ASSERT(a2->dado == a1->dado);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }

    TEST("Clonar árvore com 2 elementos deve retornar árvore com 2 elementos")
    {
        monta_arvore(2, &a1, 1, 2);
        monta_arvore(0, &a2);
        
        clonar(a1, &a2);
        ASSERT(a2->dado == a1->dado);
        ASSERT(a2->dir->dado == a1->dir->dado);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }

    TEST("Clonar árvore com 4 elementos deve retornar árvore com 4 elementos")
    {
        monta_arvore(4, &a1, 7, 6, 8, 9);
        monta_arvore(0, &a2);
        
        clonar(a1, &a2);
        ASSERT(a2->dado == a1->dado);
        ASSERT(a2->esq->dado == a1->esq->dado);
        ASSERT(a2->dir->dado == a1->dir->dado);
        ASSERT(a2->dir->dir->dado == a1->dir->dir->dado);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }
    
}END_TEST()

START_TEST("Testar união de arvores"){

    arvore* a1;
    arvore* a2;
    arvore* resultado = NULL;
    
    TEST("União de arvores NULL com NULL da NULL");
    {
        monta_arvore(0, &a1);
        monta_arvore(0, &a2);

        uniao(a1, a2, &resultado); 
        ASSERT(resultado == NULL);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }    

    TEST("União de arvores NULL com 7 da 7");
    {
        monta_arvore(0, &a1);
        monta_arvore(1, &a2, 7);

        uniao(a1, a2, &resultado); 
        ASSERT(resultado->dado == 7 && resultado->esq == NULL && resultado->dir == NULL);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }

    TEST("União de arvores 7 com NULL");
    {
        monta_arvore(1, &a1, 7);
        monta_arvore(0, &a2);

        uniao(a1, a2, &resultado); 
        ASSERT(resultado->dado == 7 && resultado->esq == NULL && resultado->dir == NULL);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }

    TEST("União de arvores 7 com 7");
    {
        monta_arvore(1, &a1, 7);
        monta_arvore(1, &a2, 7);

        uniao(a1, a2, &resultado); 
        ASSERT(resultado->dado == 7 && resultado->esq == NULL && resultado->dir == NULL);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }

    TEST("União de arvores 7 com 7 8");
    {
        monta_arvore(1, &a1, 7);
        monta_arvore(2, &a2, 7, 8);

        uniao(a1, a2, &resultado); 
        ASSERT(resultado->dado == 7 && resultado->esq == NULL && resultado->dir->dado == 8);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }

    TEST("União de arvores 7 8 com 7 6 8 9");
    {    
        monta_arvore(2, &a1, 7, 8);
        monta_arvore(4, &a2, 7, 6, 8, 9);

        uniao(a1, a2, &resultado); 
        ASSERT(
            resultado->dado == 7 && 
            resultado->esq->dado == 6 && 
            resultado->dir->dado == 8 && 
            resultado->dir->dir->dado == 9
        );

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }

    TEST("União de arvores 7 6 8 9 com 6 9");
    {
        monta_arvore(4, &a1, 7, 6, 8, 9);
        monta_arvore(2, &a2, 6, 9);

        uniao(a1, a2, &resultado); 
        ASSERT(
            resultado->dado == 7 && 
            resultado->esq->dado == 6 && 
            resultado->dir->dado == 8 && 
            resultado->dir->dir->dado == 9
        );

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }

}END_TEST()

START_TEST("Testar subtração de arvores"){

    arvore* a1;
    arvore* a2;
    arvore* resultado = NULL;

    TEST("Subtração de arvores NULL com NULL deve retornar NULL")
    {
        monta_arvore(0, &a1);
        monta_arvore(0, &a2);
        
        subtracao(a1, a2, &resultado);
        ASSERT(resultado == NULL);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }

    TEST("Subtração de arvores NULL com 1 deve retornar NULL")
    {
        monta_arvore(0, &a1);
        monta_arvore(1, &a2, 1);
        
        subtracao(a1, a2, &resultado);
        ASSERT(resultado == NULL);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }

    TEST("Subtração de arvores 1 com NULL deve retornar 1")
    {
        monta_arvore(1, &a1, 1);
        monta_arvore(0, &a2);
        
        subtracao(a1, a2, &resultado);
        ASSERT(resultado->dado == 1);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }

    TEST("Subtração de arvores 1 com 1 deve retornar NULL")
    {
        monta_arvore(1, &a1, 1);
        monta_arvore(1, &a2, 1);
        
        subtracao(a1, a2, &resultado);
        ASSERT(resultado == NULL);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }

    TEST("Subtração de arvores 1 2 com 2 deve retornar 1")
    {
        monta_arvore(2, &a1, 1, 2);
        monta_arvore(1, &a2, 2);
        
        subtracao(a1, a2, &resultado);
        ASSERT(resultado->dado == 1);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }

    TEST("Subtração de arvores 1 2 0 com 1 deve retornar 2 0")
    {
        monta_arvore(3, &a1, 0, 1, 2);
        monta_arvore(1, &a2, 1);
        
        subtracao(a1, a2, &resultado);
        ASSERT(resultado->dado == 2 && resultado->esq->dado == 0);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }

    TEST("Subtração de arvores 1 2 0 com 2 0 deve retornar 1")
    {
        monta_arvore(3, &a1, 0, 1, 2);
        monta_arvore(2, &a2, 2, 0);
        
        subtracao(a1, a2, &resultado);
        ASSERT(resultado->dado == 1);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }

    TEST("Subtração de arvores 7 6 8 9 com 6 9 deve retornar 8 7");
    {
        monta_arvore(4, &a1, 7, 6, 8, 9);
        monta_arvore(2, &a2, 6, 9);

        subtracao(a1, a2, &resultado);
        ASSERT(
            resultado->dado == 8 && 
            resultado->esq->dado == 7 && 
        1);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }
    
}END_TEST()

START_TEST("Testar Interseção de arvores"){

    arvore* a1;
    arvore* a2;
    arvore* resultado = NULL;

    TEST("Interseção de arvores NULL com NULL deve retornar NULL")
    {
        monta_arvore(0, &a1);
        monta_arvore(0, &a2);
        
        intersecao(a1, a2, &resultado);
        ASSERT(resultado == NULL);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }

    TEST("Interseção de arvores NULL com 1 deve retornar NULL")
    {
        monta_arvore(0, &a1);
        monta_arvore(1, &a2, 1);
        
        intersecao(a1, a2, &resultado);
        ASSERT(resultado == NULL);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }

    TEST("Interseção de arvores 1 com NULL deve retornar NULL")
    {
        monta_arvore(1, &a1, 1);
        monta_arvore(0, &a2);
        
        intersecao(a1, a2, &resultado);
        ASSERT(resultado == NULL);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }

    TEST("Interseção de arvores 1 com 1 deve retornar 1")
    {
        monta_arvore(1, &a1, 1);
        monta_arvore(1, &a2, 1);
        
        intersecao(a1, a2, &resultado);
        ASSERT(resultado->dado == 1);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }

    TEST("Interseção de arvores 1 2 com 2 deve retornar 2")
    {
        monta_arvore(2, &a1, 1, 2);
        monta_arvore(1, &a2, 2);
        
        intersecao(a1, a2, &resultado);
        ASSERT(resultado->dado == 2);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }

    TEST("Interseção de arvores 1 2 0 com 1 deve retornar 1")
    {
        monta_arvore(3, &a1, 0, 1, 2);
        monta_arvore(1, &a2, 1);
        
        intersecao(a1, a2, &resultado);
        ASSERT(resultado->dado == 1);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }

    TEST("Interseção de arvores 1 2 0 com 2 0 deve retornar 2 0")
    {
        monta_arvore(3, &a1, 0, 1, 2);
        monta_arvore(2, &a2, 2, 0);
        
        intersecao(a1, a2, &resultado);
        ASSERT(resultado->dado == 2 && resultado->esq->dado == 0);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }

    TEST("Interseção de arvores 7 6 8 9 com 6 9 deve retornar 9 6");
    {
        monta_arvore(4, &a1, 7, 6, 8, 9);
        monta_arvore(2, &a2, 6, 9);

        intersecao(a1, a2, &resultado);
        ASSERT(
            resultado->dado == 9 && 
            resultado->esq->dado == 6 && 
        1);

        desocupar_arvore(a1);
        desocupar_arvore(a2);
    }
    
}END_TEST()

END_TESTS()
