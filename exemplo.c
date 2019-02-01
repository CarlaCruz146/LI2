#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cgi.h"
#include "estado.h"

FILE * gameinfo;
FILE * score;

#define MAX_BUFFER		10240
#define TAM				15
#define ESCALA			60


/**
\brief Função que nos indica a abcissa da posição de um inimigo, sendo 'a' o tipo dos inimigos
*/
int getposinimigox(ESTADO e, int a){
	return e.inimigo[ a ].pos.x;
}

/**
\brief Função que nos indica a ordenada da posição de um inimigo, sendo 'a' o tipo dos inimigos
*/
int getposinimigoy(ESTADO e, int a){
	return e.inimigo[ a ].pos.y;
}


/**
\brief Função que imprime casa transparente
*/

void imprime_casa_transparente(int x, int y) {
	char *cor = "#ffffff";
	QUADRADOT(x, y,ESCALA, cor,0.0);
}

/**
\brief Função que recebe o Estado de jogo e uma posição, e caso o nível seja múltiplo de 5 verifica se é a posição da loja
*/
int tem_shop(ESTADO e,int x,int y){
	if (e.nivel % 5 != 0)
		return 0;
	return (e.shop.x == x && e.shop.y == y);
}


/**
\brief Função que nos indica a abcissa da posição do jogador
*/
int getposjogx(ESTADO e){
	return e.jog.pos.x;
}
/**
\brief Função que nos indica a ordenada da posição do jogador
*/
int getposjogy(ESTADO e){
	return e.jog.pos.y;
}
/**
\brief Função que nos indica se uma posição é válida (maior ou igual a zero e sempre menor que o tamanho do lado do tabuleiro)
*/
int posicao_valida(int x, int y) {
	return (x >= 0 && y >= 0 && x < TAM && y < TAM);
}
/**
\brief Função que nos indica se duas posições são iguais
*/
int posicao_igual (POSICAO p, int x, int y){
	return p.x == x && p.y == y ;
}
/**
\brief Função que nos indica se a posição de um montro é igual a uma determinada posição
*/
int posicao_igualm (INFOMONSTRO p, int x, int y){
	return p.pos.x == x && p.pos.y == y ;
}
/**
\brief Função que nos indica se a posição do jogador é igual a uma determinada posição
*/
int posicao_igualp (INFOJOG p, int x, int y){
	return p.pos.x == x && p.pos.y == y ;
}

/**
\brief Função que nos indica se uma determinada posição tem algum inimigo
*/
int tem_inimigo (ESTADO e, int x, int y) {
	int i;
	for (i = 0; i < e.num_inimigos; i++)
		if (posicao_igualm (e.inimigo[i], x, y))
			return 1;
	return 0;
}

/**
\brief Função que nos indica se uma determinada posição tem obstáculos
*/
int tem_obstaculo (ESTADO e, int x, int y) {
	int i;
	for (i = 0; i < e.num_obstaculos; i++){
		if (e.obstaculo[i].type == 0)
			if (posicao_igual (e.obstaculo[i].pos, x, y))
				return 1;
	}

	return 0;
}

/**
\brief Função que nos indica se uma determinada posição tem algum pote
*/
int tem_pot (ESTADO e, int x, int y) {
	int i;
	for (i = 0; i < e.num_pot; i++)
		if (posicao_igual (e.pot[i].pos, x, y))
			return i;
	return -1;
}

/**
\brief Função que recebe o Estado e uma posição, e diz se o jogador está nessa mesma posição
*/
int tem_jogador (ESTADO e, int x, int y) {
	return getposjogx(e) == x && getposjogy(e) == y;
}

/**
\brief Função que recebe o Estado e uma posição, e diz se a saída está nessa mesma posição
*/
int tem_saida (ESTADO e, int x, int y) {
	return e.saida.x == x && e.saida.y == y;
}
/**
\brief Função que nos indica se alguma casa está ocupada por inimigo, obstáculo ou jogador
*/
int casa_ocupada (ESTADO e, int x, int y) {
	return tem_inimigo (e, x, y) || tem_obstaculo (e, x, y) || tem_jogador (e, x, y) ;
}

/**
\brief Função que nos indica se alguma posição está ocupada por inimigo, obstáculo, jogador ou saída 
*/
int casa_ocupada_cs (ESTADO e, int x, int y) {
	return casa_ocupada (e, x, y) || tem_saida (e, x, y);
}
/**
\brief Função que aumenta a experência e o gold do jogador. 
*/
ESTADO getexp (ESTADO e){
	e.jog.exp += e.nivel * e.jog.expboost;
	int expfinal = (e.jog.lvl * e.jog.lvl );
	if (e.jog.exp >= expfinal){
		e.jog.exp -= expfinal;
		e.jog.lvl++;
		e.jog.lvlupstats++;
	}
	e.jog.gold += random() % (e.nivel*10);
	return e;
}

/**
\brief Função que nos indica o dano causado por cada tipo de inimigos.
*/
ESTADO atacajogador(ESTADO e, int i){

	if (e.inimigo[i].type == 0 ){ // Inimigo do tipo Xamã
		e.jog.heal -= (5 + e.inimigo[i].lvl  - e.jog.spiri / 2) ;
		if (e.jog.heal < 0) e.jog.heal = 0;
	}if (e.inimigo[i].type == 1 ){ // Inimigo do tipo 
		e.jog.heal -= (10 + e.inimigo[i].lvl - e.jog.percep - e.jog.stam/2) ;
		if (e.jog.heal < 0) e.jog.heal = 0;
	}if (e.inimigo[i].type == 2 ){ // Inimigo do tipo Arqueiro
		e.jog.heal -= (10 + e.inimigo[i].lvl - e.jog.percep - e.jog.stam/2) ;
		if (e.jog.heal < 0) e.jog.heal = 0;
	}if (e.inimigo[i].type == 3 ){ // Inimigo do tipo 
		e.jog.heal -= (7 + e.inimigo[i].lvl - e.jog.stam/2 );
		if (e.jog.heal < 0) e.jog.heal = 0;
	}
return e;	
}
/**
\brief Função que inverta um valor negativo num positivo e vice-versa.
*/
int inverte(int x){ 
	if (x<0) 
		return abs(x);
	else     
		x = 0-x;
	return x;
		}

/**
\brief Função que imprime todas as casa afetadas pelo ataque dos inimigos.
*/



