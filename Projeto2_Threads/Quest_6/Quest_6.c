#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define T_MAX_BUF 10
int N = 4;
int ID = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct{
    int A;
    int B;
} Parameters;

typedef struct lista{
    int (*function)(int), ID;
    Parameters P;
    struct lista *next;
} List;

typedef struct{
    int TamMax, Tam;
    List *Head, *Tail;
} Buffer;

void Insert_Buffer(Buffer *B, int (*function)(int), Parameters P){ //Insere a função, o parametro e o id no Buffer
    List *New = (List*) malloc (sizeof(List));
    New->function = function;
    New->P = P;
    New->ID = ID;
    New->next = B->Tail->next;
    B->Tail = New;
    (B->Tam)++;
    ID++;
}

void Delete_Buffer(Buffer *B){ //Deleta a função do buffer
    List *Del;
    B->Head->next = Del;
    B->Head->next= Del->next;
    free(Del);
    if(B->Head->next == NULL){
        B->Tail = B->Head;
    }
    (B->Tam)--;
}

int agendarExecucao(Buffer *B, int (*function)(int), Parameters P){
    
}

int main(){
    Buffer B;
    B.TamMax = T_MAX_BUF;
    B.Head = (List*) malloc (sizeof(List));
    B.Tail = B.Head;
    B.Head->next = NULL;
    B.Tam = 0;
    
}
