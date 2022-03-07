/* Pong Gameplay - Movimente as palhetas com W e S (Esquerda) e O e L (Direita). */

#include <ncurses.h> 
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define DELAY 50000
#define MAX_GOLS 10

int main (void) {
	int x = 0, y = 0, max_x = 0, max_y = 0, next_x = 0, 
	next_y = 0, direction_x = 0, direction_y = 0,
	gol_esq = 0, gol_dir = 0, palheta_esq_y = 0, palheta_dir_y;
 	int *pdx = &direction_x;
 	int *pdy = &direction_y;
 	int *px = &x;
 	int *py = &y;
 	int *pmaxx = &max_x;
 	int *pmaxy = &max_y;
 	int *pgole = &gol_esq;
 	int *pgold = &gol_dir;
 	int *ppalesqy = &palheta_esq_y;
 	int *ppaldiry = &palheta_dir_y;
 	
 	//Inicia janela e oculta cursor
 	initscr(); 
 	curs_set(FALSE);
	
	getmaxyx(stdscr, max_y, max_x); 

	//Coloca bolinha na posição inicial no meio da tela
	x = max_x/2;
	y = max_y/2;
	
	//Sorteia a direção inicial
	sorteia_posicao(pdx, pdy);
	
	//Fica em repetição até ocorrer um erro (CTRL + C)
	while (1) { 
	 	clear();  //Limpa a tela
	 	
	 	//Verifica se algum dos placares chegou a 10, se positivo encerra o programa
		if (checar_placar(pgole, pgold) == 1) {
			sair();
		}
		
		//Imprime a bolinha O, nomes dos times e placar de cada um
		mvprintw(y, x, "O"); 
		mvprintw(1, 22, "Time A");
		mvprintw(2, 25, "%d", *pgole);
		mvprintw(1, max_x - 28, "Time B");
		mvprintw(2, max_x - 25, "%d", *pgold);
		//Desenha as "palhetas" a serem movimentadas pelos usuários
		desenha_palheta_inicial(pmaxx, pmaxy, ppalesqy, ppaldiry);
		
		//Imprime um | a cada 2 linhas para simbolizar a metade do campo
		for(int i = 0; i < max_y; i+=2) {
			mvprintw(i, max_x/2, "|");
		}
		
		//Captura tecla digitada
		int key = getch();
	
		//Switch verifica se foi digitado um comando para movimentar as palhetas. W, w, S e S movimentam a palheta esquerda, O, o, L e l movimentam a direita. Conforme a tecla verificada, chama o método para redesenhar a palheta na nova posição.
		switch (key) {
			case 87: // W
				redesenha_palheta_esq(pmaxx, pmaxy, ppalesqy);
			case 119: // w
				redesenha_palheta_esq(pmaxx, pmaxy, ppalesqy);
			case 83: // S
			  redesenha_palheta_esq(pmaxx, pmaxy, ppalesqy);
			case 115: // s
				redesenha_palheta_esq(pmaxx, pmaxy, ppalesqy);
			case 79: // O
				redesenha_palheta_dir(pmaxx, pmaxy, ppaldiry);	
			case 111: // o  
				redesenha_palheta_dir(pmaxx, pmaxy, ppaldiry);
			case 76: // L
				redesenha_palheta_dir(pmaxx, pmaxy, ppaldiry);	
			case 108: // l  
				redesenha_palheta_dir(pmaxx, pmaxy, ppaldiry);
			case 51: // ESC
		  	sair ();
			default:
		 		refresh();
  	}
		
		//verificar movimentos. onde encaixar a função.
		//Redesenhar palhetas
		//Checar colisão com palhetas.		
		refresh(); 

		usleep (DELAY); 
		
		next_x = x + direction_x;
		next_y = y + direction_y;	

		checar_limites(next_x, next_y, pmaxx, pmaxy, px, py, pdx, pdy, pgole, pgold);
	}

 	return 0;
}

//Desenha as palhetas nas posições iniciais nos cantos da tela, atribui os valores da posição em y das palhetas.
void desenha_palheta_inicial(int *pmx, int *pmy, int *ppalesqy, int *ppaldiry) {
	mvprintw(1, 1, "|");
	mvprintw(2, 1, "|");
	mvprintw(3, 1, "|");
	mvprintw(*pmy-1, *pmx-1, "|");
	mvprintw(*pmy-2, *pmx-1, "|");
	mvprintw(*pmy-3, *pmx-1, "|");
	*ppalesqy = 1;
	*ppaldiry = *pmy-1;
}

//Redesenha palheta direita, deve verificar se não está acima/abaixo dos limites da tela (vertical y) e fazer mvprintw nas novas posições (uma acima ou uma abaixo)
void redesenha_palheta_dir(int *pmx, int *pmy, int *ppaldiry) {

}

//Redesenha palheta esquerda, deve verificar se não está acima/abaixo dos limites da tela (vertical y) e fazer mvprintw nas novas posições (uma acima ou uma abaixo)
void redesenha_palheta_esq(int *pmx, int *pmy, int *ppalesqy) {

}

void sorteia_posicao(int *d_x, int *d_y) {
	//Funçao para sortear números
	srand(time(NULL));
	int r_x = 0;
	int r_y = 0;

	//Sorteia 2 números, atribui a x ou y;
	for (int i = 0; i < 2; i++) {
		if (i == 0) {
			r_x = rand() % 2; //Sorteia entre 0 e 1
		} else {
			r_y = rand() % 2; //Sorteia entre 0 e 1
		}
	}
	
	if (r_x == 1) { //Atribui direção inicial 1 ou -1 (frente ou trás)
		*d_x = 1;
	} else {
		*d_x = -1;
	}
	
	if (r_y == 1) { //Atribui direção inicial 1 ou -1 (frente ou trás)
		*d_y = 1;
	} else {
		*d_y = -1;
	}
}

//Recebe ponteiros para as variáveis gols do time esquerdo e direito. Checa se um dos dois marcou 10 gols, retorna 1 e encerra a execução caso sim, senão retorna 0.
int checar_placar(int *pgole, int *pgold) {
	if (*pgole == MAX_GOLS || *pgold == MAX_GOLS) {
		return 1;
	} else {
		return 0;
	}
}

//Recebe as variáveis. next_x e next_y são modificadas dentro do laço while. Os demais são ponteiros para as variáveis de controle, x, y, x máximo, y máximo, posição x e y e gol do time esquerdo e direito.
void checar_limites(int next_x, int next_y, int *pmx, int *pmy, int *px, int *py, int *pdx, int *pdy, int *pgole, int *pgold) {  
  //Checa colisão em cima ou embaixo e inverte a direção
  if (next_y >= *pmy || next_y < 0) {
    *pdy *= -1;   
  } else {
    *py += *pdy;
  } 
  
  //Checa se chegou no início ou final de x. Caso isso ocorra, contabiliza um gol para o adversário, e reinicia o movimento da bolinha no lado que sofreu o gol
  if (next_x >= *pmx || next_x < 0) {
    if (next_x < 0) {
    	*pgold += 1;
    	*py = *pmy / 2;
    	*px = 10;
    	*pdx = 1;
    } else {
    	*pgole += 1;
    	*py = *pmy / 2;
    	*px = *pmx - 10;
    	*pdx = -1;
    }    
  } else {
    *px += *pdx;
  }
}

void checar_colisao(int *next_x, int *next_y, int *pmx, int *pmy, int *px, int *py, int *pdx, int *pdy) {
  //Colisão com as palhetas	
}

void sair() {
 endwin ();
 exit (0);
}