void imprime_ataquesafetado (ESTADO e, int i){
	int x,y,raioataquex,raioataquey,posalvox,posalvoy; // Abcissa e ordenada da posição do inimigo, raio de ataque do inimigo em ordem a cada eixo do referencial, abcissa e ordenada da posição alvo
	x = getposinimigox (e, i);
	y = getposinimigoy (e, i);
	if (e.inimigo[i].type == 0 ){ // Inimigo do tipo Xamã 
			for (raioataquey = -3; raioataquey <= 3; raioataquey++){
				raioataquex = raioataquey;
				posalvox = raioataquex+x;
				posalvoy = raioataquey+y;
				if (posicao_valida (posalvox, posalvoy))
				QUADRADOT (posalvox, posalvoy, ESCALA , "#80ff00" , 0.2);
				raioataquex = inverte(raioataquey);
				posalvox = raioataquex+x;
				posalvoy = raioataquey+y;
				if (posicao_valida (posalvox, posalvoy))
				QUADRADOT (posalvox, posalvoy, ESCALA , "#80ff00" , 0.2);
				
				}
			}

	if (e.inimigo[i].type == 1 ){ // Inimigo do tipo Rouge
		for (raioataquex = -1; raioataquex <= 1; raioataquex++)
			for (raioataquey = -1; raioataquey <= 1; raioataquey++){
				posalvox = raioataquex+x;	
				posalvoy = raioataquey+y;
				if (posicao_valida (posalvox, posalvoy))
				QUADRADOT (posalvox, posalvoy, ESCALA , "#ff8000" , 0.2);
				}
			}
	if (e.inimigo[i].type == 2 ){ // Inimigo do tipo Arqueiro
			for (raioataquey = -3; raioataquey <= 3; raioataquey++){
				posalvox = x;
				posalvoy = raioataquey+y;
				if (posicao_valida (posalvox, posalvoy))
				QUADRADOT (posalvox, posalvoy, ESCALA , "#80ff00" , 0.2);
				
				posalvox = x+raioataquey;
				posalvoy = y;
				if (posicao_valida (posalvox, posalvoy))
				QUADRADOT (posalvox, posalvoy, ESCALA , "#80ff00" , 0.2);
				}
			}
	if (e.inimigo[i].type == 3 ){ // Inimigo do tipo Guerreio
		for (raioataquex = -1; raioataquex <= 1; raioataquex++)
			for (raioataquey = -1; raioataquey <= 1; raioataquey++){
				posalvox = raioataquex+x;
				posalvoy = raioataquey+y;
				if (posicao_valida (posalvox, posalvoy))
				QUADRADOT (posalvox, posalvoy, ESCALA , "#0000ff" , 0.2);
				
			}
	}
}
/**
\brief Função que imprime todos os ataques afetados por inimigos.
*/
void imprime_ataquesafetados (ESTADO e){
	int i;
	if (e.showataques == 1)
	for (i = 0; i < e.num_inimigos; i++)
		imprime_ataquesafetado(e,i);
}
/**
\brief Função que verifica se a posicao recebida é menos à atual da matriz, caso seja evoca recursivamente a funcao para as posições à volta
*/
MATRIZ criaposicao(MATRIZ mapa ,ESTADO e, int x1, int x2, int distjog){
	if (posicao_valida(x1,x2)){
	if (distjog <= mapa.map[x1][x2]  ){
		mapa.map[x1][x2] = distjog;
		distjog++;
		int dx,dy;
		int x,y;
		for (dx = -1; dx <= 1; dx++){
					for (dy = -1; dy <= 1; dy++){
						x = x1;
						y = x2;
						x += dx;
						y += dy;
						if (!tem_shop(e,x,y) && !tem_obstaculo(e,x,y) && !tem_saida(e,x,y)  &&  posicao_valida(x,y)  && (distjog < mapa.map[x][y] )) {
							mapa.map[x][y] = distjog;
							mapa = criaposicao( mapa, e, x, y, distjog);
							
						}
					}
		}
	}
}


return mapa;	
}
/**
\brief Função que inicia a matriz
*/

MATRIZ criamatriz (ESTADO e){
	int x,y;
	MATRIZ mapa;
	int valor = 300;
	for (x = 0; x < TAM ; x++)
		for (y = 0; y < TAM; y++)
			mapa.map[x][y] = valor;
	x = getposjogx (e);
	y = getposjogy (e);
	mapa.map[x][y] = 0;
	mapa = criaposicao( mapa,e,x,y,	0);
	return mapa;
}

/**
\brief Função que nos diz qual a melhor jogada a ser executada pelo jogador
*/
POSICAO melhorjogada (ESTADO e, int m, MATRIZ mapa){
	int x,y;
	int dx,dy;
	int gx,gy;
	gx = e.inimigo[m].pos.x;
	gy = e.inimigo[m].pos.y;
	int valor = 300;
	x = getposinimigox (e, m);
	y = getposinimigoy (e, m);
	valor = mapa.map[x][y];	
	for (dx = -1; dx <= 1; dx++)
		for (dy = -1; dy <= 1; dy++){
			x = getposinimigox (e, m);
			y = getposinimigoy (e, m);
			x += dx;
			y += dy;
			if (posicao_valida(x,y) && (mapa.map[x][y] < valor) && (!casa_ocupada_cs (e, x, y))) {
				valor = mapa.map[x][y];
				gx = x;
				gy = y;
			}

	 	}

	e.inimigo[m].pos.x = gx;
	e.inimigo[m].pos.y = gy;	

return e.inimigo[m].pos;
}


/**
\brief Função que altera o pathfinding conforme o tipo do enimigo
*/

MATRIZ alteramatriz(MATRIZ mapabase,int tipo, int x, int y,ESTADO e){
	if ( tipo == 1 || tipo == 3)
		return mapabase;
	else if ( tipo == 0){
		mapabase = criaposicao(mapabase,e,x-3,y-3, -3);
		mapabase = criaposicao(mapabase,e,x+3,y+3, -3);
	}
	else if ( tipo == 2){
		mapabase = criaposicao(mapabase,e,x-3,y, -3);
		mapabase = criaposicao(mapabase,e,x+3,y, -3);
		mapabase = criaposicao(mapabase,e,x,y+3, -3);
		mapabase = criaposicao(mapabase,e,x,y-3, -3);
	}
	return mapabase;
}




/**
\brief Função que faz com que o monstro se mova ou ataque e tem em conta o seu tipo
*/
ESTADO movemonstro (ESTADO e, int i,MATRIZ mapa){
	int x,y;
	int dx,dy;
	int posfinalx,posfinaly;
	int tipo;
	tipo = e.inimigo[i].type;
				
	mapa = alteramatriz (mapa,tipo, getposjogx(e), getposjogy (e),e);	
	x = getposinimigox (e, i);
	y = getposinimigoy (e, i);
	if (tipo == 0 ){ // Inimigo tipo Xamã
			for (dy = -3; dy <= 3; dy++){
				dx = dy;
				posfinalx = dx+x;
				posfinaly = dy+y;
				if (posicao_igualp (e.jog , posfinalx, posfinaly)){
					e = atacajogador(e ,i);
					posfinalx -= e.inimigo[i].pos.x;
					posfinaly -= e.inimigo[i].pos.y;
					ANIMATEATACAIFLAME(i,posfinalx,posfinaly,ESCALA);
					return e;
				}
				dx = inverte(dy);
				posfinalx = dx+x;
				posfinaly = dy+y;
				if (posicao_igualp (e.jog , posfinalx, posfinaly)){
					e = atacajogador(e ,i);
					posfinalx -= e.inimigo[i].pos.x;
					posfinaly -= e.inimigo[i].pos.y;
					ANIMATEATACAIFLAME(i,posfinalx,posfinaly,ESCALA);
					return e;
				}

			}
	posfinalx = e.inimigo[i].pos.x;
	posfinaly = e.inimigo[i].pos.y;
	e.inimigo[i].pos = melhorjogada(e,i,mapa);
	posfinalx = posfinalx -  e.inimigo[i].pos.x;
	posfinaly = posfinaly -  e.inimigo[i].pos.y;
	ANIMATEMOVEI (i,posfinalx,posfinaly,ESCALA);
	}
	
	if (tipo == 1 ){ 
		for (dx = -1; dx <= 1; dx++)
			for (dy = -1; dy <= 1; dy++){
				posfinalx = dx+x;	
				posfinaly = dy+y;
				if (posicao_igualp (e.jog , posfinalx, posfinaly)){
					ANIMATEATACAI(i,dx,dy,ESCALA);
					e = atacajogador(e ,i);
					return e;
				}
			}
			posfinalx = e.inimigo[i].pos.x;
			posfinaly = e.inimigo[i].pos.y;
			e.inimigo[i].pos = melhorjogada(e,i,mapa);
			posfinalx = posfinalx -  e.inimigo[i].pos.x;
			posfinaly = posfinaly -  e.inimigo[i].pos.y;
			ANIMATEMOVEI (i,posfinalx,posfinaly,ESCALA);
				}
	if (tipo == 2 ){ 
			for (dy = -3; dy <= 3; dy++){
				posfinalx = x;
				posfinaly = dy+y;
				if (posicao_igualp (e.jog , posfinalx, posfinaly)){
					e = atacajogador(e ,i);
					posfinalx -= e.inimigo[i].pos.x;
					posfinaly -= e.inimigo[i].pos.y;
					ANIMATEATACAISETA(i,posfinalx,posfinaly,ESCALA);
					return e;
				}
				posfinalx = x+dy;
				posfinaly = y;
				if (posicao_igualp (e.jog , posfinalx, posfinaly)){
					e = atacajogador(e ,i);
					posfinalx -= e.inimigo[i].pos.x;
					posfinaly -= e.inimigo[i].pos.y;
					ANIMATEATACAISETA(i,posfinalx,posfinaly,ESCALA);
					return e;
				

				}
			}
			posfinalx = e.inimigo[i].pos.x;
			posfinaly = e.inimigo[i].pos.y;
			e.inimigo[i].pos = melhorjogada(e,i,mapa);
			posfinalx = posfinalx -  e.inimigo[i].pos.x;
			posfinaly = posfinaly -  e.inimigo[i].pos.y;
			ANIMATEMOVEI (i,posfinalx,posfinaly,ESCALA);
	}
	if (tipo == 3 ){ 
		for (dx = -1; dx <= 1; dx++)
			for (dy = -1; dy <= 1; dy++){
				posfinalx = dx+x;
				posfinaly = dy+y;
				if (posicao_igualp (e.jog , posfinalx, posfinaly)){
					ANIMATEATACAI(i,dx,dy,ESCALA);
					e = atacajogador(e ,i);

					return e;
				}
			}
			posfinalx = e.inimigo[i].pos.x;
			posfinaly = e.inimigo[i].pos.y;
			e.inimigo[i].pos = melhorjogada(e,i,mapa);
			posfinalx -=  e.inimigo[i].pos.x  ;
			posfinaly -=  e.inimigo[i].pos.y  ;
			ANIMATEMOVEI (i,posfinalx,posfinaly,ESCALA);
	}		
return e;
}

