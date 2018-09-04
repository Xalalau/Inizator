#include "inizator.h"

no* ini_alocar() {
  // Aloca um novo "no" para uso na estrutura de leitura do ini
  // Retorna o endereco do novo "no"

  no* alocacao;

  if ((alocacao = malloc(sizeof(no))) == NULL) {
    printf("\n[Inizator] Erro ao tentar alocar memoria para um no. (Ela esta muito cheia?)\n");
    return NULL;
  }
  return alocacao;
}

no* ini_ler(char arquivo[]) {
  // Le o arquivo ini e gera uma esturuta de "nos" contendo todos os seus dados
  // Retorna o endereco do primeiro "no" ou NULL se houver erro

  int c, tamanho = 0, fix = 0, bloquear_leitura = 0, i;
  no *inicio, *temp, *temp2, *topo, *verificar;
  char caracteres[MAX_CHAR_PARAMETRO_VALOR];
  FILE * fp = fopen(arquivo, "r");

  if (fp == NULL)
    return NULL;

  // Primeiro no
  if ((temp = temp2 = topo = inicio = ini_alocar()) == NULL)
    return NULL;

  //Indicativo para reconhecer a primeira secao
  strcpy((*inicio).secao, "inicio");

  while ((c = fgetc(fp)) != EOF) {
    // FIX - Isso limpa um lixo que aparece no inicio da leitura do arquivo
    if (fix == 0) {
      if (c == '#') {
        while ((c = fgetc(fp)) != EOF) {
          if (c == '\n')
            break;
        }
        fix = 1;
      } else
        continue;
    }
    // Comentarios
    if (c == ';') {
      while ((c = fgetc(fp)) != EOF) {
        // Processar comentarios nessa linha... (eles estao sendo ignorados)
        if (c == '\n')
          break;
      }
      continue;
    }
    // Secoes
    if (c == '[') {
      while ((c = fgetc(fp)) != EOF) {
        // Coleto o nome da secao
        if (c != ']')
          caracteres[tamanho++] = c;
        // Se "tamanho" ficar grande demais, sou obrigado a cortar ele
        if (tamanho > MAX_CHAR_SECAO) {
          caracteres[tamanho--] = '\0';
          printf("\n[Inizator] A seguinte secao passou o limite de '%d' caracteres:\n%s\n\n", MAX_CHAR_SECAO, caracteres);
          // jogo o resto da linha no lixo
          while ((c = fgetc(fp)) != EOF)
            if (c == '\n')
              break;
          c = ']';
        }
        if (c == ']') {
          caracteres[tamanho] = '\0';
          // Garanto que a secao vai iniciar desbloqueada para leitura de seus parametros/valores
          if (bloquear_leitura == 1)
            bloquear_leitura = 0;
          if (strcmp((*temp).secao, "inicio") != 0) {
            // Verifico se a secao ja existe. Se sim, ignora ela e todos os seus parametros/valores
            verificar = inicio;
            while (1) {
              if (strcmp((*verificar).secao, caracteres) == 0) {
                bloquear_leitura = 1;
                break;
              }
              if ((*verificar).proxima_secao != NULL)
                verificar = (*verificar).proxima_secao;
              else
                break;              
            }
            if ((strcmp((*verificar).secao, caracteres) == 0) || (bloquear_leitura == 1)) {
              printf("\n[Inizator] Detectada secao '%s' duplicada. Ignorando-a...\n", caracteres);
              bloquear_leitura = 1;
              tamanho = 0;
              break;
            }
            // Esse "no" finaliza os "nos" criados na secao anterior
            if((temp = ini_alocar()) == NULL) {
              ini_limpar(inicio);
              return NULL;
            }
            (*temp2).proximo_parametro = temp;
            (*temp).proximo_parametro = NULL;
            (*temp).proxima_secao = NULL;
            // Alocacao de novos "nos" de secao
            if((temp = ini_alocar()) == NULL) {
              ini_limpar(inicio);
              return NULL;
            }
            (*topo).proxima_secao = temp;
            (*temp).proximo_parametro = NULL;
            (*temp).proxima_secao = NULL;
            temp2 = temp;
            topo = temp;
          // Se for o primeiro "no", eu inicializo ele
          } else {
            strcpy((*temp).parametro, "");
            strcpy((*temp).parametro_valor, "");
            (*temp).proximo_parametro = NULL;
            (*temp).proxima_secao = NULL;
          }
          // Guardo o nome da secao
          strcpy((*temp).secao, caracteres);
          tamanho = 0;
          break;
        }
      }
    }

    // Parametros: nomes e valores
    if ((bloquear_leitura == 0) && (c != ' ') && (c != '\n') && (c != '\t')) {
      // Coleto a primeira letra do parametro
      caracteres[tamanho] = c;
      while ((c = fgetc(fp)) != EOF) {
        // Vou coletando o resto do parametro
        if (c != '=')
          caracteres[++tamanho] = c;
        // Se "tamanho" ficar grande demais, sou obrigado a cortar ele
        if (tamanho > MAX_CHAR_PARAMETRO_NOME) {
          caracteres[tamanho--] = '\0';
          printf("\n[Inizator] O seguinte parametro passou o limite de '%d' caracteres:\n%s\n\n", MAX_CHAR_PARAMETRO_NOME, caracteres);
          // jogo o resto da linha no lixo
          while ((c = fgetc(fp)) != EOF)
            if (c == '\n')
              break;
          c = '=';
        }
        if (c == '=') {
          // Removo os espacos em branco no final do nome do parametro (caso existam)
          if (caracteres[tamanho] == ' ') {
            while (caracteres[tamanho - 1] == ' ') {
              tamanho--;
            }
          }
          caracteres[tamanho++] = '\0';
          // Verifico se o parametro ja existe na secao. Se sim, ignora ele e seu valor
          verificar = topo;
          while (1) {
            if (strcmp((*verificar).parametro, caracteres) == 0) {
              bloquear_leitura = 1;
              break;
            }
            if ((*verificar).proximo_parametro != NULL)
              verificar = (*verificar).proximo_parametro;
            else
              break;
          }
          if ((strcmp((*verificar).parametro, caracteres) == 0) || (bloquear_leitura == 1)) {
            printf("\n[Inizator] Detectado parametro '%s' duplicado na secao '%s'. Ignorando-o...\n\n", caracteres, (*verificar).secao);
            bloquear_leitura = 0;
            tamanho = 0;
            break;
          }
          // Alocacao de novos "nos" de parametro
          if (strcmp((*temp).parametro, "") != 0) {
            if((temp = ini_alocar()) == NULL) {
              ini_limpar(inicio);
              return NULL;
            }
            (*temp2).proximo_parametro = temp;
            strcpy((*temp).secao, (*topo).secao);
            (*temp).proxima_secao = NULL;
            (*temp).proximo_parametro = NULL;
            temp2 = temp;
          }
          // Guardo o parametro
          strcpy((*temp).parametro, caracteres);
          tamanho = 0;
          // Valor do parametro
          while ((c = fgetc(fp)) != EOF) {
            if (c != ' ') {
              // Coleto a primeira letra do valor do parametro
              caracteres[tamanho] = c;
              // Se nao tiver um valor, copio uma string vazia para ele e vou embora
              if (c == '\n') {
                strcpy((*temp).parametro_valor, "");
                tamanho = 0;
                break;
              } else {
                while ((c = fgetc(fp)) != EOF) {
                  // Vou coletando o resto do valor do parametro
                  if (c != '\n')
                    caracteres[++tamanho] = c;
                  // Se "tamanho" ficar grande demais, sou obrigado a cortar ele
                  // Aqui tem que ser "==" por causa do Windows
                  if (tamanho == MAX_CHAR_PARAMETRO_VALOR) {
                    caracteres[tamanho--] = '\0';
                    printf("\n[Inizator] O seguinte valor de parametro passou o limite de '%d' caracteres:\n%s\n", MAX_CHAR_PARAMETRO_VALOR, caracteres);
                    // jogo o resto da linha no lixo
                    while ((c = fgetc(fp)) != EOF)
                      if (c == '\n')
                        break;
                  }
                  if (c == '\n') {
                    // Removo os espacos em branco no final do valor do parametro (caso existam)
                    if ((caracteres[tamanho - 1] == ' ') || (caracteres[tamanho] == ' ')) {
                      while ((caracteres[tamanho - 1] == ' ') || (caracteres[tamanho] == ' ')) {
                        tamanho--;
                      }
                    }
                    if (strcmp(SISTEMA,"Linux") == 0)
                      caracteres[tamanho] = '\0';
                    else if (strcmp(SISTEMA,"Windows") == 0)
                      caracteres[tamanho+1] = '\0';
                    // Guardo o valor do parametro
                    strcpy((*temp).parametro_valor, caracteres);
                    tamanho = 0;
                    break;
                  }
                }
                break;
              }
            }
          }
        }
        if (c == '\n') {
          tamanho = 0; // Preciso disso aqui para evitar erros por parte do usuario
          break;
        }
      }
    }
  }

  fclose(fp);

  // Esse "no" finaliza os "nos" criados na ultima secao
  if((temp = ini_alocar()) == NULL) {
    ini_limpar(inicio);
    return NULL;
  }
  (*temp2).proximo_parametro = temp;
  (*temp).proximo_parametro = NULL;
  (*temp).proxima_secao = NULL;
  // Esse no finaliza a ultima secao
  if((temp = ini_alocar()) == NULL) {
    ini_limpar(inicio);
    return NULL;
  }
  (*topo).proxima_secao = temp;
  (*temp).proximo_parametro = NULL;
  (*temp).proxima_secao = NULL;

  return inicio;
}

