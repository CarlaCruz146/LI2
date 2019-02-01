#ifndef ___ESTADO_H___
#define ___ESTADO_H___

/**
@file estado.h
Definição do estado e das funções que convertem estados em strings e vice-versa
*/

/** \brief O nº máximo de inimigos */
#define MAX_INIMIGOS		100

/** \brief O tamanho de cada lado do tabuleiro */
#define TAM					15


/** \brief O nº máximo de obstáculos */
#define MAX_OBSTACULOS		100

/** \brief O nº máximo de potes existentes */
#define MAX_POTS 			100




/**
\brief Estrutura que armazena uma posição
*/
typedef struct posicao {
	char x;
	char y;
} POSICAO;

/**
\brief Estrutura que armazena o inventário
*/
typedef struct inventario { /** id item e quantidade item */
	char item; 
	char quantidade;
} INVENTARIO;

/**
\brief Estrutura que armazena as coordenadas do mapa
*/
typedef struct matriz{
	int map[TAM][TAM];
} MATRIZ;

/**
\brief Estrutura que armazena informação relativa aos potes das poções
*/
typedef struct pot
{	POSICAO pos;
	char type;
} POT;

/**
\brief Estrutura que armazena informação relativa ao jogador
*/
typedef struct infojog
{ 	
	char str;
	char percep;
	char stam;
	char spiri;
	char lvl;
	char exp;
	char skillz;
	char shuriken;
	char healpot;
	char manapot;
	char lvlupstats;
	char doublejump;
	char obs;
	char frezzespell;
	char doublespell;
	char viewspell;
	char obsspell;
	char expboost;
	POSICAO pos;
	int heal;
	int mana;
	int max_heal;
	int max_mana;
	int gold;
} INFOJOG;
/**
\brief Estrutura que armazena informação relativa aos monstros, desde o seu tipo ao seu nível
*/
typedef struct infomonstro
{	char type;
	char lvl;
	int heal;	
	POSICAO pos;
} INFOMONSTRO;

typedef struct obstaculos
{	char type;
	POSICAO pos;
} OBS;

/**
\brief Estrutura que armazena o estado do jogo
*/
typedef struct estado {
	char nivel;
	char num_inimigos;
	char num_obstaculos;
	char frezze;
	char enimigosmortos;
	char showataques;
	char higthscores;
	char num_pot;
	char loja;
	char estadojog;
	POSICAO saida;
	POSICAO shop;
	OBS obstaculo[MAX_OBSTACULOS];
	POT pot[MAX_POTS];
	INFOMONSTRO inimigo[MAX_INIMIGOS];
	INFOJOG jog;
} ESTADO;

/**
\brief Função que converte um estado numa string
@param e O estado
@returns A string correspondente ao estado e
*/
char *estado2str(ESTADO e);

ESTADO inicializar(ESTADO x);

/**
\brief Função que converte uma string num estado 
@param argumentos Uma string contendo os argumentos passados à CGI
@returns O estado correspondente à string dos argumentos
*/
ESTADO str2estado(char *argumentos);

#endif
