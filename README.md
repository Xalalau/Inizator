# Biblioteca ini_*

Criada para ler e manipular informações de arquivos .ini.

Plataformas: Windows e Linux.


### Download + dependências

Clone o repositório e inicialize os submódulos:

```sh
$ git clone https://github.com/xalalau/Inizator.git
$ git submodule init
$ git submodule update
````

### Estrutura do arquivo .ini

(Nota: me desculpem pelos exemplos, eu escrevi o primeiro Inizator na adolescência kkk)

É da seguinte forma que o programa espera o texto e o transforma:

```ini
[Seção 1]

; Isso é um comentário inútil que vai sumir
; Outro comentário

pote de mel = ursinho puff
policia_braba_rio = BOPE
Bolsonaro = Militar


[BIRRR]

; Comentário 1

p1 = NOPE
p2 = NOPE2



[Seção 3]

; Outro comentário

link = http://site.com
PASTA = /home/teste/hu3
estado = Roraima


[Seção -1]

Hu3-Life = Half-Life 3

[Ultima seção]

parametro1 = true
parametro final = false
```

Estrutura:

![Imagem da estrutura](http://i.imgur.com/rKYUsO1.png)


### Funções disponíveis

ini* **ini_ler** (char arquivo[])
```
	/* 
	 * Lê o arquivo ini e gera uma esturuta de nós contendo todos os seus dados
	 *
	 * ENTRADA:
	 * arquivo[] = Nome do arquivo ini a ser aberto
	 *
	 * RETORNO:
	 * ini* = Sucesso,
	 * NULL = Falhou
	 * 
	 */
```

void **ini_imprimir** (ini* estrutura, char secao[])
```
	/* 
	 * Imprime as informacoes coletadas no arquivo ini
	 *
	 * ENTRADA:
	 * estrutura = Nó a partir do qual a impressão iniciará,
	 * secao = Nome da seção a imprimir (entrar com "" para imprimir todas as seções a partir de no_secao)
	 * 
	 */
```

char* **ini_buscar** (ini* estrutura, char secao[], char termo[])
```
	/* 
	 * Checa se um dado parâmetro está expresso no arquivo ini
	 *
	 * ENTRADA:
	 * estrutura = Primeiro nó de uma estrutura de arquivo ini,
	 * termo = Parâmetro buscado,
	 * secao = Nome da seção onde haverá a busca (entrar com "" para buscar em todas as seções)
	 * 
	 * RETORNO:
	 * char* = Sucesso,
	 * NULL = Falhou
	 * 
	 */
```

int **ini_alterar** (ini* estrutura, char secao[], char parametro[], char valor[])
```
	/*
	 * Busco por um parâmetro e altero o valor dele
	 *
	 * ENTRADA:
	 * estrutura = Primeiro nó de uma estrutura de arquivo ini,
	 * parametro = Nome do parâmetro que será alterado,
	 * secao = Seção do parâmetro a ser alterado (entrar com "" em secao[] para alterar toda ocorrência desse parâmetro),
	 * valor = Novo valor do parâmetro
	 *
	 * RETORNO:
	 * Valor positivo qualquer = Sucesso (é a quantidade de valores alterados),
	 * 0 = Nada foi alterado,
	 * -1 = o valor de entrada é grande demais
	 * 
	 */
```
  
int **ini_inserir** (ini* estrutura, char secao[], char parametro[], char valor[])
```
	/* 
	 * Insere um novo parâmetro em dada seção
	 *
	 * ENTRADA:
	 * estrutura = Primeiro nó de uma estrutura de arquivo ini,
	 * secao = Seção do parâmetro a ser inserido,
	 * parametro = Nome do parâmetro que será inserido,
	 * valor = Valor do parâmetro.
	 *
	 * RETORNO:
	 * 0 = Falha,
	 * 1 = Sucesso.
	 * 
	 */
```

int **ini_limpar** (ini* no_atual)
```
	/* Libera todos os nós que foram alocados na leitura do ini
	 *
	 * ENTRADA:
	 * no_atual = Entrar com primeiro nó de uma estrutura de arquivo ini. Recursavamente ele vai sendo limpo.
	 *
	 * RETORNO:
	 * 0 = Falha,
	 * 1 = Sucesso.
	 * 
	 */
