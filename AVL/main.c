#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_gfxPrimitives.h>
#include <string.h>
#endif
#include "avl.h"


int opcao_selecionada, maxx, maxy, meio_x;
int menu_ix, menu_iy, menu_x, menu_y;
int opt_sx, opt_sy, opt_dist;
int arv_y, arv_ey, arv_dist_x, arv_r, arv_dist_y;
int message_ix, message_iy, message_x, message_y, message_texty, message_texts;
int input_ix, input_iy, input_x, input_y, input_cx, input_cy;

int bgcolor;
int linecolor;
int selectedcolor;
int menutextcolor;
int exittextcolor;
int treecolor;
int numbercolor;
char titulo[255];
char m1[255];
char m2[255];
char m3[255];
char input[255];
int input_tipo;

int debug = 1;

/*

input_tipo:
  0: inicio,
  1:insere em t1 e para,
  2:insere em t2 e para,
  3:remove de t1,
  4:remove de t2,
  5:insere continuamente em t1,
  6:insere continuamente em t2,

*/
arvore* arvore_atual;
arvore* t1;
arvore* t2;
arvore* t3;
SDL_Surface* screen;


int init_graphics() {

    maxx = 1024;
    maxy = 768;

    menu_ix = 0;
    menu_iy = 50;

    menu_x = 200;
    menu_y = maxy;

    opt_sx = 180;
    opt_sy = 30;
    opt_dist = 10;

    arv_y = 200;
    arv_ey = 300;

    bgcolor = 0x0000;
    linecolor = 0x0F00;
    selectedcolor = 0x3330;
    menutextcolor = 0xAAA0;
    exittextcolor = 0xF000;
    treecolor = 0xFFF0;
    numbercolor = 0xFF00;

    arv_dist_x = 15;
    arv_r = 20;
    arv_dist_y = 50;

    message_ix = menu_x;
    message_iy = maxy - 180;
    message_texty = maxy - 160;
    message_texts = 20;

    message_x = maxx;
    message_y = maxy;

    meio_x = menu_x + ((maxx - menu_x) / 2);


    input_ix = meio_x - 100;
    input_iy = maxy - 100;
    input_x = meio_x + 100;
    input_y = maxy - 60;

    input_cx = meio_x - 90;
    input_cy = maxy - 88;


    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 0;
    }


    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    TTF_Init();
//    atexit(TTF_Quit);

    // create a new window
    screen = SDL_SetVideoMode(1024, 768, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if (screen == NULL) {
        printf("Unable to set 1024x768 video: %s\n", SDL_GetError());
        return 0;
    }

    return 1;

}

void rectangle(int x, int y, int xf, int yf, int color, int bg) {
    SDL_Rect rect = {x,y,xf-x,yf-y};
    SDL_FillRect(screen, &rect, color);
    SDL_Rect rect2 = {x+1,y+1,xf-x-2,yf-y-2};
    SDL_FillRect(screen, &rect2, bg);
}

void text(int x, int y, int cor, char* text){
    TTF_Font *fntCourier = TTF_OpenFont( "verdana.ttf", 12 );
    SDL_Color clrFg = {((cor >> 12) % 16)*16, ((cor >> 8) % 16)*16, ((cor >> 4) % 16)*16};
    SDL_Surface *sText = TTF_RenderText_Solid( fntCourier, text, clrFg );
    SDL_Rect rcDest = {x,y,0,0};
    SDL_BlitSurface( sText,NULL, screen, &rcDest);
    SDL_FreeSurface( sText );
    TTF_CloseFont( fntCourier );
}

void line(int x1, int y1, int x2, int y2, int cor){
    lineRGBA(screen, x1, y1, x2, y2,
       ((cor >> 12) % 16)*16, ((cor >> 8) % 16)*16, ((cor >> 4) % 16)*16, 255);
}

void circle(int x, int y, int r, int cor){
    ellipseRGBA(screen, x, y, r, r,
       ((cor >> 12) % 16)*16, ((cor >> 8) % 16)*16, ((cor >> 4) % 16)*16, 255);
}


void textsize(char* text, int* w, int* h){
    TTF_Font *fnt= TTF_OpenFont( "verdana.ttf", 12 );
    TTF_SizeText(fnt, text, w, h);
    TTF_CloseFont( fnt );
}

void imprime_texto_centralizado(int x, int y, int cor, char* texto) {
    int w,h;
	textsize(texto, &w, &h);
	int diffx = w/2;
    int diffy = h/2;


	text(x - diffx, y - diffy, cor, texto);
}

