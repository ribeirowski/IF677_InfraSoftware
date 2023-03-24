#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int R, G, B;
} pixel, *ptr_pixel;

void *ToGray(void *arg) {
  ptr_pixel px = (ptr_pixel)arg;
  int C = (px->R * 0.30) + (px->G * 0.59) + (px->B * 0.11);
  px->R = C;
  px->G = C;
  px->B = C;

  pthread_exit(NULL);
}

int main(void) {

  FILE *in;
  in = fopen("entrada.txt", "r+");
  if (in == NULL) {
    printf("Erro ao abrir o arquivo");
    exit(-1);
  }

  char ppm[3];
  fscanf(in, " %s", ppm);

  int height, width;
  fscanf(in, " %d %d", &height, &width);

  pixel matriz[height][width];

  int num_thread = height * width;
  pthread_t threads[num_thread];

  int max_val;
  fscanf(in, " %d", &max_val);

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      fscanf(in, "%d %d %d ", &matriz[i][j].R, &matriz[i][j].G,
             &matriz[i][j].B);
      pthread_create(&(threads[i + j]), NULL, ToGray, &(matriz[i][j]));
    }
  }

  FILE *out = fopen("saida.txt", "w+");
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      fprintf(out, "%d %d %d\n", matriz[i][j].R, matriz[i][j].G,
              matriz[i][j].B);
    }
  }
  
  fclose(in);
  fclose(out);
  return 0;
}
