#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <stdarg.h>
#include "ed1.c"

int opcao_selecionada, maxx, maxy, meio_x;
char* titulo;
arvore* arvore_atual;
arvore* t1;
arvore* t2;

void init_graphics() {
	int d = 1, m = 0;
	initgraph(&d, &m, NULL);
	setcolor(GREEN);
	int i;
	maxx = getmaxx();
	maxy = getmaxy();
	meio_x = 200 + ((maxx - 200) / 2);
	// loading screen
	// gambiarra para o programa não ser abortado
	for (i = 0; i < 50;  i++) {
		setcolor(GREEN);
		outtextxy(600, 220, "Carregando interface...");
		rectangle(maxx/2 - 200, 250, maxx/2 + 200, 270);
		line((maxx/2 - 200) + 8 * i, 250, (maxx/2 - 200) + 8 * i, 270);
		setcolor(WHITE);
		floodfill((maxx/2 - 199), 260, GREEN);
		delay(10);
		cleardevice();
	}
	setcolor(GREEN);
}

void imprime_no(int x, int y, int valor) {
	char numero[10];
	sprintf(numero, "%d", valor);
	int diffx = textwidth(numero) / 2;
	int diffy = textheight(numero) / 2;
	setfontcolor(YELLOW);
	outtextxy(x - diffx + 3, y - diffy + 2, numero);
	setfontcolor(WHITE);
	circle(x, y, 20);
}

void imprime_recursivo(arvore *t, int x, int y, int altura, int altura_max) {
	if (t != NULL) {
		imprime_no(x, y, t->dado);
		int diff = altura_max - altura;
		int xesq =  x - (15 * pow(2, diff));
		int xdir =  x + (15 * pow(2, diff));
		if (t->esq != NULL) {
			line(x, y + 20, xesq, y + 50);
			imprime_recursivo(t->esq, xesq, y + 70, altura + 1, altura_max);
		}
		if (t->dir != NULL) {
			line(x, y + 20, xdir, y + 50);
			imprime_recursivo(t->dir, xdir, y + 70, altura + 1, altura_max);
		}
	}
}

void imprime_texto_centralizado(int y, char* texto) {
	int diffx = textwidth(texto) / 2;
	outtextxy(meio_x - diffx, y, texto);
}

void imprime_arvore(arvore *t, int x, int y){
	if (t != NULL) {
		int altura_max = pegar_maior_altura(t, 0);
		imprime_recursivo(t, x, y, 0, altura_max);
	}
	else {
		imprime_texto_centralizado(300, "Arvore vazia");
	}
}

void imprime_opcao_menu(int x, int y, int selecionado, char* texto) {
	rectangle(x, y, x + 180, y + 30);
	if (selecionado == 1) {
		setcolor(DARKGRAY);
		floodfill(x + 1, y + 1, GREEN);
		setcolor(GREEN);
	}
	int diffx = textwidth(texto) / 2;
	if (texto == "Sair")
		setfontcolor(RED);
	else
		setfontcolor(LIGHTGRAY);
	outtextxy(x + (90 - diffx), y + 10, texto);
	setfontcolor(WHITE);
}

