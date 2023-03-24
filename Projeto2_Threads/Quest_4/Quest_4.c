#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_INC 4  //Numero de Incognitas
#define NUM_EXEC 5  //Numero de Execuções
int A[NUM_INC][NUM_INC] = {{1, 2, -1, 3},{3, -1, 2, -4},{2, 3, -4, 1},{-1, 1, 5, -2}};
int B[NUM_INC] = {10, -5, 12, 8};
float X[NUM_INC] = {1, 1, 1, 1};
float newX[NUM_EXEC][NUM_INC];
int N;
pthread_barrier_t barrier;

void calculate(float *X, float *newX, int *A, int *B, int i){
    float P = 0;
    for(int j = 0; j < NUM_INC; j++){
        if(j != i){
            P += A[j]*X[j];
        }
    }
    P = B[i] - P;
    newX[i] = P/A[i];
}

void *Jacobi(void *Id){
    int id = *(int *)Id;
    for(int a = 0; a < NUM_EXEC; a++){
        for(int b = id; b < NUM_INC; b += N){
            calculate(X, newX[a], A[b], B, b);
        }
        pthread_barrier_wait(&barrier); //Sincronização das threads (Posiciona barreira)
        for(int b = id; b < NUM_INC; b += N){ //Passa os valores do novo X para o X
            X[b] = newX[a][b];
        }
        pthread_barrier_wait(&barrier); //Sincronização das threads
    }
    pthread_exit(NULL);
}

int main(){
    scanf("%d", &N);
    pthread_t Thread[N];
    pthread_barrier_init(&barrier, NULL, N); //Cria a barreira
    int Id[N];
    for(int a = 0; a < N; a++){ //Cria as threads
        Id[a] = a;
        pthread_create(&Thread[a], NULL, Jacobi, (void *)&Id[a]);
    }
    for(int a = 0; a < N; a++){ //Espera todas as threads terminarem de execcutar
        pthread_join(Thread[a], NULL);
    }
    for(int a = 0; a < NUM_EXEC; a++){ //Printa os resultados obtidos
        printf("Resultado após %d iterações: \n", a+1);
        for(int b = 0; b < NUM_INC; b++){
            printf("X%d = %.4f\n", b, newX[a][b]);
        }
        printf("\n");
    }
    return 0;
}