void ini_imprimir(no* no_secao, char secao[]) {
  // Imprime as informacoes coletadas no arquivo ini
  // Obs: se entrar com "QUALQUER_SECAO" em secao[], imprimo todas as secoes

  no* no_parametro = no_secao;

  // Evito que rode a funcao em um no nulo
  if (no_secao == NULL)
    return;

  // Enquanto houver secoes...
  while ((*no_secao).proxima_secao != NULL) {
    // Se eu estiver na secao buscada ou ela nao importar...
    if ((strcmp((*no_parametro).secao, secao) == 0) || (strcmp(secao, "QUALQUER_SECAO") == 0)) {
      // Imprimo a secao
      printf("\n[%s]\n", (*no_secao).secao);
      // Enquanto houver parametros...
      while ((*no_parametro).proximo_parametro != NULL) {
        // Imprimo os parametros e seus valores
        printf("  %s = %s\n", (*no_parametro).parametro, (*no_parametro).parametro_valor);
        no_parametro = (*no_parametro).proximo_parametro;
      }
      // Ao acabar a secao, paro tudo se secao[] nao for "QUALQUER_SECAO"
      if (strcmp(secao, "QUALQUER_SECAO") != 0)
        break;
    }
    // Continuo a ler as secoes se secao[] for "QUALQUER_SECAO"
    no_secao = (*no_secao).proxima_secao;
    no_parametro = no_secao;
  }
}

