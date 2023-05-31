#include "simulador.c"

int main(int argc, char * argv[]) {
  if (argc != 5) {
    printf("Erro! Argumentos inválidos!\n");
    return 0;
  }

  algoritmo = argv[1]; // Define o algoritmo de substituição de página
  caminhoArquivo = argv[2]; // Define o caminho do arquivo de entrada
  tamPagina = atoi(argv[3]); // Converte o tamanho da página para inteiro
  tamMemoria = atoi(argv[4]); // Converte o tamanho da memória para inteiro

  if (tamPagina < 2 || tamPagina > 64) {
    printf("Erro! O tamanho de cada página deve estar entre 2 e 64!\n");
    return 0;
  }

  if (tamMemoria < 128 || tamMemoria > 16384) {
    printf("Erro! O tamanho da memória deve estar entre 128 e 16384!\n");
    return 0;
  }

  if (strcmp(algoritmo, "lru") && strcmp(algoritmo, "nru") && strcmp(algoritmo, "second_chance")) {
    printf("Erro! O algoritmo deve ser lru, nru ou second_chance!\n");
    return 0;
  }

  numPaginas = tamMemoria/tamPagina; // Calcula o número total de páginas com base no tamanho da memória e do tamanho da página

  if (strlen(caminhoArquivo) > 0) {
    arq = fopen(caminhoArquivo, "r"); // Abre o arquivo de entrada no modo de leitura
    if (arq == NULL) {
      printf("Erro! Falha ao abrir o arquivo de entrada!\n");
      return 0;
    }

    while (fgets(linha, sizeof(linha), arq) != NULL) {
      strncpy(enderecoTemp, linha, 8); // Copia os primeiros 8 caracteres da linha (endereço)
      enderecoTemp[8] = '\0'; // Adiciona o caractere nulo ao final do endereço

      if (linha[9] == 'W' || linha[9] == 'w') {
        escreveEndereco(enderecoTemp); // Realiza uma operação de escrita
      } else if (linha[9] == 'R' || linha[9] == 'r') {
        if (procuraPagina(enderecoTemp) == 1) {
          leituras++; // Incrementa o contador de operações de leitura
        }
      } else {
        printf("Erro! Os dados do arquivo de entrada estão em formato incorreto!\n");
        fclose(arq); // Fecha o arquivo de entrada
        return 0;
      }
    }

    fclose(arq); // Fecha o arquivo de entrada
  } else {
    printf("Erro! Arquivo de entrada inválido!\n");
    return 0;
  }

  printf("\nExecutando o simulador...\n\n");
  printf("Arquivo de entrada: %s\n",caminhoArquivo);
  printf("Tamanho da memória: %dKB\n", tamMemoria);
  printf("Tamanho das páginas: %dKB\n", tamPagina);
  printf("Tecnica de reposicao: %s\n", algoritmo);
  printf("Paginas lidas: %d\n", leituras);
  printf("Paginas escritas: %d\n", escritas);

  freeLista();

  return 0;
}
