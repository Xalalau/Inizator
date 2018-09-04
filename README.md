# Biblioteca geral_*

Funciona no Windows e no Linux.

no* **ini_alocar** ( ) { }
```
Aloca um novo nó para uso na estrutura de leitura do ini

RETORNO:
no* = Sucesso,
NULL = Falhou
```

no* **ini_ler** (char arquivo[ ]) { }
```
Lê o arquivo ini e gera uma esturuta de nós contendo todos os seus dados

ENTRADA:
arquivo[ ] = Nome do arquivo ini a ser aberto

RETORNO:
no* = Sucesso,
NULL = Falhou
```

void **ini_imprimir** (no* no_secao, char secao[ ]) { }
```
Imprime as informacoes coletadas no arquivo ini

ENTRADA:
no_secao = Nó de seção a partir do qual a impressão iniciará,
secao[ ] = Nome da seção a imprimir (entrar com "QUALQUER_SECAO" para imprimir todas as seções a partir de no_secao)
```


char* **ini_buscar** (no* estrutura, char termo[ ], char secao[ ]) { }
```
Checa se um dado parâmetro está expresso no arquivo ini

ENTRADA:
estrutura = Primeiro nó de uma estrutura de arquivo ini,
termo[ ] = Parâmetro buscado,
secao[ ] = Nome da seção onde haverá a busca (entrar com "QUALQUER_SECAO" para buscar em todas as seções)

RETORNO:
char* = Sucesso,
NULL = Falhou
```

int **ini_alterar_valor** (no* estrutura, char parametro[ ], char secao[ ], char parametro_novo_valor[ ]) { }
```
Busco por um parâmetro e altero o valor dele

ENTRADA:
estrutura = Primeiro nó de uma estrutura de arquivo ini,
parametro[ ] = Nome do parâmetro que será alterado,
secao[ ] = Seção do parâmetro a ser alterado (entrar com "QUALQUER_SECAO" em secao[ ] para alterar esse parâmetro em todas as seções que o tiverem),
parametro_novo_valor[ ] = Novo valor do parâmetro

RETORNO:
Valor positivo qualquer = Sucesso (é a quantidade de valores alterados),
0 = Nada foi alterado,
-1 = o valor de entrada é grande demais
```
  
no* **ini_limpar** (no* estrutura) { }
```
Libera todos os nós que foram alocados na leitura do ini

ENTRADA:
estrutura = Primeiro nó de uma estrutura de arquivo ini
```

##Exemplo de uso

```C
int main() {
  no* leitura = NULL, *leitura2 = NULL;
  char* resultado_busca;
  char termo1[30] = "parametro final";
  char secao1[30] = "Ultima seção";
  char termo2[30] = "parametro2";
  char secao2[30] = "Seção 3";
  char arquivo[100] = "./Inizator/cfg_exemplo.ini";

  if((leitura = ini_ler(arquivo)) == NULL) {
    printf("ERRO DE LEITURA!");
    return -1;
  }

  ini_imprimir(leitura, "QUALQUER_SECAO");

  printf("\n_____________________________________________\n");

  if ((resultado_busca = ini_buscar(leitura, termo1, secao1)) == NULL)
    printf("\n\n'%s' nao foi encontrado pela busca em '%s'!\n", termo1, secao1);
  else
    printf("\n\n'%s' foi encontrado pela busca em '%s'!\n VALOR: \"%s\"\n", termo1, secao1, resultado_busca);

  if ((resultado_busca = ini_buscar(leitura, termo1, "QUALQUER_SECAO")) == NULL)
    printf("\n'%s' nao foi encontrado pela busca geral!\n\n", termo1);
  else
    printf("\n'%s' foi encontrado pela busca geral em alguma secao!\n VALOR: \'%s\'\n\n", termo1, resultado_busca);

  if ((resultado_busca = ini_buscar(leitura, termo2, secao2)) == NULL)
    printf("'%s' nao foi encontrado pela busca em '%s'!\n", termo2, secao2);
  else
    printf("'%s' foi encontrado pela busca em '%s'!\n VALOR: \"%s\"\n", termo2, secao2, resultado_busca);
  if ((resultado_busca = ini_buscar(leitura, termo2, "QUALQUER_SECAO")) == NULL)
    printf("\n'%s' nao foi encontrado pela busca geral!\n\n", termo2);
  else
    printf("\n'%s' foi encontrado pela busca geral em alguma secao!\n VALOR: \'%s\'\n", termo2, resultado_busca);

  printf("\n_____________________________________________\n");

  printf("\nO valor de '%s' e: '%s'\n", termo2, ini_buscar(leitura, termo2, "Seção 1"));
  printf("\nAlterando...\n");
  ini_alterar_valor(leitura, termo2, "Seção 1", "OUTRO VALOR AÍ");
  printf("\nO novo valor de '%s' e: '%s'\n", termo2, ini_buscar(leitura, termo2, "Seção 1"));
  printf("\nImprimindo a Seção 1:\n");

  ini_imprimir(leitura, "Seção 1");

  printf("\n_____________________________________________\n");

  printf("\nAlterando todos os \"parametros1\" com o valor \"Eu mexi aqui!!!!!!\"\n");
  ini_alterar_valor(leitura, "parametro1", "QUALQUER_SECAO", "Eu mexi aqui!!!!!!");
  printf("\nSituacao geral:\n");
  ini_imprimir(leitura, "QUALQUER_SECAO");
  printf("\n\n");

  leitura = ini_limpar(leitura);

  return 0;
}
```
