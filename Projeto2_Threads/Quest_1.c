#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM 3
int num = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *Cont(void *Id){
    int id = *(int*)Id;
    while(num < 1000000){
        while(pthread_mutex_trylock(&mutex) != 0){
            if(num == 1000000){
                pthread_exit(NULL);
            }
        }
        num++;
        printf("Thread %d: %d\n", id, num);
        if(num == 1000000){
            printf("Thread %d chegou em %d\n", id, num);
            pthread_exit(NULL);
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main(){
    pthread_t Contador[NUM];
    int Id[NUM];
    for (int a = 0; a < NUM; a++){
        Id[a] = a;
        pthread_create(&Contador[a], NULL, Cont, (void*)&Id[a]);
    }
    pthread_exit(NULL);
    return 0;
}