/**
\brief Função que efetivamente move os monstros mas tendo em conta o número máximo de inimigos possíveis
*/
ESTADO movemonstros (ESTADO e, MATRIZ mapa){
	int i;
	for (i = 0; i < e.num_inimigos; i++)
		e = movemonstro (e, i, mapa);
return e;
}


/**
\brief Função que executa os ataques feitos pelo jogador, quer seja este à distância ou corpo a corpo
*/
ESTADO rangedatack(ESTADO e,int i){
	e.jog.shuriken--;
	int aux;
	if (e.inimigo[i].type == 0){
		aux = e.inimigo[i].heal - 60 - 10 * e.jog.percep;
		if (aux <= 0){
			e=getexp(e);
			while ( i < e.num_inimigos){
				e.inimigo[i] = e.inimigo[i+1];
				i++;	
			}
			e.num_inimigos--;
			e.enimigosmortos++;
			}
		else e.inimigo[i].heal = aux;
	}
	if (e.inimigo[i].type == 1){
		aux = e.inimigo[i].heal - 80 - 10 * e.jog.percep;
		if (aux <= 0){
			e=getexp(e);

			while ( i < e.num_inimigos){

				e.inimigo[i] = e.inimigo[i+1];
				i++;	
			}
			e.num_inimigos--;
			e.enimigosmortos++;
			}
		else e.inimigo[i].heal = aux;

	}
	if (e.inimigo[i].type == 2){
		aux = e.inimigo[i].heal - 40 - 10 * e.jog.percep;
		if (aux <= 0){
			e=getexp(e);

			while ( i < e.num_inimigos){
				e.inimigo[i] = e.inimigo[i+1];
				i++;	
			}
			e.num_inimigos--;
			e.enimigosmortos++;
			}
		else e.inimigo[i].heal = aux;


	}
	if (e.inimigo[i].type == 3){
		aux = e.inimigo[i].heal - 20 - 5 * e.jog.percep;
		if (aux <= 0){
			e=getexp(e);

			while ( i < e.num_inimigos){
				e.inimigo[i] = e.inimigo[i+1];
				i++;	
			}
			e.num_inimigos--;
			e.enimigosmortos++;
			}
		else e.inimigo[i].heal = aux;

	}
	return e;
}

/**
\brief Função que imprime o clicável para permitir o ataque à distância
*/
void printbatleranged (int x, int y, int h){
	char link[MAX_BUFFER];
				sprintf(link, "http://localhost/cgi-bin/dibs?range_%i", h);
			    ABRIR_LINK(link);
				imprime_casa_transparente(x, y);
				FECHAR_LINK;
}


/**
\brief Função que a posição do jogador com todos os inimigos
*/
int comparejogenyvar (ESTADO e, int x, int y){
	int h;
	for (h=0; h < e.num_inimigos;h++){
		if ( x == getposinimigox(e,h) && y == getposinimigoy(e,h))
			return h;	
	}
	return -1;
}


/**
\brief Função que cria os clicaveis para os ataques ao longe do jogador
*/
void batalharanged (ESTADO e){
	if(e.jog.shuriken != 0 )
	{
		int x,y,dibs,a,b;
		int range = 3;  	
		x = getposjogx(e) ;
		y = getposjogy(e) ;
		a = x - 3;
		b = y - 3;
		for (x = a ; x < (a+range+range+1) ;x++){
			for (y=b ; y < (b+range+range+1); y++){
				if (posicao_valida ( x ,y)){
					printf("x=%i y =%i\n",x,y);
					dibs =(comparejogenyvar ( e, x,y) );
					if  (dibs >= 0)
						printbatleranged (x, y, dibs);
				}
			}	
		}
	}
}


/**
\brief Função que imprime as repetivas casas
*/
void imprime_casa(int x, int y) {
	IMAGEM(x, y,ESCALA, "Ground1.png");
}



/**
\brief Função que inicializa o inimigo, atribuindo-lhe uma posição random e verifica se essa casa se encontra ocupada ou não.
*/
ESTADO inicializar_inimigo (ESTADO e) {
	char x; char y;char z;
	z = random() % 4;

	do {
	x = random() % TAM;
	y = random() % TAM; /* gerar posiçoes pseudo aleatorias para x e y*/
	} while (casa_ocupada_cs (e, x, y));

	e.inimigo[(int) e.num_inimigos].pos.x = x;
	e.inimigo[(int) e.num_inimigos].pos.y = y;
	e.inimigo[(int) e.num_inimigos].type  = z;
	e.inimigo[(int) e.num_inimigos].lvl  = e.nivel;
	e.inimigo[(int) e.num_inimigos].heal = 100;	
	e.num_inimigos++;
	return e;
}

/**
\brief Função que inicializa todos os inimigos, até esta atingir o número máximo de inimigos possíveis a conter no mapa
*/
ESTADO inicializar_inimigos (ESTADO e, int num_inimigos){
	int i;
	for (i = 0; i < num_inimigos; i++)
		e = inicializar_inimigo (e);
	return e;
}

/**
\brief Função que inicializa o obstáculo, atribuindo-lhe uma posição random e verifica se essa casa se encontra ocupada ou não.
*/
ESTADO inicializar_obstaculo (ESTADO e) {
	char x; char y;
	do { 
	x = random() % TAM;
	y = random() % TAM;
	} while (casa_ocupada_cs (e, x, y));

	e.obstaculo[(int) e.num_obstaculos].pos.x = x;
	e.obstaculo[(int) e.num_obstaculos].pos.y = y;	
	e.num_obstaculos++;
	return e;
}

/**
\brief Função que inicializa todos os obstáculos, até esta atingir o número máximo de obstáculos possíveis a conter no mapa
*/
ESTADO inicializar_obstaculos (ESTADO e, int num_obstaculos){
	int i;
	for (i = 0; i < num_obstaculos; i++)
		e = inicializar_obstaculo (e);
	return e;
}

/**
\brief Função que inicializa o pote, atribuindo-lhe uma posição random e verifica se essa casa se encontra ocupada ou não.
*/

ESTADO inicializar_pote (ESTADO e,int i) {

	char x; char y;char t;
	do {
	x = random() % TAM;
	y = random() % TAM;
	} while ( tem_pot(e,x,y) > 0|| casa_ocupada_cs (e, x, y)  );

	e.pot[i].pos.x = x;
	e.pot[i].pos.y = y;
	t = random() % 2; 
	e.pot[i].type = t;
	e.num_pot ++;
	return e;
}


