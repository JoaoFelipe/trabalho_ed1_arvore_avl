#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <stdarg.h>
#include "ed1.c"

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

void init() {
	int d=0,m=0;
	initgraph(&d,&m,"c:\\tc");
}

void imprime_no(int x, int y, int valor) {
	char numero[10];
	sprintf(numero, "%d", valor);
	int diffx = textwidth(numero) / 2;
	int diffy = textheight(numero) / 2;
	outtextxy(x - diffx + 3, y - diffy + 2, numero);
	circle(x, y, 20);
}

void imprime_arvore(arvore *t, int x, int y, int altura){
	int altura_max = pegar_maior_altura(t, 0);
	imprime_no(x, y, t->dado);
	int xesq =  x - 20 - (pow(2, altura_max - altura) * 10);
	int xdir =  x + 20 + (pow(2, altura_max - altura) * 10);
	if (t->esq != NULL) {
		line(x, y + 20, xesq, y + 50);
		imprime_arvore(t->esq, xesq, y + 70, altura + 1);
	}
	if (t->dir != NULL) {
		line(x, y + 20, xdir, y + 50);
		imprime_arvore(t->dir, xdir, y + 70, altura + 1);
	}
}

void imprime_opcao_menu(int x, int y, int selecionado, char* texto) {
	rectangle(x, y, x + 150, y + 30);
	if (selecionado == 1) {
		setcolor(DARKGRAY);
		floodfill(x + 1, y + 1, WHITE);
		setcolor(WHITE);
	}
	int diffx = textwidth(texto) / 2;
	outtextxy(x + (75 - diffx), y + 10, texto);
}

void imprime_menu(int opcao_selecionada) {
	int x = 5;
	int y = 5;
	imprime_opcao_menu(x, y, 0 == opcao_selecionada ? 1 : 0, "Imprime T1");
	imprime_opcao_menu(x, y + 40, 1 == opcao_selecionada ? 1 : 0, "Imprime T2");
	imprime_opcao_menu(x, y + 80, 2 == opcao_selecionada ? 1 : 0, "Intersecao T1 e T2");
	imprime_opcao_menu(x, y + 120, 3 == opcao_selecionada ? 1 : 0, "Uniao T1 e T2");
	imprime_opcao_menu(x, y + 160, 4 == opcao_selecionada ? 1 : 0, "Subtracao T1 - T2");
	imprime_opcao_menu(x, y + 200, 5 == opcao_selecionada ? 1 : 0, "Subtracao T2 - T1");
	imprime_opcao_menu(x, y + 240, 6 == opcao_selecionada ? 1 : 0, "Buscar elemento");
	imprime_opcao_menu(x, y + 280, 7 == opcao_selecionada ? 1 : 0, "Inserir em T1");
	imprime_opcao_menu(x, y + 320, 8 == opcao_selecionada ? 1 : 0, "Inserir em T2");
	imprime_opcao_menu(x, y + 360, 9 == opcao_selecionada ? 1 : 0, "Remover em T1");
	imprime_opcao_menu(x, y + 400, 10 == opcao_selecionada ? 1 : 0, "Remover em T2");
	imprime_opcao_menu(x, y + 440, 11 == opcao_selecionada ? 1 : 0, "Sair");
}

void interpretar_comando(int opcao_selecionada, arvore* t1, arvore* t2) {
	switch (opcao_selecionada) {
		case 0:
			monta_arvore(5, &t1, 7, 5, 9, 3, 4);
			imprime_arvore(t1, 400, 50, 0);
			break;
		case 1:
			imprime_arvore(t2, 400, 50, 0);
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			closegraph();
			break;
	}
}

void main() {
	init();
	cleardevice();
	setcolor(WHITE);
	//print_no(100, 400, 9999);
	char key;
	int opcao_menu = 0;
	arvore* t1 = NULL;
	arvore* t2 = NULL;
	imprime_menu(opcao_menu);
	do {
		while(!kbhit()) {
			delay(10);
		}
		key = getch();
		cleardevice();
		// caractere ',' para ir pra cima no menu
		if (key == 44) {
			opcao_menu = opcao_menu - 1 >= 0 ? opcao_menu - 1 : 11;
		}
		// caractere '.' para ir pra baixo no menu
		if (key == 46) {
			opcao_menu = opcao_menu + 1 <= 11 ? opcao_menu + 1 : 0;
		}
		if (key == 13) {
			interpretar_comando(opcao_menu, t1, t2);
			if (opcao_menu == 11)
				return;
		}
		imprime_menu(opcao_menu);
	} while (0 == 0);
}

//libsdl-image1.2 libsdl-image1.2-dev guile-1.8 guile-1.8-dev libsdl1.2debian-all libaudiofile-dev libesd0-dev libdirectfb-dev libdirectfb-extra libfreetype6-dev libxext-dev x11proto-xext-dev libfreetype6 libaa1 libaa1-dev libslang2-dev libasound2 libasound2-dev