void imprime_opcao_menu(int x, int y, int selecionado, char* texto) {
	int cor;
	cor = selecionado? selectedcolor: bgcolor;
	rectangle(x, y, x + opt_sx, y + opt_sy, linecolor, cor);
	cor = (texto == "Sair")? exittextcolor: menutextcolor;

    imprime_texto_centralizado(x + opt_sx/2, y + opt_sy/2, cor, texto);
}

void imprime_menu(){
    rectangle(menu_ix, menu_iy, menu_x, menu_y, linecolor, bgcolor);
    text(menu_ix + 85, menu_iy + 15, linecolor, "Menu");

    int sy = (opt_dist + opt_sy);
    int x = (menu_ix + opt_dist);
    int y = (menu_iy + sy);

    imprime_opcao_menu(x, y, 0 == opcao_selecionada ? 1 : 0, "Imprime T1");
    imprime_opcao_menu(x, y + sy, 1 == opcao_selecionada ? 1 : 0, "Imprime T2");
    imprime_opcao_menu(x, y + 2*sy, 2 == opcao_selecionada ? 1 : 0, "Intersecao T1 e T2");
    imprime_opcao_menu(x, y + 3*sy, 3 == opcao_selecionada ? 1 : 0, "Uniao T1 e T2");
    imprime_opcao_menu(x, y + 4*sy, 4 == opcao_selecionada ? 1 : 0, "Subtracao T1 - T2");
    imprime_opcao_menu(x, y + 5*sy, 5 == opcao_selecionada ? 1 : 0, "Subtracao T2 - T1");
    imprime_opcao_menu(x, y + 6*sy, 6 == opcao_selecionada ? 1 : 0, "Buscar elemento");
    imprime_opcao_menu(x, y + 7*sy, 7 == opcao_selecionada ? 1 : 0, "Inserir em T1");
    imprime_opcao_menu(x, y + 8*sy, 8 == opcao_selecionada ? 1 : 0, "Inserir em T2");
    imprime_opcao_menu(x, y + 9*sy, 9 == opcao_selecionada ? 1 : 0, "Remover em T1");
    imprime_opcao_menu(x, y + 10*sy, 10 == opcao_selecionada ? 1 : 0, "Remover em T2");
    imprime_opcao_menu(x, y + 11*sy, 11 == opcao_selecionada ? 1 : 0, "Sair");
}

void imprime_titulo(){
    rectangle(menu_x, 0, maxx, menu_iy, linecolor, bgcolor);
    imprime_texto_centralizado(meio_x, menu_iy/2, linecolor, titulo);
}

void atualizar_titulo(char *t){
    sprintf(titulo, "%s", t);
    imprime_titulo();
}

void imprime_no(int x, int y, int valor) {
	char numero[10];
	sprintf(numero, "%d", valor);
	imprime_texto_centralizado(x, y, numbercolor, numero);
	circle(x, y, arv_r, treecolor);

	//int diffx = textwidth(numero) / 2;
	//int diffy = textheight(numero) / 2;
	//setfontcolor(YELLOW);
	//outtextxy(x - diffx + 3, y - diffy + 2, numero);
	//setfontcolor(WHITE);
	//circle(x, y, 20);
}

void imprime_recursivo(arvore *t, int x, int y, int altura, int altura_max) {
	if (t != NULL) {
	    if (debug) {
	        char numero[10];
            sprintf(numero, "%d", t->bal);
            imprime_texto_centralizado(x+arv_r, y-arv_r, numbercolor, numero);

	    }

		imprime_no(x, y, t->dado);
		int diff = altura_max - altura;
		int xesq =  x - (arv_dist_x * pow(2, diff));
		int xdir =  x + (arv_dist_x * pow(2, diff));
		if (t->esq != NULL) {
			line(x, y + arv_r, xesq, y + arv_dist_y, linecolor);
			imprime_recursivo(t->esq, xesq, y + arv_r + arv_dist_y, altura + 1, altura_max);
		}
		if (t->dir != NULL) {
			line(x, y + arv_r, xdir, y + arv_dist_y, linecolor);
			imprime_recursivo(t->dir, xdir, y + arv_r + arv_dist_y, altura + 1, altura_max);
		}
	}
}

void imprime_arvore(arvore* t){
    rectangle(menu_x, menu_iy, maxx, menu_y, linecolor, bgcolor);
    if (t != NULL) {
		int altura_max = pegar_maior_altura(t, 0);
		imprime_recursivo(t, meio_x, arv_y, 0, altura_max);
	}
	else {
		imprime_texto_centralizado(meio_x, arv_ey, treecolor, "Arvore vazia");
	}
}

