#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_INC 4
#define NUM_EXEC 5
int A[NUM_INC][NUM_INC] = {{1, 2, -1, 3},{3, -1, 2, -4},{2, 3, -4, 1},{-1, 1, 5, -2}};
int B[NUM_INC] = {10, -5, 12, 8};
float X[NUM_INC] = {1, 1, 1, 1};
float newX[NUM_INC];
int N;
pthread_barrier_t barrier;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

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
            calculate(X, newX, A[b], B, b);
        }
        pthread_barrier_wait(&barrier);
        while(pthread_mutex_trylock(&mutex) != 0){}
        for(int b = id; b < NUM_INC; b += N){
            X[b] = newX[b];
        }
        pthread_mutex_unlock(&mutex);
        pthread_barrier_wait(&barrier);
    }
    pthread_exit(NULL);
}

int main(){
    scanf("%d", &N);
    pthread_t Thread[N];
    pthread_barrier_init(&barrier, NULL, N);
    int Id[N];
    for(int a = 0; a < N; a++){
        Id[a] = a;
        pthread_create(&Thread[a], NULL, Jacobi, (void *)&Id[a]);
    }
    for(int a = 0; a < N; a++){
        pthread_join(Thread[a], NULL);
    }
    printf("Resultado após %d iterações:\n", NUM_EXEC);
    for(int a = 0; a < NUM_INC; a++){
        printf("X%d = %.2f\n", a, X[a]);
    }
    return 0;
}
