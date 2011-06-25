#ifndef AVL_H_INCLUDED
#define AVL_H_INCLUDED

typedef struct _arvore arvore;
struct _arvore {
    arvore* dir;
    arvore* esq;
    int dado;
    int bal;
};

void desocupar_arvore(arvore* t);
int busca(arvore** t, arvore** pai, int elemento);

void rotacao_esquerda(arvore** t);
void rotacao_direita(arvore** t);
int balancear_esq(arvore** t, int ins);
int balancear_dir(arvore** t, int ins);

void novo_no_arvore(arvore** t, int valor);
int inserir(arvore** t, int valor, int texto);

int remove_simples(arvore** t);
int remove_elemento(arvore** t, int valor);

void clonar(arvore* t, arvore** clone);

void uniao_recursiva(arvore** t, arvore* outra);
void uniao(arvore* t1, arvore* t2, arvore** resultado);

void subtracao_recursiva(arvore** t, arvore* outra);
void subtracao(arvore* t1, arvore* t2, arvore** resultado);

void intersecao_recursiva(arvore** t, arvore* outra);
void intersecao(arvore* t1, arvore* t2, arvore** resultado);

int pegar_maior_altura(arvore* t, int atual);

#endif // AVL_H_INCLUDED