char* ini_buscar(no* leitura, char termo[], char secao[]) {
  // Checa se um dado parametro esta expresso no arquivo ini
  // Obs: se entrar com "QUALQUER_SECAO" em secao[], vou lendo todas as secoes e retorno a primeira aparicao do parametro
  // NULL = parametro nao encontrado, char* = parametro encontrado (ponteiro para o seu valor)

  char* retorno = NULL;
  // Se a secao do "no" for a mesma da buscada ou se a busca for geral, entro nela
  if ((strcmp((*leitura).secao, secao) == 0) || strcmp(secao, "QUALQUER_SECAO") == 0)
    // Se estivermos no parametro certo, retornamos ele
    if (strcmp((*leitura).parametro, termo) == 0)
      return (*leitura).parametro_valor;
    // Se nao estivermos no parametro certo...
    else {
      // Mudo para proximos parametros
      if ((*leitura).proximo_parametro != NULL)
        retorno = ini_buscar((*leitura).proximo_parametro, termo, secao);
      if (retorno != NULL)
        return retorno;
      // Mudo para proximas secoes
      if ((*leitura).proxima_secao != NULL)
        retorno = ini_buscar((*leitura).proxima_secao, termo, secao);
    }
  // Se a secao do "no" nao for a buscada, vou para a proxima
  else
    if ((*leitura).proxima_secao != NULL)
      retorno = ini_buscar((*leitura).proxima_secao, termo, secao);
  if (retorno != NULL)
    return retorno;
  else
    return NULL;
}

