#ifndef ___CGI_H___
#define ___CGI_H___

/**
@file cgi.h
Macros úteis para gerar CGIs
*/

#include <stdio.h>

/**
* Caminho para as imagens
*/
#define IMAGE_PATH							"http://localhost/images/"

/**
\brief Macro para começar o html
*/
#define COMECAR_HTML						printf("Content-Type: text/html\n\n")

/**
\brief Macro para abrir um svg
@param tamx O comprimento do svg
@param tamy A altura do svg
*/
#define ABRIR_SVG(tamx, tamy)				printf("<svg width=%d height=%d>\n", tamx, tamy)
/**
\brief Macro para fechar um svg
*/
#define FECHAR_SVG							printf("</svg>\n")

/**
\brief Macro para criar uma imagem
@param X A coordenada X do canto superior esquerdo
@param Y A coordenada Y do canto superior esquerdo
@param ESCALA A escala da imagem
@param FICHEIRO O caminho para o link do ficheiro
*/
#define IMAGEM(X, Y, ESCALA, FICHEIRO)		printf("<image x=%d y=%d width=%d height=%d xlink:href=%s />\n", \
												ESCALA * X +2, ESCALA* Y+2, ESCALA, ESCALA, IMAGE_PATH FICHEIRO)

#define IMAGEMATASHURIKEN(X, Y, ESCALA, FICHEIRO)		printf("<image id=shuriken x=%d y=%d width=%d height=%d opacity ='0' xlink:href=%s />\n", \
															 ESCALA * X, ESCALA* Y, ESCALA, ESCALA, IMAGE_PATH FICHEIRO)

#define IMAGEMATAFLAME(X, Y, ESCALA, FICHEIRO, NUMERO)		printf("<image id=flame%i x=%d y=%d width=%d height=%d opacity ='0' xlink:href=%s />\n", \
														NUMERO, ESCALA * X, ESCALA* Y, ESCALA, ESCALA, IMAGE_PATH FICHEIRO)
#define IMAGEMATARange(X, Y, ESCALA, FICHEIRO, NUMERO)		printf("<image id=ranged%i x=%d y=%d width=%d height=%d opacity ='0' xlink:href=%s />\n", \
														NUMERO, ESCALA * X, ESCALA* Y, ESCALA, ESCALA, IMAGE_PATH FICHEIRO)

#define IMAGEMT(X, Y, ESCALA, FICHEIRO, OPACIDADE)		printf("<image x=%d y=%d width=%d height=%d xlink:href=%s  opacity = %f />\n", \
																ESCALA * X, ESCALA* Y, ESCALA, ESCALA, IMAGE_PATH FICHEIRO, OPACIDADE)

#define IMAGEMI(X, Y, ESCALA, FICHEIRO, NUMERO)		printf("<image id=inimigo%i x=%d y=%d width=%d height=%d xlink:href=%s />\n", \
												NUMERO, ESCALA * X, ESCALA* Y, ESCALA, ESCALA, IMAGE_PATH FICHEIRO)
 
#define IMAGEMJ(X, Y, ESCALA, FICHEIRO)		 printf("<image id=jog x=%d y=%d width=%d height=%d xlink:href=%s />\n", \
												 ESCALA * X, ESCALA* Y, ESCALA, ESCALA, IMAGE_PATH FICHEIRO)
 
/**
\brief Macro para criar um quadrado
@param X A coordenada X do canto superior esquerdo
@param Y A coordenada Y do canto superior esquerdo
@param ESCALA A escala do quadrado
@param COR A cor de preenchimento do quadrado
*/
#define ANIMATEATACAIFLAME(COISA, X,Y,ESCALA) 		printf("<animateMotion  xlink:href= #flame%i dur=%fs begin= %fs fill=freeze  path=%cM%i,%i %i,%i %c/>\n<animate  xlink:href= #flame%i attributeType='auto' attributeName='opacity' to='0' dur='0.1' begin='0.35s' fill=freeze />\n<animate  xlink:href= #flame%i attributeType='auto' attributeName='opacity' to='1' dur='0.001' begin='0s' fill=freeze />\n", \
														 COISA, 0.1,0.25,'"' , 0,0,X*ESCALA, Y*ESCALA ,'"', COISA,COISA)