/**
\brief Função que inicializa todos os potes, até esta atingir o número máximo de potes possíveis a conter no mapa
*/

ESTADO inicializar_pot (ESTADO e, int num_pot){
	int i;
	for (i = 0; i < num_pot; i++)
		e = inicializar_pote (e,i);
	return e;
}


/**
\brief Função que gera a posição da loja e que verifica se esta se encontra ocupada e se tem um pote respetivamente
*/
ESTADO inicializar_shop(ESTADO e){
	do {
	e.shop.x = random() % TAM;
	e.shop.y = random() % TAM;
	} while (casa_ocupada_cs (e, e.shop.x, e.shop.y) || tem_pot(e,e.shop.x,e.shop.y) > 0);
	return e;
}



/**
\brief Função que inicializa o jogo no nivel 0
*/
ESTADO inicializarlvl0() {
	ESTADO e;
	memset(&e,0,sizeof(ESTADO));
	e.jog.max_heal = 100;
	e.jog.max_mana = 100;
	e.jog. str=0;
	e.jog. percep=0;
	e.jog. stam=0;
	e.jog. spiri=0;
	e.jog. lvl=1;
	e.jog. exp=0;
	e.jog.expboost = 1;
	e.nivel = 1;
	e.jog. lvlupstats=0;
	e.estadojog= 0;	
	e.jog.heal = 100;
	e.jog.mana = 100;
	e.jog.shuriken  =  10;
	e.jog.pos.x = random() % TAM;
	e.jog.pos.y = random() % TAM;

	do {
		e.saida.x = random() % TAM;
		e.saida.y = random() % TAM;
	} while(posicao_igualp
		(e.jog, e.saida.x, e.saida.y));

	e = inicializar_inimigos (e, e.nivel + 2);
	e = inicializar_obstaculos (e, 30);
	e = inicializar_pot (e, 3);

	return e;
}

/**
\brief Função que inicializa o jogo num determinado nivel
*/
ESTADO inicializar(ESTADO antigo) {
	ESTADO e;
	memset(&e,0,sizeof(ESTADO));
	e.enimigosmortos = antigo.enimigosmortos;
	e.nivel = antigo.nivel;
	e.jog.exp = antigo.jog.exp;
	e.jog = antigo.jog;
	e.jog.heal = e.jog.max_heal;
	e.jog.mana = e.jog.max_mana;
	e.estadojog = 1;
			if (e.jog.shuriken + 10 > 100)
			e.jog.shuriken = 90;
	e.jog.shuriken  += + 10;
	do {
		e.saida.x = random() % TAM;
		e.saida.y = random() % TAM;
	} while(posicao_igualp
		(e.jog, e.saida.x, e.saida.y));

	e = inicializar_inimigos (e, e.nivel + 2);
	e = inicializar_obstaculos (e, 30);
	e = inicializar_pot (e, 3);
	if (e.nivel % 5 == 0)
		e=inicializar_shop(e);

	return e;
}

/**
\brief Função que faz com que o jogador ataque e retire vida ao monstro
*/
ESTADO atacamonstro(ESTADO e, int i){
	int aux;
	if (e.inimigo[i].type == 0){ 
		aux = e.inimigo[i].heal - 80 - 10*e.jog.str;
		if (aux <= 0){
			e=getexp(e);

			while ( i < e.num_inimigos){
				e.inimigo[i] = e.inimigo[i+1];
				i++;		
			}
			e.num_inimigos--;
			e.enimigosmortos++;
		}
		else  e.inimigo[i].heal = aux;
	}
	if (e.inimigo[i].type == 1){ 
		aux = e.inimigo[i].heal - (80+10*e.jog.str);
		if (aux <= 0){
			e=getexp(e);

			while ( i < e.num_inimigos){
				e.inimigo[i] = e.inimigo[i+1];
				i++;	
			}
			e.num_inimigos--;
			e.enimigosmortos++;
		}
		else  e.inimigo[i].heal = aux;


	}
	if (e.inimigo[i].type == 2){ 
		aux = e.inimigo[i].heal - (60+10*e.jog.str);
		if (aux <= 0){
			e=getexp(e);

			while ( i < e.num_inimigos){
				e.inimigo[i] = e.inimigo[i+1];
				i++;	
			}
			e.num_inimigos--;
			e.enimigosmortos++;
		}
		else  e.inimigo[i].heal = aux;


	}
	if (e.inimigo[i].type == 3){ 
		aux = e.inimigo[i].heal - (40+10*e.jog.str);

		if (aux <= 0){
			e=getexp(e);

			while ( i < e.num_inimigos){
				e.inimigo[i] = e.inimigo[i+1];
				i++;
				}	
			e.num_inimigos--;
			e.enimigosmortos++;
		}
		else  e.inimigo[i].heal = aux;


	}		
	return e;
}

/**
\brief Função que imprime os clicaveis para os ataques corpo a corpo do jogador
*/
void mellebattle (ESTADO e, int x, int y, int c){
	int i;
	char link[MAX_BUFFER];
		for (i = 0; i < e.num_inimigos; i++){
			if (getposinimigox (e, i) == x && getposinimigoy (e, i) == y ){

				sprintf(link, "http://localhost/cgi-bin/dibs?melle_%i", i);
			    ABRIR_LINK(link);
			    if (c == 0){
				imprime_casa_transparente(x, y);
				}
			else QUADRADOT(x, y,ESCALA, "#982354",0.2);
				FECHAR_LINK;
		}
	}   
}


/**
\brief Função que imprime o clicavel para um dado movimento do jogador
*/
void imprime_movimento(ESTADO e, int dx, int dy,int c) {
	int x = getposjogx(e) + dx;
	int y = getposjogy(e) + dy;
	char link[MAX_BUFFER];
	if(!posicao_valida(x, y))
		return;
	if (tem_inimigo (e, x, y))
		mellebattle(e, x, y,c);
	if (casa_ocupada (e, x, y))
		return;
	if (tem_shop (e,x,y))
		sprintf(link, "http://localhost/cgi-bin/dibs?shop");

	else	sprintf(link, "http://localhost/cgi-bin/dibs?mov_%i_%i", x,y);	


		ABRIR_LINK(link);
		if (c == 0){
				imprime_casa_transparente(x, y);
				}
		else {QUADRADOT(x, y,ESCALA, "#982354",0.2);
}
		FECHAR_LINK;
}


/**
\brief Função que imprime os clicaveis para os movimentos do jogador
*/
void imprime_movimentos(ESTADO e) {
	int dx; int dy;
	if (e.jog.doublejump == 0){
			for (dx = -1; dx <= 1; dx++)
				for (dy = -1; dy <= 1; dy++)
					if (dx != 0 || dy != 0)
						imprime_movimento(e, dx, dy,0);}
	else {for (dx = -2; dx <= 2; dx++)
				for (dy = -2; dy <= 2; dy++)
					if (dx != 0 || dy != 0)
						if(dx == 2 || dx == -2 || dy == -2 || dy == 2)
						imprime_movimento(e, dx, dy,1);
		}
}

/**
\brief Função que imprime o jogador
*/

void imprime_jogador(ESTADO e) {
	IMAGEMJ(getposjogx(e), getposjogy(e), ESCALA, "Jog.png");
	imprime_movimentos(e);
}

/**
\brief Função que imprime o saida
*/
void imprime_saida(ESTADO e) {
	IMAGEM(e.saida.x, e.saida.y, ESCALA, "Hole.png");

}

/**
\brief Função que imprime o loja
*/
void imprime_shop(ESTADO e){
	if (e.nivel % 5 == 0)
	IMAGEM(e.shop.x, e.shop.y, ESCALA, "mercador.png");

}

/**
\brief Função que compara strings
*/
int strgigual (char *x1 , char *x2){
	int i;
	for (i=0; (x1[i] != '\0' && x2[i] != '\0') ; i++){
		if (x1[i] != x2[i] ) 
			return 0;
	}
	return 1;
}

