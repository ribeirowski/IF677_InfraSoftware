#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM 3
int num = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *Cont(void *Id){
    int id = *(int*)Id;
    while(num < 1000000){
        while(pthread_mutex_trylock(&mutex) != 0){ //Impede outras threads de somar o "num"
            if(num == 1000000){   //Se alguma thread já tiver alcançado o num = 1000000, a thread atual é fechada
                pthread_exit(NULL);
            }
        }
        num++;
        printf("Thread %d: %d\n", id, num); //Printa a thread e o contador
        if(num == 1000000){    //Se a thread atual chegou em num = 1000000, acaba  tudo
            printf("Thread %d chegou em %d\n", id, num); //Printa a thread que chegou no resultado
            pthread_exit(NULL);  //Fecha a thread sem dar o unlock para que threads futuras continuem sem poder incrementar o "num"
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main(){
    pthread_t Contador[NUM];
    int Id[NUM];
    for (int a = 0; a < NUM; a++){ //Cria as threads
        Id[a] = a;
        pthread_create(&Contador[a], NULL, Cont, (void*)&Id[a]);
    }
    pthread_exit(NULL);
    return 0;
}