#define ANIMATEATACAISETA(COISA, X,Y,ESCALA) 		printf("<animateMotion  xlink:href= #ranged%i dur=%fs begin= %fs fill=freeze  path=%cM%i,%i %i,%i %c/>\n<animate  xlink:href= #ranged%i attributeType='auto' attributeName='opacity' to='0' dur='0.1' begin='0.35s' fill=freeze />\n<animate  xlink:href= #ranged%i attributeType='auto' attributeName='opacity' to='1' dur='0.001' begin='0s' fill=freeze />\n", \
														 COISA, 0.1,0.25,'"' , 0,0,X*ESCALA, Y*ESCALA ,'"', COISA,COISA)


#define QUADRADO(X, Y, ESCALA, COR)			printf("<rect x=%d y=%d width=%d height=%d fill=%s />\n", \
												ESCALA * X +2, ESCALA* Y+2, ESCALA, ESCALA, COR)


#define ANIMATESHURIKEN( X,Y,ESCALA) 		printf("<animateMotion  xlink:href= #shuriken dur=%fs begin= %is fill=freeze path=%cM%i,%i %i,%i %c/>\n<animate  xlink:href= #shuriken attributeType='auto' attributeName='opacity' to='0' dur='0.1' begin='0.1s' fill=freeze />\n<animate  xlink:href= #shuriken attributeType='auto' attributeName='opacity' to='1' dur='0.001' begin='0s' fill=freeze />\n", \
													 0.1,0,'"', 0,0, X*ESCALA, Y*ESCALA , '"')


#define ANIMATEMOVEI(COISA, X,Y,ESCALA) 		printf("<animateMotion  xlink:href= #inimigo%i dur=%fs begin= %is fill=freeze path=%cM%i,%i %i,%i %c/>\n<animate  xlink:href= #shuriken attributeType='auto' attributeName='opacity' to='0' dur='0.001' begin='0.1s' fill=freeze />\n", \
													COISA, 0.10,0,'"',X*ESCALA, Y*ESCALA , 0,0, '"')

#define ANIMATEMOVEJOG(X,Y,ESCALA) 		printf("<animateMotion  xlink:href= #jog dur=%fs begin= %is fill=freeze path=%cM%i,%i %i,%i %c/>\n", \
													 0.25,0,'"',X*ESCALA, Y*ESCALA , 0,0 ,'"')

#define ANIMATEATACAI(COISA, X,Y,ESCALA) 		printf("<animateMotion  xlink:href= #inimigo%i dur=%fs begin= %fs fill=freeze path=%cM%i,%i %i,%i %c/>\n", \
													 COISA, 0.1,0.1,'"',X*ESCALA/2, Y*ESCALA/2 , 0,0 ,'"')


#define ANIMATEATACAJOG(X,Y,ESCALA) 		printf("<animateMotion  xlink:href= #jog dur=%fs begin= %is fill=freeze path=%cM%i,%i %i,%i %c/>\n", \
													 0.1,0,'"',X*ESCALA/2, Y*ESCALA/2 , 0,0 ,'"')

#define QUADRADOT(X, Y, ESCALA, COR, OPACIDADE)			printf("<rect x=%d y=%d width=%d height=%d fill=%s opacity = %f />\n", \
												ESCALA * X +2, ESCALA* Y +2, ESCALA, ESCALA, COR, OPACIDADE)

/**
\brief Macro para abrir um link
@param link O caminho para o link
*/
#define ABRIR_LINK(link)					printf("<a xlink:href=%s>\n", link)

/**
\brief Macro para fechar o link
*/
#define FECHAR_LINK							printf("</a>\n")


#endif
