#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <stdarg.h>
#include <string.h>
#include "ed1.c"

#include <termios.h>
#include <unistd.h>

int mygetch( ) {
  struct termios oldt,
                 newt;
  int            ch;
  tcgetattr( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
  return ch;
}

int opcao_selecionada, maxx, maxy, meio_x;
char* titulo;
arvore* arvore_atual;
arvore* t1;
arvore* t2;

void erase(int x, int y, int width, int height){
    setcolor(BLACK);
    bar(x, y, width, height);
    setcolor(GREEN);
	rectangle(x, y, width, height);
}


void clear(){
    erase(0,0,maxx, maxy);  
}

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
	clear();
	outtextxy(600, 220, "Carregando interface...");
	for (i = 0; i < 100;  i++) {
		setcolor(GREEN);
		rectangle(maxx/2 - 200, 250, maxx/2 + 200, 270);
		line((maxx/2 - 200) + 4 * i, 250, (maxx/2 - 200) + 4 * i, 270);
		setcolor(BLACK);
		floodfill((maxx/2 - 199), 260, GREEN);
		delay(25);
	}
    clear();
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
    erase(200, 50, maxx - 5, maxy - 180);
	if (t != NULL) {
		int altura_max = pegar_maior_altura(t, 0);
		imprime_recursivo(t, x, y, 0, altura_max);
	}
	else {
		imprime_texto_centralizado(300, "Arvore vazia");
	}
}

