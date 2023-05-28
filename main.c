#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Page {
  char address[9];
  struct Page * next;
}
Page;

char * alg; // Algoritmo de substituição de página
char * filePath; // Caminho do arquivo de entrada
char line[20]; // Linha lida do arquivo
char tmpAddress[9]; // Endereço temporário
int pageSize; // Tamanho de cada página em KB
int memSize; // Tamanho total da memória em KB
int numPages; // Número total de páginas
int operations = 0; // Total de operações
int reads = 0; // Total de operações de leitura
int writes = 0; // Total de operações de escrita
int hits = 0; // Total de acertos (hits)
int misses = 0; // Total de faltas (misses)
int writebacks = 0; // Total de escritas na memória secundária
int usedPages = 0; // Número de páginas utilizadas atualmente
float faults = 0; // Total de page faults
FILE * file; // Ponteiro para o arquivo de entrada
Page * first = NULL; // Ponteiro para a primeira página
Page * last = NULL; // Ponteiro para a última página

// Função para adicionar uma nova página
void AddNewPage(char value[9]) {
  Page * current = malloc(sizeof(Page));
  strcpy(current -> address, value);
  current -> next = NULL;

  if (usedPages == 0) {
    // Se nenhuma página estiver sendo usada, definimos a nova página como a primeira e a última
    first = current;
    last = first;
  } else {
    // Caso contrário, adicionamos a nova página ao final da lista encadeada
    last -> next = current;
    last = current;
  }

  if (usedPages < numPages)
    usedPages++;

  writes++; // Incrementa o contador de operações de escrita
}

// Algoritmo Least Recently Used (LRU)
void LRU(char value[9]) {
  AddNewPage(value);
  if (usedPages == numPages)
    first = first -> next; // Remove a primeira página da lista encadeada (a mais antiga)
}

// Algoritmo Not Recently Used (NRU)
void NRU(char value[9]) {
  AddNewPage(value);
  if (usedPages == numPages) {
    Page *tmp = first;
    first = first->next; // Remove a primeira página da lista encadeada (a mais antiga)
    free(tmp); // Libera a página mais antiga da memória
  }
}

// Algoritmo Segunda Chance (Second-Chance)
void SegundaChance(char value[9]) {
  AddNewPage(value);
  if (usedPages == numPages) {
    Page *tmp = first;
    first = first->next; // Remove a primeira página da lista encadeada (a mais antiga)
    free(tmp); // Libera a página mais antiga da memória
  }
}

// Função para encontrar uma página na memória
int Find(char value[9]) {
  Page * tmp = first;
  while (tmp != NULL) {
    if (strcmp(tmp -> address, value) == 0) {
      if (strcmp(alg, "lru") == 0)
        LRU(value);
      else if (strcmp(alg, "nru") == 0)
        NRU(value);
      return 0; // Encontrou a página, retorna 0 (acerto)
    }
    tmp = tmp -> next;
  }
  return 1; // Não encontrou a página, retorna 1 (falha)
}

// Função para substituir uma página na memória
void ReplacePage(char value[9]) {
  if (strcmp(alg, "lru") == 0)
    LRU(value);
  else if (strcmp(alg, "nru") == 0)
    NRU(value);
  else if (strcmp(alg, "second_chance") == 0)
    SegundaChance(value);
  writebacks++; // Incrementa o contador de escritas na memória secundária
}

// Função para escrever um endereço na memória
void WriteAddress() {
  if (usedPages < numPages) {
    AddNewPage(tmpAddress); // Se ainda há páginas disponíveis na memória, adiciona a nova página
  } else {
    faults++;
    ReplacePage(tmpAddress); // Caso contrário, ocorre uma falta de página e é necessário substituir uma página existente
  }
}
void FreeMemory(){
    printf("\n...\n");
    Page *current = first;
    while (current != NULL) {
      Page *tmp = current;
      current = current->next;
      free(tmp);
    }
}
int main(int argc, char * argv[]) {
  if (argc != 5) {
    printf("ERRO: Argumentos inválidos.\n");
    return 0;
  }

  alg = argv[1]; // Define o algoritmo de substituição de página
  filePath = argv[2]; // Define o caminho do arquivo de entrada
  pageSize = atoi(argv[3]); // Converte o tamanho da página para inteiro
  memSize = atoi(argv[4]); // Converte o tamanho da memória para inteiro

  if (pageSize < 2 || pageSize > 64) {
    printf("ERRO: O tamanho de cada página deve estar entre 2 e 64.\n");
    return 0;
  }

  if (memSize < 128 || memSize > 16384) {
    printf("ERRO: O tamanho da memória deve estar entre 128 e 16384.\n");
    return 0;
  }

  if (strcmp(alg, "lru") && strcmp(alg, "nru") && strcmp(alg, "second_chance")) {
    printf("ERRO: O algoritmo deve ser lru, nru ou second_chance.\n");
    return 0;
  }

  numPages = memSize / pageSize; // Calcula o número total de páginas com base no tamanho da memória e do tamanho da página

  if (strlen(filePath) > 0) {
    file = fopen(filePath, "r"); // Abre o arquivo de entrada no modo de leitura
    if (file == NULL) {
      printf("ERRO: Falha ao abrir o arquivo de entrada.\n");
      return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
      operations++; // Incrementa o contador de operações
      strncpy(tmpAddress, line, 8); // Copia os primeiros 8 caracteres da linha (endereço)
      tmpAddress[8] = '\0'; // Adiciona o caractere nulo ao final do endereço

      if (line[9] == 'W' || line[9] == 'w') {
        WriteAddress(tmpAddress); // Realiza uma operação de escrita
      } else if (line[9] == 'R' || line[9] == 'r') {
        if (Find(tmpAddress)) {
          hits++; // A página foi encontrada na memória (acerto)
        } else {
          misses++; // A página não foi encontrada na memória (falta)
          WriteAddress(tmpAddress); // Realiza uma operação de escrita para trazer a página para a memória
        }
        reads++; // Incrementa o contador de operações de leitura
      } else {
        printf("ERRO: Os dados do arquivo de entrada estão em formato incorreto.\n");
        fclose(file); // Fecha o arquivo de entrada
        return 0;
      }
    }
    fclose(file); // Fecha o arquivo de entrada
  } else {
    printf("ERRO: Arquivo de entrada inválido.\n");
    return 0;
  }

  printf("\nExecutando o simulador...\n\n");
  printf("Arquivo de entrada: %s\n",filePath);
  printf("Tamanho da memória: %iKB\n", memSize);
  printf("Tamanho das páginas: %iKB\n", pageSize);
  printf("Tecnica de reposicao: %s\n", alg);
  printf("Paginas lidas: %i\n", reads);
  printf("Paginas escritas: %i\n", writes);

  FreeMemory();

  return 0;
}