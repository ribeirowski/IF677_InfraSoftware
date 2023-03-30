#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define T_MAX_BUF 10
#define N 4
int ID = 0;
pthread_t Thread[N];
bool Exe[N] = {false};
pthread_mutex_t mutex1, mutex2, mutex3 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty, full, ready = PTHREAD_COND_INITIALIZER;

typedef struct {
    int A;
    int B;
    int Thread_id;
} Parameters;

typedef struct lista {
    void *(*function)(void*);
    int ID;
    Parameters P;
    struct lista *next;
} List;

typedef struct {
    int TamMax, Tam;
    List *Head, *Tail;
} Buffer;

typedef struct list {
    int ID, R;
    struct list *next;
} Res;

typedef struct {
    Res *Head, *Tail;
} Buff;

Buffer B;
Buff Result;

void Insert_Buffer(void *(*function)(void*), Parameters P){ // Insere a função, o parametro e o id no Buffer
    List *New = (List *)malloc(sizeof(List));
    New->function = function;
    New->P = P;
    New->ID = ID;
    New->next = B.Tail->next;
    B.Tail->next = New;
    B.Tail = New;
    (B.Tam)++;
    ID++;
    if(B.Tam == 1){
        pthread_cond_signal(&empty);
    }
}

List Delete_Buffer() { // Deleta a função do buffer
    List *Del, Ret;
    Del = B.Head->next;
    B.Head->next = Del->next;
    Ret = *Del;
    free(Del);
    if (B.Head->next == NULL) {
        B.Tail = B.Head;
    }
    (B.Tam)--;
    if(B.Tam == (B.TamMax)-1){
        pthread_cond_signal(&full);
    }
    return Ret;
}

void Insert_Result(int R, int Id){ //Insere o reusltado na fila de resultados
    Res *new = (Res *)malloc(sizeof(Res));
    new->ID = Id;
    new->R = R;
    new->next = Result.Tail->next;
    Result.Tail->next = new;
    Result.Tail = new;
    pthread_cond_signal(&ready);
}
<<<<<<< HEAD

=======
                                      
>>>>>>> c5e97126f2387c0947a44b24273da7add2ef0857
Res Delete_Result(Res *Cur){ //Deleta um resultado da fila de resultados
    Res Ret, *Del;
    Del = Cur->next;
    Cur->next = Del->next;
    Ret = *Del;
    free(Del);
    if(Result.Head->next == NULL){
        Result.Tail = Result.Head;
    }
    return Ret;
}

void *mmc(void *P){
<<<<<<< HEAD
    int resto, a, b;
    List num = *(List *)P; 
    pthread_mutex_unlock(&mutex3);
    a = num.P.A;
    b = num.P.B;
    do {
        resto = a % b;
        a = b;
        b = resto;
    } while (resto != 0);
    pthread_mutex_lock(&mutex3);
    Insert_Result((num.P.A * num.P.B) / a, num.ID);
    Exe[num.P.Thread_id] = false;
    pthread_mutex_unlock(&mutex3);
    pthread_exit(NULL);
=======
  int resto, a, b;
  List num = *(List *)P; 
  pthread_mutex_unlock(&mutex3);
      a = num.P.A;
      b = num.P.B;
      do {
          resto = a % b;
          a = b;
          b = resto;
      } while (resto != 0);
      pthread_mutex_lock(&mutex3);
      Insert_Result((num.P.A * num.P.B) / a, num.ID);
      Exe[num.P.Thread_id] = false;
      pthread_mutex_unlock(&mutex3);
      pthread_exit(NULL);
>>>>>>> c5e97126f2387c0947a44b24273da7add2ef0857
}

void *mdc(void *P){
    int resto;
    List num = *(List *)P; 
    pthread_mutex_unlock(&mutex3);
    do {
        resto = num.P.A % num.P.B;
        num.P.A = num.P.B;
        num.P.B = resto;
    } while (resto != 0);
    pthread_mutex_lock(&mutex3);
    Insert_Result(num.P.A, num.ID);
    Exe[num.P.Thread_id] = false;
    pthread_mutex_unlock(&mutex3);
    pthread_exit(NULL);
}

