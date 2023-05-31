#include "simulador.h"

// Função para adicionar uma nova página
void adicionaPagina(char conteudo[9]) {
  Pagina * pagina = malloc(sizeof(Pagina));

  strcpy(pagina->endereco, conteudo);
  pagina->proximo = NULL;

  if (paginasUtilizadas == 0) {
    // Se nenhuma página estiver sendo usada, definimos a nova página como a primeira e a última
    primeiro = pagina;
    ultimo = primeiro;
  } else {
    // Caso contrário, adicionamos a nova página ao final da lista encadeada
    ultimo->proximo = pagina;
    ultimo = pagina;
  }

  if (paginasUtilizadas < numPaginas)
    paginasUtilizadas++;

  escritas++; // Incrementa o contador de operações de escrita
}

// Função para encontrar uma página na memória
int procuraPagina(char conteudo[9]) {
  Pagina *aux = primeiro;

  while (aux != NULL) {
    if (strcmp(aux->endereco, conteudo) == 0) {
      if (strcmp(algoritmo, "lru") == 0)
        lru(conteudo);
      else if (strcmp(algoritmo, "nru") == 0)
        nru(conteudo);
      return 0; // Encontrou a página, retorna 0 (acerto)
    }
    aux = aux->proximo;
  }
  return 1; // Não encontrou a página, retorna 1 (falha)
}

// Função para substituir uma página na memória
void substituiPagina(char conteudo[9]) {
  if (strcmp(algoritmo, "lru") == 0){
    lru(conteudo);
  }else if (strcmp(algoritmo, "nru") == 0){
    nru(conteudo);
  }else if (strcmp(algoritmo, "second_chance") == 0){
    segundaChance(conteudo);
  }
}

// Algoritmo Least Recently Used (LRU)
void lru(char conteudo[9]) {
  adicionaPagina(conteudo);

  if (paginasUtilizadas == numPaginas){
    primeiro = primeiro->proximo; // Remove a primeira página da lista encadeada (a mais antiga)
  }
}

// Algoritmo Not Recently Used (NRU)
void nru(char conteudo[9]) {
  adicionaPagina(conteudo);

  if (paginasUtilizadas == numPaginas) {
    Pagina *aux = primeiro;
    primeiro = primeiro->proximo; // Remove a primeira página da lista encadeada (a mais antiga)
    free(aux); // Libera a página mais antiga da memória
  }
}

// Algoritmo Segunda Chance (Second-Chance)
void segundaChance(char conteudo[9]) {
  adicionaPagina(conteudo);

  if (paginasUtilizadas == numPaginas) {
    Pagina *aux = primeiro;
    primeiro = primeiro->proximo; // Remove a primeira página da lista encadeada (a mais antiga)
    free(aux); // Libera a página mais antiga da memória
  }
}


// Função para escrever um endereço na memória
void escreveEndereco() {
  if (paginasUtilizadas < numPaginas) {
    adicionaPagina(enderecoTemp); // Se ainda há páginas disponíveis na memória, adiciona a nova página
  } else {
    substituiPagina(enderecoTemp); // Caso contrário, ocorre uma falta de página e é necessário substituir uma página existente
  }
}

//Função para liberar a memória
void freeLista(){
    printf("\n...\n");
    Pagina *current = primeiro;
    while (current != NULL) {
      Pagina *tmp = current;
      current = current->proximo;
      free(tmp);
    }
}