void imprime_caixa_mensagem(){
    rectangle(message_ix, message_iy, message_x, message_y, linecolor, bgcolor);
    imprime_texto_centralizado(meio_x, message_texty + 0*message_texts, linecolor, m1);
    imprime_texto_centralizado(meio_x, message_texty + 1*message_texts, linecolor, m2);
    imprime_texto_centralizado(meio_x, message_texty + 2*message_texts, linecolor, m3);

    if (strcmp(input, "-1") != 0){
        rectangle(input_ix, input_iy, input_x, input_y, linecolor, bgcolor);
        text(input_cx, input_cy, numbercolor, input);
    }

}

void desenha_interface(){
    //text(600, 20, cor, "Gerenciador de Arvores AVL");
    imprime_menu();
    imprime_titulo();
    imprime_arvore(arvore_atual);
    imprime_caixa_mensagem();
  //  SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

}

void mensagem_vazia() {
    sprintf(input, "-1");
    sprintf(m1, "");
    sprintf(m2, "");
    sprintf(m3, "");
}

void mensagem_ler_numero() {
    sprintf(m1, "Digite um numero");
    sprintf(m2, "ou pressione a barra de espaco para cancelar");
    sprintf(m3, "");
}

void mensagem_construir(char* T){
    char temp[255];
    sprintf(temp, "Criando arvore %s", T);
    atualizar_titulo(temp);
    sprintf(m1, "Construa agora a arvore %s, digitando um numero e apertando enter.", T);
    sprintf(m2, "Quando quiser terminar, pressione a barra de espacos.");
    sprintf(m3, "Pressione qualquer tecla para continuar...");

}

void mensagem_escolher(){
    sprintf(m1, "Digite o numero:");
    sprintf(m2, "1- T1");
    sprintf(m3, "2- T2");

}


void ler_numero(int tipo){
    sprintf(input, "");
    input_tipo = tipo;
    mensagem_ler_numero();
}

void monitorar_input(int status){
    if (status == -1){
        mensagem_vazia();
        switch (input_tipo){
            case 1:
				arvore_atual = t1;
				atualizar_titulo("Exibindo T1, insercao cancelada");
                break;
            case 2:
				atualizar_titulo("Exibindo T2, insercao cancelada");
                break;
            case 3:
				atualizar_titulo("Exibindo T1, exclusão cancelada");
                break;
            case 4:
				atualizar_titulo("Exibindo T2, exclusão cancelada");
                break;
            case -1:
            case 5:
                mensagem_construir("T2");
                input_tipo = 0;
                break;
            case 0:
            case 6:
                opcao_selecionada = 0;
                input_tipo = 7;
                break;
            case 9:
            case 8:
                input_tipo = 7;
                break;
        }
    }
    if (status == 1){
        int numero = atoi(input);
        switch (input_tipo){
            case -1:
                arvore_atual = t1;
                ler_numero(5);
                break;
            case -0:
                arvore_atual = t2;
                ler_numero(6);
                break;
            case 1:
                inserir(&t1, numero, 0);
				arvore_atual = t1;
				mensagem_vazia();
				atualizar_titulo("Exibindo T1 depois de inserir um elemento");
                break;
            case 2:
                inserir(&t2, numero, 0);
				arvore_atual = t2;
				mensagem_vazia();
				atualizar_titulo("Exibindo T2 depois de inserir um elemento");
                break;
            case 3:
                remove_elemento(&t1, numero);
				arvore_atual = t1;
				mensagem_vazia();
				atualizar_titulo("Exibindo T1 depois de remover um elemento");
                break;
            case 4:
                remove_elemento(&t2, numero);
				arvore_atual = t2;
				mensagem_vazia();
				atualizar_titulo("Exibindo T2 depois de remover um elemento");
                break;
            case 5:
                inserir(&t1, numero, 0);
                sprintf(input, "");
				arvore_atual = t1;
				break;
            case 6:
                inserir(&t2, numero, 0);
                sprintf(input, "");
				arvore_atual = t2;
				break;
            case 8:
                if (numero == 1){
                    arvore_atual = t1;
                    atualizar_titulo("Buscando em T1");
                } else {
                    arvore_atual = t2;
                    atualizar_titulo("Buscando em T2");
                }
                ler_numero(9);
                break;
            case 9:
                mensagem_vazia();
                arvore* temp = NULL;
                int encontrou = busca(&arvore_atual, &temp, numero);

                atualizar_titulo("Exibindo arvore encontrada");
                if (encontrou == 1){
                    sprintf(m1, "O elemento %i foi encontrado", numero);
                    if (temp != NULL){
                        sprintf(m2, "Filho da %s do elemento %i", (temp->esq == arvore_atual)? "esquerda" : "direita", temp->dado);
                    } else {
                        sprintf(m2, "Raiz da arvore");
                    }
                    sprintf(m3, "Balanco: %i", arvore_atual->bal);
                } else {
                    sprintf(m1, "O elemento %i nao foi encontrado", numero);
                }

              //


        }

    }

}


