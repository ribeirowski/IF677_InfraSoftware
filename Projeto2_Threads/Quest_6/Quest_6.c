#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define T_MAX_BUF 10
#define N 4
int ID = 0;
pthread_t Thread[N];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int A;
    int B;
} Parameters;

typedef struct lista {
    int (*function)(int), ID;
    Parameters P;
    struct lista *next;
} List;

typedef struct {
    int TamMax, Tam;
    List *Head, *Tail;
} Buffer;
// TO DO: criar functions de execução

void Insert_Buffer(Buffer *B, int (*function)(int), Parameters P){ // Insere a função, o parametro e o id no Buffer
    List *New = (List *)malloc(sizeof(List));
    New->function = function;
    New->P = P;
    New->ID = ID;
    New->next = B->Tail->next;
    B->Tail = New;
    (B->Tam)++;
    ID++;
}

void Delete_Buffer(Buffer *B) { // Deleta a função do buffer
    List *Del;
    B->Head->next = Del;
    B->Head->next = Del->next;
    free(Del);
    if (B->Head->next == NULL) {
      B->Tail = B->Head;
    }
    (B->Tam)--;
}

int agendarExecucao(Buffer *B, int (*function)(int), Parameters P) {
  if (B->Tam < B->TamMax) { // insere a função a ser executada no buffer de
                            // execução, se o buffer não estiver cheio
      Insert_Buffer(B, function, P);
      return ID; // retorna o id sequencial
  } else
      return -1;
}

int main() {
    Buffer B;
    B.TamMax = T_MAX_BUF;
    B.Head = (List *)malloc(sizeof(List));
    B.Tail = B.Head;
    B.Head->next = NULL;
    B.Tam = 0;
    pthread_t Despache;
    pthread_create(&Despache, NULL, Handler, NULL);
    int exit = 0;
    while (!exit) {
        int op, retorno;
        Parameters operandos;
        printf("\nQual operacao?\n\n");
        printf("1 - \n 2- \n 3 - \n 4 - exit\n> ");
        scanf("%d", &op);
        switch (op) {
        case 1:
            printf("Defina os valores: ");
            scanf(" %d %d", &operandos.A, &operandos.B);
            retorno = agendarExecucao(B, function1, operandos);
            break;
        case 2:
            printf("Defina os valores: ");
            scanf(" %d %d", &operandos.A, &operandos.B);
            retorno = agendarExecucao(B, function2, operandos);
            break;
        case 3:
            printf("Defina os valores: ");
            scanf(" %d %d", &operandos.A, &operandos.B);
            retorno = agendarExecucao(B, function3, operandos);
            break;
        case 4:
            exit = 1;
            break;
        default:
            printf("operacao nao listada");
            break;
        }
        if (retorno == -1) {
            printf("Buffer cheio\n");
        } else {
            printf("Operation ID = %d\n", retorno);
      }
  }
  // TO DO: criar lista de threads em exec e threads prontas
  // o user pode escolher qual resultado printar pela ID da thread
}