/**
\brief Função que faz com que ao clicar na saída faz com que se passe de nivel e gera um tabuleiro aleatório
*/
ESTADO check_for_exit(ESTADO e) {
	
		e.nivel++;	
		e = inicializar(e);
	
	return e;
}

/**
\brief Função que altera o estado conforme o argumento que recebe no link
*/

ESTADO decodelink(char *args, ESTADO e){
printf("%s\n",(estado2str (e)) );
	char filename[1024];
	int argx,argy;
	sscanf(args,"%[^_]_%d_%d",filename,&argx,&argy);
	if (strgigual (args, "comprarboostexp")){
		if (e.jog.gold >= 1000){
		e.jog.expboost = 2;
		e.jog.gold -= 1000;
		}
	}

	if (strgigual (args, "comprarexp")){
		if(e.jog.gold >= 500){
		e.jog.lvl++;
		e.jog.lvlupstats++;
		e.jog.gold-=500;
		}
	}
	if (strgigual (args, "learnfrezze")){
		if(e.jog.gold >= 500){
		e.jog.frezzespell++;
		e.jog.gold-=500;
		}
	}

	if (strgigual (args, "learnview")){
		if(e.jog.gold >= 100){
		e.jog.viewspell++;
		e.jog.gold-=100;
		}
	}

	if (strgigual (args, "learnobs")){
		if(e.jog.gold >= 100){
		e.jog.obsspell++;
		e.jog.gold-=100;
		}
	}

	if (strgigual (args, "learndouble")){
		if(e.jog.gold >= 500){
		e.jog.doublespell++;
		e.jog.gold-=500;
		}
	}

	if (strgigual (args, "comprarmana")){
		if (e.jog.gold >= 100){
		e.jog.manapot++;
		e.jog.gold -= 100;
		}
	}
		if (strgigual (args, "comprarhealt")){
		if (e.jog.gold >= 100){
		e.jog.healpot++;
		e.jog.gold -= 100;
		}
	}
	if (strgigual (args, "comprarscore")){
			if (e.jog.gold >= 100){
		e.jog.healpot++;
		e.jog.gold -= 100;
		}
	}


	if (strgigual (args, "mov")){
		if (e.jog.doublejump == 1)
			e.jog.doublejump--;
		if (e.showataques == 1)
			e.showataques--;
		int i;
		int a,b;
		int x, y;
		x = argx;
		y = argy;
		i = tem_pot (e,x,y);
			if (i >= 0){
				if (e.pot[i].type == 0){
					e.jog.manapot ++;	}
					else {e.jog.healpot++;}
				while ( i < e.num_pot){
				e.pot[i] = e.pot[i+1];
				i++;
				}	
			e.num_pot--;
		}
					
		a = e.jog.pos.x;
		b = e.jog.pos.y;
		a = a -  x;
		b = b -  y;
		printf("%i %i\n",a,b );

		if (x == e.saida.x && y == e.saida.y)
			return check_for_exit(e);	
		ANIMATEMOVEJOG (a,b,ESCALA);
		e.jog.pos.x = x;
		e.jog.pos.y = y;
		if (e.frezze){
			e.frezze--;
			return e;}
		MATRIZ mapa;
		mapa = criamatriz(e);
		e = movemonstros (e, mapa);
		
	}
	if (strgigual (args, "manapot")){
		e.jog.manapot--;
		e.jog.mana = e.jog.max_mana;
	}
	if (strgigual (args, "healpot")){
		e.jog.healpot--;
		e.jog.heal = e.jog.max_heal;
	}
	if (strgigual (args, "range")){
		if (e.jog.doublejump == 1)
			e.jog.doublejump--;
		if (e.showataques == 1)
			e.showataques--;
		int x;
		sscanf(args,"%[^_]_%d",filename,&x);
		int a,b;
		a = getposinimigox(e,x);
		b = getposinimigoy(e,x);
		a -= e.jog.pos.x;
		b -= e.jog.pos.y;
		e=rangedatack(e,x);
		ANIMATESHURIKEN(a,b,ESCALA);


		if (e.frezze){
			e.frezze--;
			return e;}
		MATRIZ mapa;
		mapa = criamatriz(e);
		e = movemonstros (e, mapa);

		
		}			
	if (strgigual (args, "skillz")){
		if( e.jog.mana >= 20){
			if (e.showataques == 0){
				e.showataques++;
				e.jog.mana -= 20;}
		}
	}
	if (strgigual (args, "shop")){
			if (e.loja == 0){
				e.loja++;}
			else e.loja--;
	}
	if (strgigual (args, "scores")){
			if (e.higthscores == 0){
				e.higthscores++;}
			else e.higthscores--;
	}
	if (strgigual (args, "djenable")){
		if( e.jog.mana >= 40){
			if (e.jog.doublejump == 0){
				e.jog.doublejump++;
				e.jog.mana -= 40;
				}
			else {e.jog.doublejump--; 
			e.jog.mana += 40;
			}
		}
		else {if (e.jog.doublejump == 1)
			{e.jog.doublejump--; 
			e.jog.mana += 40;
			}
		}
		
	}
	if (strgigual (args, "melle")){
		if (e.jog.doublejump == 1)
			e.jog.doublejump--;
		if (e.showataques == 1)
			e.showataques--;
		int x;
		sscanf(args,"%[^_]_%d",filename,&x);
		int a,b;
		a = getposinimigox(e,x);
		b = getposinimigoy(e,x);
		a -= e.jog.pos.x;
		b -= e.jog.pos.y;

		ANIMATEATACAJOG (a,b,ESCALA);
		e = atacamonstro (e, x);
		if (e.frezze){
			e.frezze--;
			return e;}
		MATRIZ mapa;
		mapa = criamatriz(e);
		e = movemonstros (e, mapa);
					
	} 	
	if (strgigual (args, "frezze")){
		if( e.jog.mana >= 20){
			if (e.frezze == 0){
				e.frezze++;
				e.jog.mana -= 20;
			}
		}
	}	
	if (strgigual (args, "obs")){
		if( e.jog.mana >= 20){
			if (e.jog.obs == 0){
				e.jog.obs++;
				e.jog.mana -= 20;
				}
			else {e.jog.obs--; 
			e.jog.mana += 20;
			}
		}
		else{
			if (e.jog.obs == 1)
				{e.jog.obs--; 
				e.jog.mana += 20;
			}
		}
	}
	
	if (strgigual (args, "pobs")){
		int x;
		e.jog.obs--;
		sscanf(args,"%[^_]_%d",filename,&x);
		e.obstaculo[x].type = 1;
	}

	if (strgigual (args, "lvlupstr")){
		e.jog.str++;
		e.jog.lvlupstats--;
	}
	if (strgigual (args, "lvluppercep")){
		e.jog.percep++;
		e.jog.lvlupstats--;
	}
	if (strgigual (args, "lvlupstam")){
		e.jog.stam++;
		e.jog.max_heal+=10;
		e.jog.heal+=10;
		e.jog.lvlupstats--;
	}
	if (strgigual (args, "lvlupspiri")){
		e.jog.spiri++;
		e.jog.max_mana+=10;
		e.jog.mana +=10;
		e.jog.lvlupstats--;
	}

	if (strgigual (args, "vendermanapot")){
		if (e.jog.manapot >= 1){
		e.jog.manapot--;
		e.jog.gold += 50;
		}
	}
	if (strgigual (args, "venderhealtpot")){
		if (e.jog.healpot >= 1){
		e.jog.healpot--;
		e.jog.gold += 50;
		}
	}
	if (strgigual (args, "jogar")){
		e.estadojog=1;
	}

	return e;
}


/**
\brief Função que lê e atualiza o estado de jogo
*/

