
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
  int R, G, B;
} pixel;

pixel **matriz;
int height, width;

//ToGray função de rotina da Thread
void *ToGray(void *arg) {
  int id = *(int*) arg;
  for(int j = 0; j < width; j++){
    //formula de conversao dada na questão
    int C = (matriz[id][j].R * 0.30) + (matriz[id][j].G * 0.59) + (matriz[id][j].B * 0.11);  

    //atualiza o valor de cada elemento RGB da celula da matriz
    matriz[id][j].R = C;
    matriz[id][j].G = C;
    matriz[id][j].B = C;
  }
  pthread_exit(NULL);
}

int main(void) {
  char ppm[3];
  int max_val;
  FILE *in;

  in = fopen("flowers.ppm", "r+");
  if (in == NULL) {
    printf("Erro ao abrir o arquivo");
    exit(-1);
  }

  //ppm recebe o tipo de arquivo de ppm, na questão dada o tipo é P3
  fscanf(in, " %s", ppm);

  if(strcmp(ppm, "P3") != 0){
    printf("Tipo errado de PPM");
  }else {
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

    //numero de threads = altura da matriz
    int num_thread = height; 
    pthread_t threads[num_thread];
    int Id[num_thread];
  
    for (int i = 0; i < height; i++){
          Id[i] = i;
          pthread_create(&(threads[i]), NULL, ToGray, (void*) &Id[i]);
    }

    //espera a conclusão dos processos de todas as threads antes de começar a printar
    for(int i = 0; i < num_thread; i++){
       pthread_join(threads[i], NULL);
    }
  
    FILE *out = fopen("flowers_gray.ppm", "w+");
    if(out == NULL){
      printf("erro na criacao do arquivo de saida");
      exit(-1);
    }
  
    fprintf(out, "%s\n", ppm); 
    fprintf(out, "%d %d\n", width, height); 
    fprintf(out, "%d\n", max_val); 
  
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        //pega o valor atualizado pelas threads e escreve no arquivo
        fprintf(out, "%d %d %d\n", matriz[i][j].R, matriz[i][j].G, matriz[i][j].B); 
      }
    }
    printf("DONE\n");
    
    fclose(out);
  }
  fclose(in);
  return 0;
}
