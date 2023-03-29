#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct{
  int R, G, B;
} pixel;

pixel **matriz;
int height, width;

void *ToGray(void *arg) { //ToGray função de rotina da Thread
  int id = *(int*) arg;
  for(int j = 0; j < width; j++){
    int C = (matriz[id][j].R * 0.30) + (matriz[id][j].G * 0.59) + (matriz[id][j].B * 0.11);  //formula dde conversao dada na questão

    //aqui atualiza o valor de cada elemento RGB da celula da matriz
    matriz[id][j].R = C;
    matriz[id][j].G = C;
    matriz[id][j].B = C;
  }

  //aqui a thread é morta pois ja cumpriu sua função
  pthread_exit(NULL);
}

int main(void) {
  char ppm[3];
  int max_val;
  FILE *in;

  //criação do ponteiro para o arquivo de entrada
  in = fopen("flowers.ppm", "r+");
  if (in == NULL) {
    printf("Erro ao abrir o arquivo");
    exit(-1);
  }

  //ppm recebe o tipo de arquivo de ppm, na questão dada o tipo é P3
  fscanf(in, " %s", ppm);
  //altura e largura da matriz
  fscanf(in, " %d %d", &width, &height);
  fscanf(in, "%d", &max_val);
  //alocação dinâmica da matriz
  matriz = (pixel**)malloc(sizeof(pixel*) * height);
  for(int i = 0;  i < height; i++){
    matriz[i] = (pixel*)malloc(sizeof(pixel) * width);
    for(int j = 0; j < width; j++){
      fscanf(in, " %d %d %d", &matriz[i][j].R, &matriz[i][j].G, &matriz[i][j].B);
    }
  }
  int num_thread = height; // o numero de threadas é a quantidade de celulas da matriz
  pthread_t threads[num_thread];
  int Id[num_thread];

  for (int i = 0; i < height; i++){
        Id[i] = i;
        pthread_create(&(threads[i]), NULL, ToGray, (void*) &Id[i]);
  }

  for(int i = 0; i < num_thread; i++){
     pthread_join(threads[i], NULL);
  }

  FILE *out = fopen("flowers_gray.ppm", "w+");
  if(out == NULL){
    printf("erro na criacao do arquivo de saida");
    exit(-1);
  }

  fprintf(out, "%s\n", ppm); //pega o valor
  fprintf(out, "%d %d\n", width, height); //pega o valor
  fprintf(out, "%d\n", max_val); //pega o valor

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      fprintf(out, "%d %d %d\n", matriz[i][j].R, matriz[i][j].G, matriz[i][j].B); //pega o valor atualizado pelas threads e envia escreve no arquivo
    }
  }
  printf("DONE\n");
  fclose(in);
  fclose(out);
  return 0;
}