void interpretar_comando() {
	int numero;
	arvore_atual = NULL;
	switch (opcao_selecionada) {
		case 0:
			atualizar_titulo("Exibindo arvore T1");
			arvore_atual = t1;
			break;
		case 1:
			atualizar_titulo("Exibindo arvore T2");
			arvore_atual = t2;
			break;
		case 2:
			atualizar_titulo("Exibindo a intersecao entre T1 e T2");
			if (t3 != NULL){
                desocupar_arvore(t3);
                t3 = NULL;
			}
			intersecao(t1, t2, &t3);
			arvore_atual = t3;
			break;
		case 3:
			atualizar_titulo("Exibindo a uniao entre T1 e T2");
			if (t3 != NULL){
                desocupar_arvore(t3);
                t3 = NULL;
			}
			uniao(t1, t2, &t3);
			arvore_atual = t3;
			break;
		case 4:
			atualizar_titulo("Exibindo a subtração de T1 com T2");
			if (t3 != NULL){
                desocupar_arvore(t3);
                t3 = NULL;
			}
			subtracao(t1, t2, &t3);
			arvore_atual = t3;
			break;
		case 5:
			atualizar_titulo("Exibindo a subtração de T2 com T1");
			if (t3 != NULL){
                desocupar_arvore(t3);
                t3 = NULL;
			}
			subtracao(t2, t1, &t3);
			arvore_atual = t3;
			break;
		case 6:
			atualizar_titulo("Buscar em:");
            mensagem_escolher();
            sprintf(input, "");
            input_tipo = 8;
			//TODO: busca
			break;
		case 7:
			atualizar_titulo("Inserindo na arvore T1");
			arvore_atual = t1;
			ler_numero(1);
            break;
		case 8:
			atualizar_titulo("Inserindo na arvore T2");
			arvore_atual = t2;
			ler_numero(2);
            break;
		case 9:
			atualizar_titulo("Removendo da arvore T1");
			arvore_atual = t1;
			ler_numero(3);
			break;
		case 10:
			atualizar_titulo("Removendo da arvore T2");
			arvore_atual = t2;
			ler_numero(4);
			break;
		case 11:
            exit(0);
			break;
	}
}


int ler_input(int key){

    if (key == 32)
        return -1;
    if (key >= 48 && key <= 57 && strlen(input) < 5) {
        sprintf(input, "%s%c", input, key);
    }
    if (key == 8 && strlen(input) > 0) {
        input[strlen(input)-1] = 0;
    }

    //printf("%i\n", key);

    if (key == 13)
        return 1;
    return 0;


}

int ler_menu(int key){
   // printf("%i\n", key);
    if (key ==SDLK_UP) //cima
        opcao_selecionada = opcao_selecionada >= 1 ? opcao_selecionada - 1 : 11;
    if (key == SDLK_DOWN) // baixo
        opcao_selecionada = opcao_selecionada <= 10 ? opcao_selecionada + 1 : 0;
    if (key == 13) { //enter
        interpretar_comando();
        // se opção for sair, sai do loop, fechando o programa
        if (opcao_selecionada == 11)
            return 1;
    }
    return 0;

}


int main ( int argc, char** argv ) {
#ifdef __cplusplus
    printf("c++");
#else
    printf("c");
#endif

    screen = NULL;

    if (!init_graphics()){
        return 1;
    }

    opcao_selecionada = -1;
    arvore_atual = NULL;
	t1 = NULL;
	t2 = NULL;
	t3 = NULL;
	sprintf(titulo, "");
	mensagem_vazia();
	sprintf(input, "-1");
	input_tipo = -1;
    mensagem_construir("T1");



    int done = 0;
    while (!done) {
        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // check for messages
            switch (event.type) {
                // exit if the window is closed
                case SDL_QUIT:
                    done = 1;
                    break;

                    // check for keypresses
                case SDL_KEYDOWN:
                    // exit if ESCAPE is pressed
                    if ((strcmp(input, "-1") != 0)  || (opcao_selecionada == -1)) {
                        monitorar_input(ler_input(event.key.keysym.sym));
                    } else {
                        if (ler_menu(event.key.keysym.sym))
                            done = 1;
                    }


                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = 1;
                    break;

            } // end switch
        } // end of message processing

        // DRAWING STARTS HERE

        // clear screen
        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

        // draw bitmap
        desenha_interface();
        // DRAWING ENDS HERE

        // finally, update the screen :)
        SDL_Flip(screen);
    } // end main loop


    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}
