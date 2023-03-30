#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T_MAX_BUF 10
#define N 4
int ID = 0;
pthread_t Thread[N];
bool Exe[N] = {false};
pthread_mutex_t mutex1, mutex2, mutex3, mutex4 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty, full, ready, pronto = PTHREAD_COND_INITIALIZER;

typedef struct {
    int A;
    int B;
    int Thread_id;
} Parameters;

typedef struct lista {
    void *(*function)(void *);
    int ID;
    Parameters P;
    struct lista *next;
} List;

typedef struct {
    int TamMax, Tam;
    List *Head, *Tail;
} Buffer;

typedef struct {
    int R;
    bool Exe;
    bool ready;
} Result;

Buffer B;
Result *R;

void Insert_Buffer(
    void *(*function)(void *),
    Parameters P) { // Insere a função, o parametro e o id no Buffer
    List *New = (List *)malloc(sizeof(List));
    New->function = function;
    New->P = P;
    New->ID = ID;
    New->next = B.Tail->next;
    B.Tail->next = New;
    B.Tail = New;
    (B.Tam)++;
    ID++;
    R = (Result *)realloc(R, sizeof(Result) * ID);
    R[ID - 1].ready = 0;
    if (B.Tam == 1) {
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
    if (B.Tam == (B.TamMax) - 1) {
        pthread_cond_signal(&full);
    }
    return Ret;
}

void *mmc(void *P) {
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
    R[num.ID].R = (num.P.A * num.P.B) / a;
    R[num.ID].Exe = 0;
    R[num.ID].ready = 1;
    pthread_cond_signal(&pronto);
    Exe[num.P.Thread_id] = false;
    pthread_exit(NULL);
}

void *mdc(void *P) {
    int resto;
    List num = *(List *)P;
    pthread_mutex_unlock(&mutex3);
    do {
        resto = num.P.A % num.P.B;
        num.P.A = num.P.B;
        num.P.B = resto;
    } while (resto != 0);
    R[num.ID].R = num.P.A;
    R[num.ID].Exe = 0;
    R[num.ID].ready = 1;
    pthread_cond_signal(&pronto);
    Exe[num.P.Thread_id] = false;
    pthread_exit(NULL);
    }

    int agendarExecucao(void *(*function)(void *), Parameters P) {
    pthread_mutex_lock(&mutex1);
    if (B.Tam == B.TamMax) {
        pthread_cond_wait(&full, &mutex1);
    }
    pthread_mutex_unlock(&mutex1);
    Insert_Buffer(function, P);
    return B.Tail->ID; // retorna o id sequencial
}

void *Handler() {
    List Task;
    bool Escolha;
    while (true) {
        pthread_mutex_lock(&mutex1);
        Escolha = false;
        if (B.Tam == 0) {
            pthread_cond_wait(&empty, &mutex1);
        }
        pthread_mutex_lock(&mutex3);
        Task = Delete_Buffer();
        while (!Escolha) {
            for (int a = 0; a < N && !Escolha; a++) {
                if (Exe[a] == false) {
                    Exe[a] = true;
                    Task.P.Thread_id = a;
                    pthread_create(&Thread[a], NULL, Task.function, (void *)&Task);
                    Escolha = true;
                }
            }
        }
        pthread_mutex_unlock(&mutex1);
    }
}

int pegarResultadoExecucao(int id) {
    pthread_mutex_lock(&mutex3);
    while (!R[id].ready) {
        pthread_cond_wait(&pronto, &mutex3);
    }
    pthread_mutex_unlock(&mutex3);
    return R[id].R;
}

int main() {
    Parameters P;
    B.TamMax = T_MAX_BUF;
    B.Head = (List *)malloc(sizeof(List));
    B.Tail = B.Head;
    B.Head->next = NULL;
    B.Tam = 0;
    R = (Result *)calloc(1, sizeof(Result));
    pthread_t Despache;
    pthread_create(&Despache, NULL, Handler, NULL);
    // Agendar as funções
    FILE *input = fopen("entrada.txt", "r+");
    if (input == NULL) {
        printf("falha ao abrir o arquivo de entrada");
        exit(-1);
    }
    char op[4];
    do {
        int id;
        fscanf(input, " %s", op);
        if (strcmp(op, "REQ") == 0) {
            fscanf(input, " %d", &id);
            printf("Resultado %d = %d\n", id, pegarResultadoExecucao(id));
        } else if (strcmp(op, "MMC") == 0) {
            fscanf(input, " %d %d", &P.A, &P.B);
            printf("%s %d %d ID: %d\n", op, P.A, P.B, agendarExecucao(mmc, P));
        } else if (strcmp(op, "MDC") == 0) {
            fscanf(input, " %d %d", &P.A, &P.B);
            printf("%s %d %d ID: %d\n", op, P.A, P.B, agendarExecucao(mdc, P));
        }
    } while (strcmp(op, "END") != 0);
    fclose(input);
    return 0;
}