int agendarExecucao(void *(*function)(void*), Parameters P) {
    if(B.Tam == B.TamMax){
        pthread_mutex_lock(&mutex1);
        pthread_cond_wait(&full, &mutex1);
        pthread_mutex_unlock(&mutex1);
    }
    Insert_Buffer(function, P);
    return B.Tail->ID; // retorna o id sequencial
}

void *Handler(){
    List Task;
    bool Escolha;
    while(true){
        Escolha = false;
        if(B.Tam == 0){
            pthread_mutex_lock(&mutex1);
            pthread_cond_wait(&empty, &mutex1);
            pthread_mutex_unlock(&mutex1);
        }
        pthread_mutex_lock(&mutex3);
        Task = Delete_Buffer();
        while(!Escolha){
            for(int a = 0; a < N && !Escolha; a++){
                if(Exe[a] == false){
                    Exe[a] = true;
                    Task.P.Thread_id = a;
                    pthread_create(&Thread[a], NULL, Task.function, (void*)&Task);
                    Escolha = true;
                }
            }
        }
    }
}

int pegarResultadoExecucao(int id){
    Res *Aux = Result.Head;
    Res Ret;
    while(true){
        if(Aux->next == NULL){
            pthread_mutex_lock(&mutex2);
            pthread_cond_wait(&ready, &mutex2);
            pthread_mutex_unlock(&mutex2);
        }
        if(Aux->next->ID == id){
            Ret = Delete_Result(Aux);
            return Ret.R;
        }
        Aux = Aux->next;
    }
}

int main() {
    Parameters P;
    B.TamMax = T_MAX_BUF;
    B.Head = (List *)malloc(sizeof(List));
    B.Tail = B.Head;
    B.Head->next = NULL;
    B.Tam = 0;
    Result.Head = (Res *)malloc(sizeof(Res));
    Result.Tail = Result.Head;
    Result.Head->next = NULL;
    pthread_t Despache;
    pthread_create(&Despache, NULL, Handler, NULL);
    //Agendar as funções
    FILE * input = fopen("entrada.txt", "r+");
    if(input == NULL){
<<<<<<< HEAD
        printf("falha ao abrir o arquivo de entrada");
            exit(-1);
=======
      printf("falha ao abrir o arquivo de entrada");
      exit(-1);
>>>>>>> c5e97126f2387c0947a44b24273da7add2ef0857
    }

    char op[4];

    do {
<<<<<<< HEAD
    int id;
    fscanf(input," %s", op);
    if(strcmp(op, "REQ") == 0){
        fscanf(input, " %d", &id);
        printf("Resultado %d = %d\n", id, pegarResultadoExecucao(id));
    }else if(strcmp(op, "MMC") == 0){
        fscanf(input, " %d %d", &P.A, &P.B);
        printf("%s %d %d ID: %d\n",op, P.A, P.B, agendarExecucao(mmc, P));
    }else if(strcmp(op, "MDC") == 0){
        fscanf(input, " %d %d", &P.A, &P.B);
        printf("%s %d %d ID: %d\n",op, P.A, P.B, agendarExecucao(mdc, P));
    }
=======
      int id;
      fscanf(input," %s", op);
      if(strcmp(op, "REQ") == 0){
        fscanf(input, " %d", &id);
        printf("Resultado %d = %d\n", id, pegarResultadoExecucao(id));
      }else if(strcmp(op, "MMC") == 0){
        fscanf(input, " %d %d", &P.A, &P.B);
        printf("%s %d %d ID: %d\n",op, P.A, P.B, agendarExecucao(mmc, P));
      }else if(strcmp(op, "MDC") == 0){
        fscanf(input, " %d %d", &P.A, &P.B);
        printf("%s %d %d ID: %d\n",op, P.A, P.B, agendarExecucao(mdc, P));
      }
>>>>>>> c5e97126f2387c0947a44b24273da7add2ef0857
    }while(strcmp(op, "END") != 0);
    fclose(input);
    pthread_exit(NULL);
    return 0;
<<<<<<< HEAD
}
=======
}
>>>>>>> c5e97126f2387c0947a44b24273da7add2ef0857