ESTADO ler_estado(char *args) {
	int i;
	ESTADO e;
	memset(&e,0,sizeof(ESTADO));
	if(strlen(args) == 0){
		e = inicializarlvl0();
	}
	
	else{

	gameinfo = fopen("/tmp/gameinfo", "r");
	i = fread(&e, sizeof(ESTADO), 1,gameinfo);
		if( i!= 0){
		e = decodelink(args , e);
		fclose(gameinfo);
		}
	}

	gameinfo = fopen("/tmp/gameinfo", "w");
	fwrite(	&e, sizeof(ESTADO), 1, gameinfo);
	fclose(gameinfo);	
	return e;

}

/**
\brief Função que imprime os inimgos no tabuleiro
*/
void imprime_inimigos(ESTADO e) {
	int i; 
	for(i = 0; i < e.num_inimigos; i++){
		int j;
		j = e.inimigo[i].type;
		if (j == 1) IMAGEMI(getposinimigox (e,i), getposinimigoy (e,i), ESCALA, "Monstro2.png",i );  
		if (j == 2) {IMAGEMI(getposinimigox (e,i), getposinimigoy (e,i), ESCALA, "Monstro3.png",i );
					IMAGEMATARange(getposinimigox (e,i), getposinimigoy (e,i), ESCALA, "Shuriken2.png",i );					}  
		if (j == 3) IMAGEMI(getposinimigox (e,i), getposinimigoy (e,i), ESCALA, "Monstro4.png",i );
		if (j == 0) {IMAGEMI(getposinimigox (e,i), getposinimigoy (e,i), ESCALA, "Monstro1.png",i ); 
					IMAGEMATAFLAME(getposinimigox (e,i), getposinimigoy (e,i), ESCALA, "Fireball.png",i );					}
		if (e.frezze)  IMAGEMT(getposinimigox (e,i), getposinimigoy (e,i), ESCALA, "Congelado.png",0.5 );
	}
}

/**
\brief Função que imprime os obstáculos no tabuleiro
*/
void imprime_obstaculos(ESTADO e) {
	int i;
	if (e.jog.obs == 0){
		for(i = 0; i < e.num_obstaculos; i++){
			if (e.obstaculo[i].type == 0)
			IMAGEM(e.obstaculo[i].pos.x, e.obstaculo[i].pos.y, ESCALA, "Lava_Obsidian_2.png");
			else
			IMAGEM(e.obstaculo[i].pos.x, e.obstaculo[i].pos.y, ESCALA, "Lava_Obsidian_1.png");
		}
	}
	else {
	char link[MAX_BUFFER];
		for(i = 0; i < e.num_obstaculos; i++){
			if (e.obstaculo[i].type == 0){
			IMAGEM(e.obstaculo[i].pos.x, e.obstaculo[i].pos.y, ESCALA, "Lava_Obsidian_2.png");
			sprintf(link, "http://localhost/cgi-bin/dibs?pobs_%i", i);	
			ABRIR_LINK(link);
			QUADRADOT(e.obstaculo[i].pos.x , e.obstaculo[i].pos.y ,ESCALA, "#982354",0.5);
			FECHAR_LINK;}
			
			else
			IMAGEM(e.obstaculo[i].pos.x, e.obstaculo[i].pos.y, ESCALA, "Lava_Obsidian_1.png");
			
		}
	}
}


