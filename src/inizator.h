/*
 * INIZATOR v1.0 [10/06/2016]
 * por Xalalau Xubilozo
 * 
 * Licensa: CC BY-NC-SA 4.0
 * http://creativecommons.org/licenses/by-nc-sa/4.0/
 * */

#ifndef HEADER_FILE
#define HEADER_FILE
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <errno.h>
  #include "../../geral/src/geral.h"
#endif

#define MAX_CHAR_SECAO 40
#define MAX_CHAR_PARAMETRO_NOME 50
#define MAX_CHAR_PARAMETRO_VALOR 700

typedef struct nos {
  char secao[MAX_CHAR_SECAO];
  char parametro[MAX_CHAR_PARAMETRO_NOME];
  char parametro_valor[MAX_CHAR_PARAMETRO_VALOR];
  struct nos* proxima_secao;
  struct nos* proximo_parametro;
}no;

no* ini_ler(char arquivo[]);
void ini_imprimir(no* no_secao, char secao[]);
char* ini_buscar(no* leitura, char termo[], char secao[]);
int ini_alterar_valor(no* no_secao, char parametro[], char secao[], char parametro_novo_valor[]);
void ini_limpar(no* leitura);
