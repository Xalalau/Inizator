/*
 * INIZATOR v.Git
 * por Xalalau Xubilozo
 * https://github.com/xalalau/Xalateca/tree/master/C/Inizator
 * Licença: MIT
 * */

#ifndef INIZATOR_H
#define INIZATOR_H

	#define MAX_CHAR_SECAO 40
	#define MAX_CHAR_PARAMETRO_NOME 50
	#define MAX_CHAR_PARAMETRO_VALOR 700

	typedef struct no_ {
		char secao[MAX_CHAR_SECAO];
		char parametro[MAX_CHAR_PARAMETRO_NOME];
		char valor[MAX_CHAR_PARAMETRO_VALOR];
		struct no_* proxima_secao;
		struct no_* proximo_parametro;
	} no;

	no* ini_ler(char arquivo[]);
	void ini_imprimir(no* estrutura, char secao[]);
	char* ini_buscar(no* estrutura, char secao[], char termo[]);
	int ini_alterar(no* estrutura, char secao[], char parametro[], char novo_valor[]);
	int ini_inserir(no* estrutura, char secao[], char parametro[], char valor[]);
	int ini_limpar(no* estrutura);

#endif //INIZATOR_H
