#include "inizator.h"

no* ini_alocar() {
  // Aloca um novo nó para uso na estrutura de leitura do ini
  //
  // RETORNO:
  // no* = Sucesso,
  // NULL = Falhou

  no* alocacao;

  if ((alocacao = malloc(sizeof(no))) == NULL) {
    printf("\n[Inizator] Erro ao tentar alocar memoria para um no. (Ela esta muito cheia?)\n");
    return NULL;
  }
  return alocacao;
}

no* ini_ler(char arquivo[]) {
  // Lê o arquivo ini e gera uma esturuta de nós contendo todos os seus dados
  //
  // ENTRADA:
  // arquivo[] = Nome do arquivo ini a ser aberto
  //
  // RETORNO:
  // no* = Sucesso,
  // NULL = Falhou

  int c, tamanho = 0, fix = 0, bloquear_leitura = 0, i;
  no *inicio, *temp, *temp2, *topo, *verificar;
  char caracteres[MAX_CHAR_PARAMETRO_VALOR];
  FILE * fp = fopen(arquivo, "r");

  if (fp == NULL)
    return NULL;

  // Primeiro nó
  if ((temp = temp2 = topo = inicio = ini_alocar()) == NULL)
    return NULL;

  // Indicativo para reconhecer a primeira seção
  strcpy((*inicio).secao, "inicio");

  while ((c = fgetc(fp)) != EOF) {
    // FIX - Isso limpa um lixo que aparece no início da leitura do arquivo
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
    // Comentários
    if (c == ';') {
      while ((c = fgetc(fp)) != EOF) {
        // Processar comentários nessa linha... (eles estão sendo ignorados)
        if (c == '\n')
          break;
      }
      continue;
    }
    // Seções
    if (c == '[') {
      while ((c = fgetc(fp)) != EOF) {
        // Coleto o nome da seção
        if (c != ']')
          caracteres[tamanho++] = c;
        // Se "tamanho" ficar grande demais, sou obrigado a cortar ele caracteres[]
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
          // Garanto que a seção vai iniciar desbloqueada para leitura de seus parâmetros/valores
          if (bloquear_leitura == 1)
            bloquear_leitura = 0;
          if (strcmp((*temp).secao, "inicio") != 0) {
            // Verifico se a seção já existe. Se sim, ignora ela e todos os seus parâmetros/valores
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
            // Esse nó finaliza os nós criados na secao anterior
            if((temp = ini_alocar()) == NULL) {
              ini_limpar(inicio);
              return NULL;
            }
            (*temp2).proximo_parametro = temp;
            (*temp).proximo_parametro = NULL;
            (*temp).proxima_secao = NULL;
            // Alocação de novos nós de seção
            if((temp = ini_alocar()) == NULL) {
              ini_limpar(inicio);
              return NULL;
            }
            (*topo).proxima_secao = temp;
            (*temp).proximo_parametro = NULL;
            (*temp).proxima_secao = NULL;
            temp2 = temp;
            topo = temp;
          // Se for o primeiro nó, eu inicializo ele
          } else {
            strcpy((*temp).parametro, "");
            strcpy((*temp).parametro_valor, "");
            (*temp).proximo_parametro = NULL;
            (*temp).proxima_secao = NULL;
          }
          // Guardo o nome da seção
          strcpy((*temp).secao, caracteres);
          tamanho = 0;
          break;
        }
      }
    }

    // Parâmetros: nomes e valores
    if ((bloquear_leitura == 0) && (c != ' ') && (c != '\n') && (c != '\t')) {
      // Coleto a primeira letra do parâmetro
      caracteres[tamanho] = c;
      while ((c = fgetc(fp)) != EOF) {
        // Vou coletando o resto do parâmetro
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
          // Removo os espacos em branco no final do nome do parâmetro (caso existam)
          if (caracteres[tamanho] == ' ') {
            while (caracteres[tamanho - 1] == ' ') {
              tamanho--;
            }
          }
          caracteres[tamanho++] = '\0';
          // Verifico se o parâmetro já existe na seção. Se sim, ignora ele e seu valor
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
          // Alocação de novos nós de parâmetro
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
          // Guardo o parâmetro
          strcpy((*temp).parametro, caracteres);
          tamanho = 0;
          // Valor do parâmetro
          while ((c = fgetc(fp)) != EOF) {
            if (c != ' ') {
              // Coleto a primeira letra do valor do parâmetro
              caracteres[tamanho] = c;
              // Se não tiver um valor, copio uma string vazia para ele e vou embora
              if (c == '\n') {
                strcpy((*temp).parametro_valor, "");
                tamanho = 0;
                break;
              } else {
                while ((c = fgetc(fp)) != EOF) {
                  // Vou coletando o resto do valor do parâmetro
                  if (c != '\n')
                    caracteres[++tamanho] = c;
                  // Se "tamanho" ficar grande demais, sou obrigado a cortar ele caracteres[]
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
                    // Removo os espaços em branco no final do valor do parâmetro (caso existam)
                    if ((caracteres[tamanho - 1] == ' ') || (caracteres[tamanho] == ' ')) {
                      while ((caracteres[tamanho - 1] == ' ') || (caracteres[tamanho] == ' ')) {
                        tamanho--;
                      }
                    }
                    if (strcmp(SISTEMA,"Linux") == 0)
                      caracteres[tamanho] = '\0';
                    else if (strcmp(SISTEMA,"Windows") == 0)
                      caracteres[tamanho+1] = '\0';
                    // Guardo o valor do parâmetro
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
          tamanho = 0; // Preciso disso aqui para evitar erros por parte do usuário
          break;
        }
      }
    }
  }

  fclose(fp);

  // Esse nó finaliza os nós criados na última seção
  if((temp = ini_alocar()) == NULL) {
    ini_limpar(inicio);
    return NULL;
  }
  (*temp2).proximo_parametro = temp;
  (*temp).proximo_parametro = NULL;
  (*temp).proxima_secao = NULL;
  // Esse nó finaliza a última seção
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
  //
  // ENTRADA:
  // no_secao = Nó de seção a partir do qual a impressão iniciará,
  // secao[] = Nome da seção a imprimir (entrar com "QUALQUER_SECAO" para imprimir todas as seções a partir de no_secao)

  no* no_parametro = no_secao;

  // Evito que rode a função em um nó nulo
  if (no_secao == NULL)
    return;

  // Enquanto houver seções...
  while ((*no_secao).proxima_secao != NULL) {
    // Se eu estiver na seção buscada ou ela não importar...
    if ((strcmp((*no_parametro).secao, secao) == 0) || (strcmp(secao, "QUALQUER_SECAO") == 0)) {
      // Imprimo a seção
      printf("\n[%s]\n", (*no_secao).secao);
      // Enquanto houver parâmetros...
      while ((*no_parametro).proximo_parametro != NULL) {
        // Imprimo os parâmetros e seus valores
        printf("  %s = %s\n", (*no_parametro).parametro, (*no_parametro).parametro_valor);
        no_parametro = (*no_parametro).proximo_parametro;
      }
      // Ao acabar a seção, paro tudo se secao[] não for "QUALQUER_SECAO"
      if (strcmp(secao, "QUALQUER_SECAO") != 0)
        break;
    }
    // Continuo a ler as seções se secao[] for "QUALQUER_SECAO"
    no_secao = (*no_secao).proxima_secao;
    no_parametro = no_secao;
  }
}

char* ini_buscar(no* estrutura, char termo[], char secao[]) {
  // Checa se um dado parâmetro está expresso no arquivo ini
  //
  // ENTRADA:
  // estrutura = Primeiro nó de uma estrutura de arquivo ini,
  // termo[] = Parâmetro buscado,
  // secao[] = Nome da seção onde haverá a busca (entrar com "QUALQUER_SECAO" para buscar em todas as seções)
  // 
  // RETORNO:
  // char* = Sucesso,
  // NULL = Falhou

  char* retorno = NULL;
  // Se a seção do nó for a mesma da buscada ou se a busca for geral, entro nela
  if ((strcmp((*estrutura).secao, secao) == 0) || strcmp(secao, "QUALQUER_SECAO") == 0)
    // Se estivermos no parâmetro certo, retornamos ele
    if (strcmp((*estrutura).parametro, termo) == 0)
      return (*estrutura).parametro_valor;
    // Se não estivermos no parâmetro certo...
    else {
      // Mudo para próximos parâmetros
      if ((*estrutura).proximo_parametro != NULL)
        retorno = ini_buscar((*estrutura).proximo_parametro, termo, secao);
      if (retorno != NULL)
        return retorno;
      // Mudo para próximas seções
      if ((*estrutura).proxima_secao != NULL)
        retorno = ini_buscar((*estrutura).proxima_secao, termo, secao);
    }
  // Se a seção do nó não for a buscada, vou para a próxima
  else
    if ((*estrutura).proxima_secao != NULL)
      retorno = ini_buscar((*estrutura).proxima_secao, termo, secao);
  if (retorno != NULL)
    return retorno;
  else
    return NULL;
}

int ini_alterar_valor(no* estrutura, char parametro[], char secao[], char parametro_novo_valor[]) {
  // Busco por um parâmetro e altero o valor dele
  //
  // ENTRADA:
  // estrutura = Primeiro nó de uma estrutura de arquivo ini,
  // parametro[] = Nome do parâmetro que será alterado,
  // secao[] = Seção do parâmetro a ser alterado (entrar com "QUALQUER_SECAO" em secao[] para alterar esse parâmetro em todas as seções que o tiverem),
  // parametro_novo_valor[] = Novo valor do parâmetro
  //
  // RETORNO:
  // Valor positivo qualquer = Sucesso (é a quantidade de valores alterados),
  // 0 = Nada foi alterado,
  // -1 = o valor de entrada é grande demais
 
  no* no_parametro = estrutura;
  int i = 0;

  // Checo se parametro_novo_valor[] não é grande demais
  while (parametro_novo_valor[i] != '\0')
    i++;
  if (i >= MAX_CHAR_PARAMETRO_VALOR)
    return -1;

  i = 0;
  // Enquanto houver seções...
  while ((*estrutura).proxima_secao != NULL) {
    // Se eu estiver na seção buscada ou ela não importar...
    if ((strcmp((*no_parametro).secao, secao) == 0) || (strcmp(secao, "QUALQUER_SECAO") == 0)) {
      // Enquanto houver parâmetros...
      while ((*no_parametro).proximo_parametro != NULL) {
        // Se for o parâmetro que eu busco...
        if (strcmp((*no_parametro).parametro, parametro) == 0) {
          // Mudo o valor dele e paro de checar essa sequência de parâmetros
          strcpy((*no_parametro).parametro_valor, parametro_novo_valor);
          i ++;
          break;
        }
        // Se não for o parâmetro que eu busco, vejo se é o próximo
        no_parametro = (*no_parametro).proximo_parametro;
      }
      // Se "QUALQUER_SECAO" não estiver marcado, paro de ler as seções
      if (strcmp(secao, "QUALQUER_SECAO") != 0)
        break;
    }
    // Se eu não estiver na seção certa ou ela não importar, vou para a próxima seção
    estrutura = (*estrutura).proxima_secao;
    no_parametro = estrutura;
  }
  return i;
}

no* ini_limpar(no* estrutura) {
  // Libera todos os nós que foram alocados na leitura do ini
  //
  // ENTRADA:
  // estrutura = Primeiro nó de uma estrutura de arquivo ini

  // Evito que rode a função em um nó nulo
  if (estrutura == NULL)
    return NULL;
  // Mudo para próximos parâmetros
  if ((*estrutura).proximo_parametro != NULL)
    ini_limpar((*estrutura).proximo_parametro);
  // Mudo para próximas seções
  if ((*estrutura).proxima_secao != NULL)
    ini_limpar((*estrutura).proxima_secao);
  // Limpo o que estiver aberto
  free(estrutura);
}