void imprime_opcao_menu(int x, int y, int selecionado, char* texto) {
	erase(x, y, x + 180, y + 30);
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

void imprime_opcao(int x, int y, int opcao) {
	switch (opcao){
		case 0:
			return imprime_opcao_menu(x, y, 0 == opcao_selecionada ? 1 : 0, "Imprime T1");
		case 1:
			return imprime_opcao_menu(x, y + 40, 1 == opcao_selecionada ? 1 : 0, "Imprime T2");
		case 2:
			return imprime_opcao_menu(x, y + 80, 2 == opcao_selecionada ? 1 : 0, "Intersecao T1 e T2");
		case 3:
			return imprime_opcao_menu(x, y + 120, 3 == opcao_selecionada ? 1 : 0, "Uniao T1 e T2");
		case 4:
			return imprime_opcao_menu(x, y + 160, 4 == opcao_selecionada ? 1 : 0, "Subtracao T1 - T2");
		case 5:
			return imprime_opcao_menu(x, y + 200, 5 == opcao_selecionada ? 1 : 0, "Subtracao T2 - T1");
		case 6:
			return imprime_opcao_menu(x, y + 240, 6 == opcao_selecionada ? 1 : 0, "Buscar elemento");
		case 7:
			return imprime_opcao_menu(x, y + 280, 7 == opcao_selecionada ? 1 : 0, "Inserir em T1");
		case 8:
			return imprime_opcao_menu(x, y + 320, 8 == opcao_selecionada ? 1 : 0, "Inserir em T2");
		case 9:
			return imprime_opcao_menu(x, y + 360, 9 == opcao_selecionada ? 1 : 0, "Remover em T1");
		case 10:
			return imprime_opcao_menu(x, y + 400, 10 == opcao_selecionada ? 1 : 0, "Remover em T2");
		case 11:
			return imprime_opcao_menu(x, y + 440, 11 == opcao_selecionada ? 1 : 0, "Sair");
	} 
}

void imprime_menu(int x, int y) {
	int i;
	outtextxy(85, 65, "Menu");
	for (i = 0; i < 12; i++)
		imprime_opcao(x,y,i);
}

void titulo_box(){
    erase(200, 0, maxx - 5, 50);
	imprime_texto_centralizado(20, titulo);
}

void atualizar_titulo(char* t){
    titulo = t;
    titulo_box();
}

void message_box(char* m1, char* m2, char* m3){
    erase(200, maxy - 180, maxx - 5, maxy-5);
	imprime_texto_centralizado(maxy - 160, m1);
	imprime_texto_centralizado(maxy - 140, m2);
    imprime_texto_centralizado(maxy - 120, m3);
}


void desenha_interface(int desenha_arvore) {
	clear();
	setfontcolor(GREEN);
	outtextxy(600, 20, "Gerenciador de Arvores AVL");
	setfontcolor(WHITE);
	rectangle(5, 50, 195, maxy - 50);
	rectangle(200, 50, maxx - 5, maxy - 50);
	imprime_menu(10, 90);
    titulo_box();
	//imprime_texto_centralizado(80, titulo);
	if (desenha_arvore == 1)
		imprime_arvore(arvore_atual, meio_x, 200);
}





void desenha_get_numero(char* texto) {
	//desenha_interface(1); // *
    erase(meio_x - 100, maxy - 100, meio_x + 100, maxy-60);
	moveto(meio_x - 90, maxy - 85);
	setfontcolor(YELLOW);
	outtext(texto);
	setfontcolor(WHITE);
}

int get_numero(int message) {
	char texto[6];
	texto[0] = 0;
	texto[1] = 0;
	texto[2] = 0;
	texto[3] = 0;
	texto[4] = 0;
	texto[5] = 0;
	//desenha_interface(1); //*
    if (message)    
        message_box(
            "Digite um numero",
            "ou pressione a barra de espaco para cancelar",
            "ou 'e' para apagar um caracter"
        );
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
		if (key == 'e' && tamanho > 0) {
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
	switch (opcao_selecionada) {
		case 0:	
			atualizar_titulo("Exibindo arvore T1");
			imprime_arvore((arvore_atual = t1), meio_x, 200);
			break;
		case 1:
			atualizar_titulo("Exibindo arvore T2");
			imprime_arvore((arvore_atual = t2), meio_x, 200);
			break;
		case 2:
			titulo = "Exibinco a intersecao entre T1 e T2";
			//TODO: intersecao
			break;
		case 3:
			//TODO: uniao
			break;
		case 4:
			//TODO: t1 - t2
			break;
		case 5:
			//TODO: t2 - t1
			break;
		case 6:
			//TODO: busca
			break;
		case 7:
			atualizar_titulo("Inserindo na arvore T1");
			imprime_arvore((arvore_atual = t1), meio_x, 200);
			numero = get_numero(1);
			if (numero >= 0) {
				inserir(&t1, numero, 0);
				imprime_arvore((arvore_atual = t1), meio_x, 200);
				atualizar_titulo("Exibindo T1 depois de inserir um elemento");
			}
			else
				atualizar_titulo("Exibindo T1, insercao cancelada");
			break;
		case 8:
			atualizar_titulo("Inserindo na arvore T2");
			imprime_arvore((arvore_atual = t2), meio_x, 200);
			numero = get_numero(1);
			if (numero >= 0) {
				inserir(&t2, numero, 0);
				imprime_arvore((arvore_atual = t2), meio_x, 200);
				atualizar_titulo("Exibindo T2 depois de inserir um elemento");
			}
			else
				atualizar_titulo("Exibindo T2, insercao cancelada");
			break;
		case 9:
			atualizar_titulo("Removendo da arvore T1");
			imprime_arvore((arvore_atual = t1), meio_x, 200);
			remove_elemento(&t1, get_numero(1));
			imprime_arvore((arvore_atual = t1), meio_x, 200);
			atualizar_titulo("Exibindo T1 depois de remover um elemento");
			break;
		case 10:
			atualizar_titulo("Removendo da arvore T2");
			imprime_arvore((arvore_atual = t2), meio_x, 200);
			remove_elemento(&t2, get_numero(1));
			imprime_arvore((arvore_atual = t2), meio_x, 200);
			atualizar_titulo("Exibindo T2 depois de remover um elemento");
			break;
		case 11:
			closegraph();
			break;
	}
}

void imprimir_mensagem_get_numero(){
    message_box(
        "Digite um numero",
        "ou pressione a barra de espaco para cancelar",
        "ou 'e' para apagar um caracter"
    );
}

void construir(char* T, arvore** tn){
	int numero;
    char temp[255];

    sprintf(temp, "Criando arvore %s", T);
    atualizar_titulo(temp);
	    
    sprintf(temp, "Construa agora a arvore %s, digitando um numero e apertando enter.", T);
    message_box(
        temp,
        "Quando quiser terminar, pressione a barra de espacos.",
        "Pressione qualquer tecla para continuar..."
    );
    getch();

    imprime_arvore(NULL, meio_x, 200);
    imprimir_mensagem_get_numero();

    do {
		numero = get_numero(0);
		if (numero >= 0){
			inserir(tn, numero, 0);	
            imprime_arvore(*tn, meio_x, 200);
        }
	} while (numero >= 0);
}

void construir_listas() {
	construir("T1", &t1);
	construir("T2", &t2);
	atualizar_titulo("Selecione uma opcao do menu");
}


int main() {
	int anterior;
	init_graphics();
	opcao_selecionada = -1;
	t1 = NULL;
	t2 = NULL;
	titulo = "";
	desenha_interface(1);
	construir_listas();
	opcao_selecionada = 0;
	char key;
	desenha_interface(1);
	do {
		key = (char) getchar();
		if (key == 44 || key == 46 || key == 13) {
			anterior = opcao_selecionada;
			// caractere ',' para ir pra cima no menu
			if (key == 44) {
				opcao_selecionada = opcao_selecionada >= 1 ? opcao_selecionada - 1 : 11;
			}
			// caractere '.' para ir pra baixo no menu
			if (key == 46) {
				opcao_selecionada = opcao_selecionada <= 10 ? opcao_selecionada + 1 : 0;
			}
			// enter para executar opção selecioanda
			if (key == 13) {
				interpretar_comando();
				// se opção for sair, sai do loop, fechando o programa
				if (opcao_selecionada == 11)
					return 0;
			}
			imprime_opcao(10, 90, anterior);
			imprime_opcao(10, 90, opcao_selecionada);
			erase(200, maxy - 180, maxx - 5, maxy-5);
			//desenha_interface(1);
			delay(10);
		}
	} while (0 == 0);
	return 0;
}

//libsdl-image1.2 libsdl-image1.2-dev guile-1.8 guile-1.8-dev libsdl1.2debian-all libaudiofile-dev libesd0-dev libdirectfb-dev libdirectfb-extra libfreetype6-dev libxext-dev x11proto-xext-dev libfreetype6 libaa1 libaa1-dev libslang2-dev libasound2 libasound2-dev
