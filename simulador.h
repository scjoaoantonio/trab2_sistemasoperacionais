#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pagina {
  char endereco[9];
  struct Pagina * proximo;
}Pagina;

char *algoritmo = NULL; // Algoritmo de substituição de página
char *caminhoArquivo = NULL; // Caminho do arquivo de entrada
char linha[20]; // Linha lida do arquivo
char enderecoTemp[9]; // Endereço temporário
int tamPagina = 0; // Tamanho de cada página em KB
int tamMemoria = 0; // Tamanho total da memória em KB
int numPaginas = 0; // Número total de páginas
int leituras = 0; // Total de operações de leitura
int escritas = 0; // Total de operações de escrita
int paginasUtilizadas = 0; // Número de páginas utilizadas atualmente
FILE * arq = NULL; // Ponteiro para o arquivo de entrada
Pagina *primeiro = NULL; // Ponteiro para a primeira página
Pagina *ultimo = NULL; // Ponteiro para a última página

void adicionaPagina(char conteudo[9]);
int procuraPagina(char conteudo[9]);
void substituiPagina(char conteudo[9]);
void lru(char conteudo[9]);
void nru(char conteudo[9]);
void segundaChance(char conteudo[9]);
void escreveEndereco();
void freeLista();

#endif