```

### Exemplo de main(), para testes

Copie o código logo abaixo em um arquivo "teste.c", dentro da pasta "src":

```C
#include "inizator.h"
#include <stdio.h>

int main() {
	/*
	 * Exemplos de uso do Inizator.
	 * 
	 */
	
	ini* leitura = NULL, *leitura2 = NULL;
	char* resultado_busca;
	char termo1[30] = "parametro final";
	char secao1[30] = "Ultima seção";
	char termo2[30] = "parametro2";
	char secao2[30] = "Seção 3";
	char arquivo[100] = "../cfg_exemplo.ini";
	//char arquivo[100] = "../cfg_exemplo2.ini";

	// Leitura
	if((leitura = ini_ler(arquivo)) == NULL) {
		printf("ERRO DE LEITURA!");
		return 0;
	}

	// Impressões
	printf("\nImpressão geral:\n");
	ini_imprimir(leitura, "");
	printf("\nImprimindo a Seção 1:\n");
	ini_imprimir(leitura, "Seção 1");

	printf("\n_____________________________________________\n");

	// Buscas
	if ((resultado_busca = ini_buscar(leitura, secao1, termo1)) == NULL)
		printf("\n\n'%s' nao foi encontrado pela busca em '%s'!\n", termo1, secao1);
	else
		printf("\n\n'%s' foi encontrado pela busca em '%s'!\n VALOR: \"%s\"\n", termo1, secao1, resultado_busca);

	if ((resultado_busca = ini_buscar(leitura, "", termo1)) == NULL)
		printf("\n'%s' nao foi encontrado pela busca geral!\n\n", termo1);
	else
		printf("\n'%s' foi encontrado pela busca geral em alguma secao!\n VALOR: \'%s\'\n\n", termo1, resultado_busca);

	if ((resultado_busca = ini_buscar(leitura, secao2, termo2)) == NULL)
		printf("'%s' nao foi encontrado pela busca em '%s'!\n", termo2, secao2);
	else
		printf("'%s' foi encontrado pela busca em '%s'!\n VALOR: \"%s\"\n", termo2, secao2, resultado_busca);
	if ((resultado_busca = ini_buscar(leitura, "", termo2)) == NULL)
		printf("\n'%s' nao foi encontrado pela busca geral!\n\n", termo2);
	else
		printf("\n'%s' foi encontrado pela busca geral em alguma secao!\n VALOR: \'%s\'\n", termo2, resultado_busca);

	printf("\n_____________________________________________\n");

	// Alterações
	printf("\nO valor de '%s' e: '%s'\n", termo2, ini_buscar(leitura, "Seção 1", termo2));
	printf("\nAlterando...\n");
	ini_alterar(leitura, "Seção 1", termo2, "OUTRO VALOR AÍ");
	printf("\nO novo valor de '%s' e: '%s'\n", termo2, ini_buscar(leitura, "Seção 1", termo2));
	printf("\nImprimindo a Seção 1:\n");

	ini_imprimir(leitura, "Seção 1");

	printf("\nAlterando todos os \"parametros1\" com o valor \"Eu mexi aqui!!!!!!\"\n");
	ini_alterar(leitura, "", "parametro1", "Eu mexi aqui!!!!!!");
	printf("\nSituacao geral:\n");
	ini_imprimir(leitura, "QUALQUER_SECAO");
	printf("\n\n");

	printf("\n_____________________________________________\n");

	// Inserção

	printf("\nInserindo novo parametro na Secao1...\n");
	ini_inserir(leitura, "Seção 1", "eu_nao_existia", "tenho um valor novo tambem");
	printf("\nImprimindo a Seção 1:\n");

	ini_imprimir(leitura, "Seção 1");

	// Limpeza
	ini_limpar(leitura);

	return 1;
}
```

E rode os seguintes comandos

```sh
cd caminho_da_pasta_src
gcc inizator.c teste.c -o teste
./teste
```