int ini_alterar_valor(no* no_secao, char parametro[], char secao[], char parametro_novo_valor[]) {
  // Busco por um parametro e altero o valor dele
  // Se entrar com "QUALQUER_SECAO" em secao[], altero esse parametro em todas as secoes que o tiverem
  // -1 = o valor de entrada é grande demais
  // 0 = Nada foi alterado
  // valor qualquer = quantidade de valores alterados
 
  no* no_parametro = no_secao;
  int i = 0;

  // Checo se parametro_novo_valor[] nao e grande demais
  while (parametro_novo_valor[i] != '\0')
    i++;
  if (i >= MAX_CHAR_PARAMETRO_VALOR)
    return -1;

  i = 0;
  // Enquanto houver secoes...
  while ((*no_secao).proxima_secao != NULL) {
    // Se eu estiver na secao buscada ou ela nao importar...
    if ((strcmp((*no_parametro).secao, secao) == 0) || (strcmp(secao, "QUALQUER_SECAO") == 0)) {
      // Enquanto houver parametros...
      while ((*no_parametro).proximo_parametro != NULL) {
        // Se for o parametro que eu busco...
        if (strcmp((*no_parametro).parametro, parametro) == 0) {
          // Mudo o valor dele e paro de checar essa sequencia de parametros
          strcpy((*no_parametro).parametro_valor, parametro_novo_valor);
          i ++;
          break;
        }
        // Se nao for o parametro que eu busco, vejo se e o proximo
        no_parametro = (*no_parametro).proximo_parametro;
      }
      // Se "QUALQUER_SECAO" nao estiver marcado, paro de ler as secoes
      if (strcmp(secao, "QUALQUER_SECAO") != 0)
        break;
    }
    // Se nao estiver, vou para a proxima
    no_secao = (*no_secao).proxima_secao;
    no_parametro = no_secao;
  }
  return i;
}

void ini_limpar(no* leitura) {
  // Libera todos os "nos" que foram alocados na leitura do ini
  // Retorna NULL no primeiro "no" para que possamos zera-lo externamente

  // Evito que rode a funcao em um no nulo
  if (leitura == NULL)
    return;
  // Mudo para proximos parametros
  if ((*leitura).proximo_parametro != NULL)
    ini_limpar((*leitura).proximo_parametro);
  // Mudo para proximas secoes
  if ((*leitura).proxima_secao != NULL)
    ini_limpar((*leitura).proxima_secao);
  // Limpo o que estiver aberto
  free(leitura);
}

