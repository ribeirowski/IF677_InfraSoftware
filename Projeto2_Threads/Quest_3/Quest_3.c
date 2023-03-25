#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct{
  int R, G, B;
} pixel;

void *ToGray(void *arg) { //ToGray função de rotina da Thread
  pixel *px = (pixel*) arg;//conversão de cada célula da matriz de pixels do tipo void para um ponteiro pixel
  int C = (px->R * 0.30) + (px->G * 0.59) + (px->B * 0.11);  //formula dde conversao dada na questão

  //aqui atualiza o valor de cada elemento RGB da celula da matriz
  px->R = C;
  px->G = C;
  px->B = C;

  //aqui a thread é morta pois ja cumpriu sua função
  pthread_exit(NULL);
}

int main(void) {


  //criação do ponteiro para o arquivo de entrada 
  FILE *in;
  in = fopen("entrada.txt", "r+");
  if (in == NULL) {
    printf("Erro ao abrir o arquivo");
    exit(-1);
  }

  //ppm recebe o tipo de arquivo de ppm, na questão dada o tipo é P3
  char ppm[3];
  fscanf(in, " %s", ppm);

  //altura e largura da matriz
  int height, width;
  fscanf(in, " %d %d", &height, &width);

  pixel matriz[height][width]; //alocação estática da matriz pois ela tem tamanho fixo 

  int num_thread = height * width; // o numero de threadas é a quantidade de celulas da matriz
  pthread_t threads[num_thread];

  int max_val;
  fscanf(in, " %d", &max_val);

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      fscanf(in, "%d %d %d ", &matriz[i][j].R, &matriz[i][j].G, &matriz[i][j].B);
      pthread_create(&(threads[i + j]), NULL, ToGray, (void*) &(matriz[i][j]));//para cada elemento da matriz é criada uma thread de conversao 
    }
  }

  FILE *out = fopen("saida.txt", "w+");
  if(out == NULL){
    printf("erro na criacao do arquivo de saida");
    exit(-1);
  }
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      fprintf(out, "%d %d %d\n", matriz[i][j].R, matriz[i][j].G, matriz[i][j].B); //pega o valor atualizado pelas threads e envia escreve no arquivo
    }
  }

  fclose(in);
  fclose(out);
  return 0;
}
