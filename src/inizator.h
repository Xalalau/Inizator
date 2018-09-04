/*
 * INIZATOR v1.3 [04/09/2018]
 * por Xalalau Xubilozo
 * https://github.com/xalalau/Xalateca/tree/master/C/Inizator
 * Licença: MIT
 * */

#ifndef INIZATOR_H
#define INIZATOR_H
	#define MAX_CHAR_SECAO 40
	#define MAX_CHAR_PARAMETRO_NOME 50
	#define MAX_CHAR_PARAMETRO_VALOR 700

	typedef struct no {
		char secao[MAX_CHAR_SECAO];
		char parametro[MAX_CHAR_PARAMETRO_NOME];
		char valor[MAX_CHAR_PARAMETRO_VALOR];
		struct no* proxima_secao;
		struct no* proximo_parametro;
	} ini;
#endif //INIZATOR_H

ini* ini_ler(char arquivo[]);
void ini_imprimir(ini* estrutura, char secao[]);
char* ini_buscar(ini* estrutura, char secao[], char termo[]);
int ini_alterar(ini* estrutura, char secao[], char parametro[], char novo_valor[]);
int ini_inserir(ini* estrutura, char secao[], char parametro[], char valor[]);
int ini_limpar(ini* estrutura);