/*
//EXEMPLO DE USO:
int main(void) {

  //-----------
  // Variaveis:
  //-----------

  // Esse e o primeiro "no" onde toda a leitura do arquivo ini vai ser estruturada:
  no* leitura = NULL, *leitura2 = NULL;
  // Aqui tem as variaveis de busca
  char* resultado_busca;
  char termo1[30] = "parametro final";
  char secao1[30] = "Ultima seção";
  char termo2[30] = "parametro4";
  char secao2[30] = "Seção 3";
  // Esse e um arquivo ini formatado corretamente
  char arquivo[100] = "/profiles/Xalalau/Codificações/xalateca/C/inizator/cfg_exemplo.ini";

  //---------
  // Funcoes:
  //---------

  // Inicia a leitura
  if((leitura = ini_ler(arquivo)) == NULL) {
    printf("ERRO DE LEITURA!");
    return -1;
  }

  // Imprimi tudo na tela
  ini_imprimir(leitura, "QUALQUER_SECAO");

  printf("\n_____________________________________________\n");

  // Faz a busca pelo parametro "arq_steamcmd". Ela retorna NULL para nao encontrado ou
  // um ponteiro para array de char com o que estiver depois de "=" para encontrado
  if ((resultado_busca = ini_buscar(leitura, termo1, secao1)) == NULL)
    printf("\n\n'%s' não foi encontrado pela busca em '%s'!\n", termo1, secao1);
  else
    printf("\n\n'%s' foi encontrado pela busca em '%s'!\n VALOR: \"%s\"\n", termo1, secao1, resultado_busca);

  // Tambem podemos fazer uma busca geral em todas as secoes ao inves de ficar restrito a
  // apenars uma. Basta colocar "QUALQUER_SECAO" na secao.
  if ((resultado_busca = ini_buscar(leitura, termo1, "QUALQUER_SECAO")) == NULL)
    printf("\n'%s' não foi encontrado pela busca geral!\n\n", termo1);
  else
    printf("\n'%s' foi encontrado pela busca geral em alguma secao!\n VALOR: \'%s\'\n\n", termo1, resultado_busca);

  // Repetimos os mesmos procedimentos feitos logo acima mas para outra busca:
  if ((resultado_busca = ini_buscar(leitura, termo2, secao2)) == NULL)
    printf("'%s' não foi encontrado pela busca em '%s'!\n", termo2, secao2);
  else
    printf("'%s' foi encontrado pela busca em '%s'!\n VALOR: \"%s\"\n", termo2, secao2, resultado_busca);
  if ((resultado_busca = ini_buscar(leitura, termo2, "QUALQUER_SECAO")) == NULL)
    printf("\n'%s' não foi encontrado pela busca geral!\n\n", termo2);
  else
    printf("\n'%s' foi encontrado pela busca geral em alguma secao!\n VALOR: \'%s\'\n", termo2, resultado_busca);

  printf("\n_____________________________________________\n");

  // Podemos alterar valores também:
  printf("\nO valor de '%s' e: '%s'\n", termo2, ini_buscar(leitura, termo2, "Seção 1"));
  printf("\nAlterando...\n");
  ini_alterar_valor(leitura, termo2, "Seção 1", "OUTRO VALOR AÍ");
  printf("\nO novo valor de '%s' e: '%s'\n", termo2, ini_buscar(leitura, termo2, "Seção 1"));
  printf("\nSituacao geral:\n");
  // Podemos imprimir uma certa secao tambem:
  ini_imprimir(leitura, "Seção 1");

  printf("\n_____________________________________________\n");

  printf("\nAlterando todos os parametros 1 com 1 comando...\n");
  ini_alterar_valor(leitura, "parametro1", "QUALQUER_SECAO", "Eu mexi aqui!!!!!!");
  printf("\nSituacao geral:\n");
  ini_imprimir(leitura, "QUALQUER_SECAO");
  printf("\n\n");

  // Libera toda a memoria dos mallocs e evita leaks (Sempre deve ser aplicada apos tudo!!)
  // NOTA: NAO FACA ISSO NO MEIO DO CODIGO, SO NO FINAL!! CAUSA BUGS INEXPLICAVEIS!!!
  ini_limpar(leitura);
  leitura = NULL;

  return 0;
}
*/