/**
\brief Função que calcula e grava o score no ficheiro
*/
void metescore (ESTADO e){
		//ABRIR_SVG(1200, 600);
	int scores = (e.nivel * 10 + e.enimigosmortos);
	printf("<image x=%d y=%d width=%d height=%d xlink:href=%s />\n", 0,0, 1200, 600, IMAGE_PATH "YOUDIED.png");
	printf ("<text x=%d y=%d style=fill:orange;>Score:%i  </tspan>  <tspan x=%d y=%d>Level = %i</tspan> <tspan x=%d y=%d>Killed enemies = %i</tspan>  </text> ",  70, 120, scores, 80, 140, e.nivel , 80, 160 ,e.enimigosmortos);
		ABRIR_LINK ("http://localhost/cgi-bin/dibs");
			printf("<image x=%d y=%d width=%d height=%d xlink:href=%s />\n", 800,400, 300, 300, IMAGE_PATH "NEVERDIE.png");
		FECHAR_LINK;


	score = fopen("/tmp/score", "r");
	if (!score){
		score = fopen("/tmp/score", "w+");
		fprintf(score,"%i-%i-%i-%i",scores,0,0,0 );
		printf ("<text x=%d y=%d style=fill:red;>HIGTHSCORE, CONSULTAR LISTA  </text> ", 100 ,100);
	}
	else{
	int fst,snd,trd,fourt;
	char ficheiro[500];
		while(fgets(ficheiro,sizeof ficheiro,score)!= NULL)
{}		

		sscanf(ficheiro, "%d-%d-%d-%d", &fst,&snd,&trd,&fourt );
		score = fopen ("/tmp/score", "w+");
		if (scores > fst) {
						fprintf(score, "%i-%i-%i-%i-", scores,fst,snd,trd);
		printf ("<text x=%d y=%d style=fill:red;>HIGTHSCORE, CONSULTAR LISTA  </text> ", 100 ,100);

					}else if (scores > snd) {
						fprintf(score, "%i-%i-%i-%i-", fst,scores,snd,trd);
		printf ("<text x=%d y=%d style=fill:red;>HIGTHSCORE, CONSULTAR LISTA  </text> ", 100 ,100);

					}else if (scores > trd) {
		printf ("<text x=%d y=%d style=fill:red;>HIGTHSCORE, CONSULTAR LISTA  </text> ", 100 ,100);
						
						fprintf(score, "%i-%i-%i-%i-", fst,snd,scores,trd);
					}else if (scores > fourt) {
		printf ("<text x=%d y=%d style=fill:red;>HIGTHSCORE, CONSULTAR LISTA  </text> ", 100 ,100);
						fprintf(score, "%i-%i-%i-%i-", fst,snd,trd,scores);
					}else 			    	
						fprintf(score, "%i-%i-%i-%i-", fst,snd,trd,fourt);
				}
	fclose(score);

}
/**
\brief Função que as skills do jogador, quer disponíveis quer indisponíveis
*/
void imprime_skillz(ESTADO e){
	char link[MAX_BUFFER];
	

	printf ("<text x=%d y=%d style=fill:black;>SCORES:   </text> ", 1400 ,100);
	sprintf(link, "http://localhost/cgi-bin/dibs?scores");
	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 1400, 140, 75, 75, "#f58000", "#000000" , IMAGE_PATH "X.png");
	FECHAR_LINK;


	sprintf(link, "http://localhost/cgi-bin/dibs?manapot");
	printf ("<text x=%d y=%d style=fill:black;>MANAPOT:   </text> ", 1200 ,590);
	if (e.jog.manapot > 0){
	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 1200, 600, 75, 75, "#f58000", "#000000" , IMAGE_PATH "manapot.png");
	FECHAR_LINK;
}
	else {
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 1200, 600, 75, 75, "#f58000", "#000000" , IMAGE_PATH "manapotempty.png");
	}

	printf ("<text x=%d y=%d style=fill:white;>%i   </text> ", 1245 ,660,e.jog.manapot);


	sprintf(link, "http://localhost/cgi-bin/dibs?healpot");
	printf ("<text x=%d y=%d style=fill:black;>HEALPOT:   </text> ", 1200 ,700);
	if (e.jog.healpot > 0){
	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 1200, 710, 75, 75, "#f58000", "#000000" , IMAGE_PATH "healpot.png");
	FECHAR_LINK;
}
	else {
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 1200, 710, 75, 75, "#f58000", "#000000" , IMAGE_PATH "manapotempty.png");
	}

	printf ("<text x=%d y=%d style=fill:white;>%i   </text> ", 1245 ,770,e.jog.healpot);



	if (e.jog.doublespell == 1){
	sprintf(link, "http://localhost/cgi-bin/dibs?djenable");
	printf ("<text x=%d y=%d style=fill:black;>DOUBLE JUMP:   </text> ", 908 ,440);
	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 908, 450, 75, 75, "#f58000", "#000000" , IMAGE_PATH "Double.png");
	FECHAR_LINK;}
	else {
	printf ("<text x=%d y=%d style=fill:black;>Not learned:   </text> ", 908 ,440);

	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 908, 450, 75, 75, "#f58000", "#000000" , IMAGE_PATH "Doubleoff.png");

	}

	if (e.jog.viewspell == 1){	
	printf ("<text x=%d y=%d style=fill:black;>VIEW:   </text> ", 908 ,290);
	sprintf(link, "http://localhost/cgi-bin/dibs?skillz");
	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 908, 300, 75, 75, "#f58000", "#000000" , IMAGE_PATH "Skillz.png");
	FECHAR_LINK;
	}

	else{
	printf ("<text x=%d y=%d style=fill:black;>Not learned:   </text> ", 908 ,290);

	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 908, 300, 75, 75, "#f58000", "#000000" , IMAGE_PATH "Skillzoff.png");

	}

	if (e.jog.frezzespell == 1){
	sprintf(link, "http://localhost/cgi-bin/dibs?frezze");
	printf ("<text x=%d y=%d style=fill:black;>FREZZE:   </text> ", 908 ,590);
	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 908, 600, 75, 75, "#f58000", "#000000" , IMAGE_PATH "Frezze.png");
	FECHAR_LINK;
	}
	else{
	printf ("<text x=%d y=%d style=fill:black;>Not learned:   </text> ", 908 ,590);
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 908, 600, 75, 75, "#f58000", "#000000" , IMAGE_PATH "Frezzeoff.png");
	}
	if (e.jog.obsspell == 1){
	sprintf(link, "http://localhost/cgi-bin/dibs?obs");
	printf ("<text x=%d y=%d style=fill:black;>OBS HELP:   </text> ", 908 ,700);
	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 908, 710, 75, 75, "#f58000", "#000000" , IMAGE_PATH "obshelp.png");
	FECHAR_LINK;
	}
	else{
	printf ("<text x=%d y=%d style=fill:black;>Not learned:   </text> ", 908 ,700);
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 908, 710, 75, 75, "#f58000", "#000000" , IMAGE_PATH "obshelpoff.png");

	}
}
/**
\brief Função que imprime os stats do jogador
*/
void imprime_stats(ESTADO e){
	printf ("<text x=%d y=%d style=fill:red;>HP:%i   </text> ", 902 ,60 ,e.jog.heal);
	printf ("<text x=%d y=%d style=fill:blue;>MANA:%i   </text> ", 902 ,140 ,e.jog.mana);
	printf ("<text x=%d y=%d style=fill:grey;>SHURIKENS:%i   </text> ", 902 ,250 ,e.jog.shuriken);


	printf ("<text x=%d y=%d style=fill:black;>LVL:%i   </text> ", 1200 ,200 ,e.jog.lvl);
	printf ("<text x=%d y=%d style=fill:black;>EXP:%i / %i  </text> ", 1200 ,225 ,e.jog.exp,e.jog.lvl*e.jog.lvl);
	printf ("<text x=%d y=%d style=fill:black;>Stats to lvl up:%i   </text> ", 1200 ,250 ,e.jog.lvlupstats);
	printf ("<text x=%d y=%d style=fill:black;>NIVEL ATUAL:%i   </text> ", 1200 ,150 ,e.nivel);

	printf ("<text x=%d y=%d  style=fill:black;>STR:%i   </text> ", 1200 ,353 ,e.jog.str);
	printf ("<text x=%d y=%d  style=fill:black;>PERCEP:%i   </text> ", 1200 ,381 ,e.jog.percep);
	printf ("<text x=%d y=%d  style=fill:black;>STAM:%i   </text> ", 1200 ,409 ,e.jog.stam);
	printf ("<text x=%d y=%d  style=fill:black;>SPIRI:%i   </text> ", 1200 ,437 ,e.jog.spiri);
	printf ("<text x=%d y=%d  style=fill:black;>GOLD:%i   </text> ", 1200 ,500 ,e.jog.gold);



	if (e.jog.lvlupstats > 0)
	{
	char link[MAX_BUFFER];
	if (e.jog.str < 10){
	sprintf(link, "http://localhost/cgi-bin/dibs?lvlupstr");
	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d stroke=%s stroke-width='2' xlink:href=%s />\n", 1325, 340, 20, 20,  "#000000" , IMAGE_PATH "aumenta.png");
	FECHAR_LINK;

}
	if (e.jog.percep < 10){
	sprintf(link, "http://localhost/cgi-bin/dibs?lvluppercep");
	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d stroke=%s stroke-width='2' xlink:href=%s />\n", 1325, 365, 20, 20,  "#000000" , IMAGE_PATH "aumenta.png");
	FECHAR_LINK;
}
	if (e.jog.stam < 10){
	sprintf(link, "http://localhost/cgi-bin/dibs?lvlupstam");
	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d stroke=%s stroke-width='2' xlink:href=%s />\n", 1325, 390, 20, 20,  "#000000" , IMAGE_PATH "aumenta.png");
	FECHAR_LINK;
}
	if (e.jog.spiri < 10){
	sprintf(link, "http://localhost/cgi-bin/dibs?lvlupspiri");
	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d stroke=%s stroke-width='2' xlink:href=%s />\n", 1325, 415, 20, 20,  "#000000" , IMAGE_PATH "aumenta.png");
	FECHAR_LINK;
}


		}



	printf("<rect x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2'/>\n", 915, 80, e.jog.heal , 30, "#ff0000","#000000");
	printf("<rect x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2'/>\n", 915, 160, e.jog.mana  , 30, "#0000ff","#000000");


}

/**
\brief Função que grava e imprime os scores do jogo
*/
void imprimescores( ESTADO e){
	QUADRADO(0,0,900,"#yjyjhff");
	char link[MAX_BUFFER];
	score = fopen("/tmp/score", "r");

	int fst,snd,trd,fourt;
	char ficheiro[500];
	if (score){
		while(fgets(ficheiro,sizeof ficheiro,score)!= NULL)
{}		

		sscanf(ficheiro, "%d-%d-%d-%d", &fst,&snd,&trd,&fourt );
	fclose(score);
	printf ("<text x=%d y=%d style=fill:white;>FIRST = %i:   </text> ", 350 ,100,fst);
	printf ("<text x=%d y=%d style=fill:white;>SECOND = %i:   </text> ", 350 ,300,snd);
	printf ("<text x=%d y=%d style=fill:white;>THIRD = %i:   </text> ", 350 ,500,trd);
	printf ("<text x=%d y=%d style=fill:white;>FOURTH = %i:   </text> ", 350 ,700,fourt);

	}
	else {
	printf ("<text x=%d y=%d style=fill:white;>NO SCORES    </text> ", 350 ,100);
	}
	if (e.estadojog)
	printf ("<text x=%d y=%d style=fill:black;>SCORES:   </text> ", 1400 ,100);
	sprintf(link, "http://localhost/cgi-bin/dibs?scores");
	ABRIR_LINK(link);
	if (e.estadojog)
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 1400, 140, 75, 75, "#f58000", "#000000" , IMAGE_PATH "X.png");
	else 
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 800, 800, 100, 100, "#f58000", "#000000" , IMAGE_PATH "X.png");

	FECHAR_LINK;
}

/**
\brief Função que imprime os potes de mana e de heal conforme o seu tipo
*/
void imprime_potes(ESTADO e){
	int i;
	for (i = 0; i< e.num_pot; i++){
		if (e.pot[i].type == 0)
		IMAGEM(e.pot[i].pos.x, e.pot[i].pos.y, ESCALA, "manapot.png");
	else
		IMAGEM(e.pot[i].pos.x, e.pot[i].pos.y, ESCALA, "healpot.png");

	}
}


