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

void Insert_Buffer(void *(*function)(void *), Parameters P) { // Insere a operação no Buffer
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
    R[ID - 1].ready = 0; // Inicializa o ready como 0 (não está pronto)
    if (B.Tam == 1) {
        pthread_cond_signal(&empty); // Avisa que o Buffer não está mais vazio
    }
}

List Delete_Buffer() { // Deleta a operação do buffer
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
        pthread_cond_signal(&full); // Avisa que o buffer não está mais cheio 
    }
    return Ret;
}

void *mmc(void *P) { // Operação MMC
    int resto, a, b;
    List num = *(List *)P;
    pthread_mutex_unlock(&mutex3); // Para evitar que uma alteração na Task (no handler) afete a operação
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
    pthread_cond_signal(&pronto); // Avisa que foi colocado um resultado no vetor
    Exe[num.P.Thread_id] = false; // Thread parou de executar
    pthread_exit(NULL);
}

void *mdc(void *P) { // Operação MDC
    int resto;
    List num = *(List *)P;
    pthread_mutex_unlock(&mutex3); // Para evitar que uma alteração na Task (no handler) afete a operação
    do {
        resto = num.P.A % num.P.B;
        num.P.A = num.P.B;
        num.P.B = resto;
    } while (resto != 0);
    R[num.ID].R = num.P.A;
    R[num.ID].Exe = 0;
    R[num.ID].ready = 1;
    pthread_cond_signal(&pronto); // Avisa que foi colocado um resultado no vetor
    Exe[num.P.Thread_id] = false; // Thread parou de executar
    pthread_exit(NULL);
    }

    int agendarExecucao(void *(*function)(void *), Parameters P) {
    pthread_mutex_lock(&mutex1);
    if (B.Tam == B.TamMax) {
        pthread_cond_wait(&full, &mutex1); // Espera não estar mais cheio o buffer
    }
    pthread_mutex_unlock(&mutex1);
    Insert_Buffer(function, P);
    return B.Tail->ID; // retorna o id sequencial
}

void *Handler() { // Função da Thread Despachante
    List Task;
    bool Escolha;
    while (true) {
        pthread_mutex_lock(&mutex1);
        Escolha = false;
        if (B.Tam == 0) {
            pthread_cond_wait(&empty, &mutex1); // Espera algo ser inserido no buffer
        }
        pthread_mutex_lock(&mutex3);
        Task = Delete_Buffer(); //Coloca a operação deletada do buffer em Task
        // Procura uma thread que não esteja executando
        while (!Escolha) {
            for (int a = 0; a < N && !Escolha; a++) {
                if (Exe[a] == false) {
                    Exe[a] = true; // Thread está executando
                    Task.P.Thread_id = a;
                    pthread_create(&Thread[a], NULL, Task.function, (void *)&Task); // Executa a operação guardada em Task
                    Escolha = true; // Escolheu uma thread para executar
                }
            }
        }
        pthread_mutex_unlock(&mutex1);
    }
}

int pegarResultadoExecucao(int id) {
    pthread_mutex_lock(&mutex3);
    while (!R[id].ready) { // Enquanto o dado desejado não estiver pronto, espera
        pthread_cond_wait(&pronto, &mutex3); // Aguarda um novo dado ser inserido no vetor
    }
    pthread_mutex_unlock(&mutex3);
    R[id].Exe = 1;
    return R[id].R; // Retorna resultado
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
    // Pega as funções de um arquivo
    FILE *input = fopen("entrada.txt", "r+");
    if (input == NULL) {
        printf("falha ao abrir o arquivo de entrada");
        exit(-1);
    }
    char op[4];
    do {
        int id;
        fscanf(input, " %s", op);
        if (strcmp(op, "REQ") == 0) { // Pegar resultado
            fscanf(input, " %d", &id);
            if(R[id].Exe == 0){
                printf("Resultado %d = %d\n", id, pegarResultadoExecucao(id));
            }
            else{
                printf("Resultado já exibido\n");
            }
        } else if (strcmp(op, "MMC") == 0) { // Operação MMC
            fscanf(input, " %d %d", &P.A, &P.B);
            printf("%s %d %d ID: %d\n", op, P.A, P.B, agendarExecucao(mmc, P));
        } else if (strcmp(op, "MDC") == 0) { // Operação MDC
            fscanf(input, " %d %d", &P.A, &P.B);
            printf("%s %d %d ID: %d\n", op, P.A, P.B, agendarExecucao(mdc, P));
        }
    } while (strcmp(op, "END") != 0);
    fclose(input);
    return 0;
}