void imprime_menu(int x, int y) {
	outtextxy(85, 65, "Menu");
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

void desenha_interface(int desenha_arvore) {
	cleardevice();
	setfontcolor(GREEN);
	outtextxy(600, 20, "Gerenciador de Arvores AVL");
	setfontcolor(WHITE);
	rectangle(5, 50, 195, maxy - 50);
	rectangle(200, 50, maxx - 5, maxy - 50);
	imprime_menu(10, 90);
	imprime_texto_centralizado(80, titulo);
	if (desenha_arvore == 1)
		imprime_arvore(arvore_atual, meio_x, 200);
}

void desenha_get_numero(char* texto) {
	desenha_interface(0);
	rectangle(meio_x - 100, 300, meio_x + 100, 340);
	imprime_texto_centralizado(240, "Digite um numero");
	imprime_texto_centralizado(260, "ou pressione a barra de espaco para cancelar");
	moveto(meio_x - 90, 315);
	setfontcolor(YELLOW);
	outtext(texto);
	setfontcolor(WHITE);
}

int get_numero() {
	char texto[6];
	texto[0] = 0;
	texto[1] = 0;
	texto[2] = 0;
	texto[3] = 0;
	texto[4] = 0;
	texto[5] = 0;
	desenha_interface(0);
	desenha_get_numero("");
	char key;
	int tamanho = 0;
	int numero;
	do {
		key = getchar();
		if (key == 32)
			return -1;
		if (key >= 48 && key <= 57 && tamanho < 5) {
			texto[tamanho++] = (char)key;
			desenha_get_numero(texto);
		}
		if (key == 8 && tamanho > 0) {
			texto[--tamanho] = 0;
			desenha_get_numero(texto);
		}
	} while (key != 13);
	texto[tamanho] = '\0';
	numero = atoi(texto);
	return numero;
}

void interpretar_comando() {
	int numero;
	arvore_atual = NULL;
	switch (opcao_selecionada) {
		case 0:	
			titulo = "Exibindo arvore T1";
			arvore_atual = t1;
			break;
		case 1:
			titulo = "Exibindo arvore T2";
			arvore_atual = t2;
			break;
		case 2:
			titulo = "Exibindo a intersecao entre T1 e T2";
			intersecao(t1, t2, &arvore_atual);
			break;
		case 3:
			titulo = "Exibindo a uniao entre T1 e T2";
			uniao(t1, t2, &arvore_atual);
			break;
		case 4:
			titulo = "Exibindo a subtração de T1 com T2";
			subtracao(t1, t2, &arvore_atual);
			break;
		case 5:
			titulo = "Exibindo a subtração de T2 com T1";
			subtracao(t2, t1, &arvore_atual);
			break;
		case 6:
			titulo = "Exibindo resultados da busca";
			//TODO: busca
			break;
		case 7:
			titulo = "Inserindo na arvore T1";
			numero = get_numero();
			if (numero >= 0) {
				inserir(&t1, numero, 0);
				arvore_atual = t1;
				titulo = "Exibindo T1 depois de inserir um elemento";
			}
			else
				titulo = "Exibindo T1, insercao cancelada";
			break;
		case 8:
			titulo = "Inserindo na arvore T2";
			numero = get_numero();
			if (numero >= 0) {
				inserir(&t2, numero, 0);
				arvore_atual = t2;
				titulo = "Exibindo T2 depois de inserir um elemento";
			}
			else
				titulo = "Exibindo T2, insercao cancelada";
			break;
		case 9:
			titulo = "Removendo da arvore T1";
			remove_elemento(&t1, get_numero());
			arvore_atual = t1;
			titulo = "Exibindo T1 depois de remover um elemento";
			break;
		case 10:
			titulo = "Removendo da arvore T2";
			remove_elemento(&t2, get_numero());
			arvore_atual = t2;
			titulo = "Exibindo T2 depois de remover um elemento";
			break;
		case 11:
			closegraph();
			break;
	}
}

void construir_listas() {
	int numero;
	titulo = "Criando arvore T1";
	desenha_interface(0);
	imprime_texto_centralizado(300, "Construa agora a arvore T1, digitando um numero e apertando enter.");
	imprime_texto_centralizado(320, "Quando quiser terminar, pressione a barra de espacos.");
	imprime_texto_centralizado(360, "Pressione qualquer tecla para continuar...");
	getch();
	do {
		numero = get_numero();
		if (numero >= 0)
			inserir(&t1, numero, 0);	
	} while (numero >= 0);
	titulo = "Criando arvore T2";
	desenha_interface(0);
	imprime_texto_centralizado(300, "Construa agora a arvore T2, digitando um numero e apertando enter.");
	imprime_texto_centralizado(320, "Quando quiser terminar, pressione a barra de espacos.");
	imprime_texto_centralizado(360, "Pressione qualquer tecla para continuar...");
	getch();
	do {
		numero = get_numero();
		if (numero >= 0)
			inserir(&t2, numero, 0);	
	} while (numero >= 0);
	titulo = "Selecione uma opcao do menu";
}

int main() {
	init_graphics();
	opcao_selecionada = -1;
	arvore* t1 = NULL;
	arvore* t2 = NULL;
	arvore_atual = NULL;
	construir_listas();
	opcao_selecionada = 0;
	char key;
	desenha_interface(1);
	do {
		key = getch();
		if (key == 44 || key == 46 || key == 13) {
			// caractere ',' para ir pra cima no menu
			if (key == 44) {
				opcao_selecionada = opcao_selecionada - 1 >= 0 ? opcao_selecionada - 1 : 11;
			}
			// caractere '.' para ir pra baixo no menu
			if (key == 46) {
				opcao_selecionada = opcao_selecionada + 1 <= 11 ? opcao_selecionada + 1 : 0;
			}
			// enter para executar opção selecioanda
			if (key == 13) {
				interpretar_comando();
				// se opção for sair, sai do loop, fechando o programa
				if (opcao_selecionada == 11)
					return 0;
			}
			desenha_interface(1);
		}
	} while (0 == 0);
	return 0;
}

//libsdl-image1.2 libsdl-image1.2-dev guile-1.8 guile-1.8-dev libsdl1.2debian-all libaudiofile-dev libesd0-dev libdirectfb-dev libdirectfb-extra libfreetype6-dev libxext-dev x11proto-xext-dev libfreetype6 libaa1 libaa1-dev libslang2-dev libasound2 libasound2-dev