/**
\brief Função que imprime a loja e permite escolher os items respetivos conforme o gold que o jogador contenha
*/
void imprimeloja (ESTADO e){
	char link[MAX_BUFFER];
	QUADRADO(0,0,900,"#yjyjhff");
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 477, 0, 500, 500, "#f58000", "#000000" , IMAGE_PATH "welele.png");
	sprintf(link, "http://localhost/cgi-bin/dibs?shop");
	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 800, 800, 100, 100, "#f58000", "#000000" , IMAGE_PATH "X.png");
	FECHAR_LINK;
	printf ("<text x=%d y=%d style=fill:white;>Sair loja:   </text> ", 800 ,800);


	printf ("<text x=%d y=%d style=fill:white;>EXP POT: Aumenta em 1 nivel o player - 500 gold  </text> ", 100 ,750);

	sprintf(link, "http://localhost/cgi-bin/dibs?comprarexp");
	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 0, 700, 100, 100, "#f58000", "#000000" , IMAGE_PATH "exppot.png");
	FECHAR_LINK;

	if (e.jog.frezzespell != 1){
		sprintf(link, "http://localhost/cgi-bin/dibs?learnfrezze");
		ABRIR_LINK(link);
		printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 5, 200, 100, 80, "#f58000", "#000000" , IMAGE_PATH "Frezze.png");
		FECHAR_LINK;
		printf ("<text x=%d y=%d style=fill:white;>FREZZE SPELL: Aprende o poder  - 500 gold  </text> ", 100 ,250);
	}
	else{
		printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 5, 200, 100, 80, "#f58000", "#000000" , IMAGE_PATH "Frezzeoff.png");
		printf ("<text x=%d y=%d style=fill:white;>FREZZE SPELL: Ja aprendido  </text> ", 100 ,250);
	}
	if (e.jog.viewspell != 1){
	sprintf(link, "http://localhost/cgi-bin/dibs?learnview");
	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 5, 300, 100, 80, "#f58000", "#000000" , IMAGE_PATH "Skillz.png");
	FECHAR_LINK;
	printf ("<text x=%d y=%d style=fill:white;>VIEW SPELL: Aprende o poder  - 100 gold  </text> ", 100 ,350);
	}
	else{
		printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 5, 300, 100, 80, "#f58000", "#000000" , IMAGE_PATH "Skillzoff.png");
		printf ("<text x=%d y=%d style=fill:white;>VIEW SPELL: Ja aprendido  </text> ", 100 ,350);
	}
	if (e.jog.obsspell != 1){
	sprintf(link, "http://localhost/cgi-bin/dibs?learnobs");
	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 5, 100, 100, 80, "#f58000", "#000000" , IMAGE_PATH "obshelp.png");
	FECHAR_LINK;
	printf ("<text x=%d y=%d style=fill:white;>OBS SPELL: Aprende o poder  - 100 gold  </text> ", 100 ,150);
	}
	else{
		printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 5, 100, 100, 80, "#f58000", "#000000" , IMAGE_PATH "obshelpoff.png");
		printf ("<text x=%d y=%d style=fill:white;>OBS SPELL: Ja aprendido  </text> ", 100 ,150);
	}
	if (e.jog.doublespell != 1){
	sprintf(link, "http://localhost/cgi-bin/dibs?learndouble");
	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 5, 5, 100, 80, "#f58000", "#000000" , IMAGE_PATH "Double.png");
	FECHAR_LINK;
	printf ("<text x=%d y=%d style=fill:white;>DOUBLE SPELL: Aprende o poder  - 500 gold  </text> ", 100 , 45);
	}
	else{
		printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 5, 5, 100, 80, "#f58000", "#000000" , IMAGE_PATH "Doubleoff.png");
		printf ("<text x=%d y=%d style=fill:white;>DOUBLE SPELL: Ja aprendido  </text> ", 100 ,45);
	}

	if (e.jog.expboost == 1){
	sprintf(link, "http://localhost/cgi-bin/dibs?comprarboostexp");
	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 0, 450, 100, 100, "#f58000", "#000000" , IMAGE_PATH "expboost.png");
	FECHAR_LINK;
	printf ("<text x=%d y=%d style=fill:white;>Anel double exp  - 1000 gold  </text> ", 100 ,500);
	}
	else {
	printf ("<text x=%d y=%d style=fill:white;>Anel double exp vendido  </text> ", 100 ,500); 
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 0, 450, 100, 100, "#f58000", "#000000" , IMAGE_PATH "expboostoff.png");

	}



	sprintf(link, "http://localhost/cgi-bin/dibs?comprarmana");
	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 0, 600, 100, 100, "#f58000", "#000000" , IMAGE_PATH "manapot.png");
	FECHAR_LINK;
	printf ("<text x=%d y=%d style=fill:white;>MANA POT: Aumenta em 1 os manapots do jogador - 100 gold  </text> ", 100 ,650);

	sprintf(link, "http://localhost/cgi-bin/dibs?comprarhealt");
	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 0, 800, 100, 100, "#f58000", "#000000" , IMAGE_PATH "healpot.png");
	FECHAR_LINK;
	printf ("<text x=%d y=%d style=fill:white;>HEAL POT: Aumenta em 1 os healpots do jogador - 100 gold  </text> ", 100 ,850);
	
	sprintf(link, "http://localhost/cgi-bin/dibs?vendermanapot");
	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 640, 500, 100, 100, "#f58000", "#000000" , IMAGE_PATH "manapot.png");
	FECHAR_LINK;
	printf ("<text x=%d y=%d style=fill:white;>Vender por 50 gold  </text> ", 720 ,550);

	sprintf(link, "http://localhost/cgi-bin/dibs?venderhealtpot");
	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 640, 600, 100, 100, "#f58000", "#000000" , IMAGE_PATH "healpot.png");
	FECHAR_LINK;
	printf ("<text x=%d y=%d style=fill:white;>Vender por 50 gold  </text> ", 720 ,650);

}
/**
\brief Função que imprime o menu 
*/
void makemenu(){
	printf("<rect x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2'/>\n", 2, 2, 900 , 900, "#ffffff","#000000");
	char link[MAX_BUFFER];
	sprintf(link, "http://localhost/cgi-bin/dibs?jogar");
	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 100, 0, 500, 500, "#f58000", "#000000" , IMAGE_PATH "start.png");
	FECHAR_LINK;


	sprintf(link, "http://localhost/cgi-bin/dibs?scores");
	ABRIR_LINK(link);
	printf("<image x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2' xlink:href=%s />\n", 100, 300, 500, 500, "#f58000", "#000000" , IMAGE_PATH "score.png");
	FECHAR_LINK;


}

/**
\brief Função principal que imprime o mapa com o jogador, os inimigos,score, stats, obstáculos, saída, potes, loja ,etc 
*/
int main() {
	int x, y;
	srandom (time(NULL));
	
		COMECAR_HTML;
	ABRIR_SVG(1600, 1400);
	ESTADO e = ler_estado(getenv("QUERY_STRING"));
	if (e.jog.heal <= 0){
		metescore(e);
	}

	else if (e.higthscores)
	{ imprimescores(e);
	if (e.estadojog){
		imprime_skillz(e);
		imprime_stats(e);
		}
	}
	else if (e.loja)
	{ imprimeloja(e);

	imprime_skillz(e);

	imprime_stats(e);
	}
	else if (e.estadojog == 0)
		makemenu();
	else {
	

		printf("<rect x=%d y=%d width=%d height=%d fill=%s stroke=%s stroke-width='2'/>\n", 2, 2, 900 , 900, "#ff0000","#000000");
		for(y = 0; y < TAM; y++)
			for(x = 0; x < TAM; x++)
				imprime_casa(x, y);
		imprime_potes(e);
		imprime_obstaculos(e);
		imprime_inimigos(e);
		imprime_saida(e);
		imprime_shop(e);
		IMAGEMATASHURIKEN(getposjogx (e),getposjogy(e),ESCALA,"Shuriken2.png");
		imprime_ataquesafetados(e);
		batalharanged(e);
		imprime_jogador(e);
		imprime_skillz(e);
		imprime_stats(e);
		


				
		}

		FECHAR_SVG;
return 0;


}

