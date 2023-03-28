#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define B 10 // tamanho do buffer
#define P 10 // quantidade de produtos
#define C 10 // quantidade de consumidores

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // inicializa o mutex
pthread_cond_t empty, full = PTHREAD_COND_INITIALIZER; // inicializa as variáveis de condição

typedef struct elem {
    int value;
    struct elem *prox;
} Elem;

typedef struct blockingQueue {
    unsigned int sizeBuffer, statusBuffer;
    Elem *head, *last;
} BlockingQueue;

BlockingQueue *Q; // cria uma fila bloqueante

BlockingQueue *newBlockingQueue(unsigned int SizeBuffer) {
    BlockingQueue *queue = (BlockingQueue *)malloc(sizeof(BlockingQueue));
    queue->sizeBuffer = SizeBuffer;
    queue->statusBuffer = 0;
    queue->head = NULL;
    queue->last = NULL;
    return queue;
}

void putBlockingQueue(BlockingQueue *Q, int newValue) {
    pthread_mutex_lock(&mutex);  // trava o mutex
    while (Q->statusBuffer == Q->sizeBuffer) { // enquanto o buffer estiver cheio
        printf("FILA CHEIA, produtor está aguardando...\n"); // imprime mensagem de ocupado
        pthread_cond_wait(&empty, &mutex); // produtor aguarda
    }
    Elem *new = (Elem *)malloc(sizeof(Elem));
    new->value = newValue;
    new->prox = NULL;
    if (Q->last == NULL) {
        Q->head = new;
        Q->last = new;
    } else {
        Q->last->prox = new;
        Q->last = new;
    }
    Q->statusBuffer++;
    if (Q->statusBuffer == 1) {
        pthread_cond_broadcast(&full); // sinaliza a variável de condição full
    }
    pthread_mutex_unlock(&mutex); // destrava o mutex
}

int takeBlockingQueue(BlockingQueue *Q) {
    pthread_mutex_lock(&mutex); // trava o mutex
    while (Q->statusBuffer == 0) { // enquanto o buffer estiver vazio
        printf("FILA VAZIA, consumidor está aguardando...\n"); // imprime mensagem de ocupado
        pthread_cond_wait(&full, &mutex); // consumidor aguarda
    }
    int value = Q->head->value;
    Elem *aux = Q->head;
    Q->head = aux->prox;
    free(aux);
    Q->statusBuffer--;
    if (Q->head == NULL) {
        Q->last = NULL;
    }
    if (Q->statusBuffer == Q->sizeBuffer - 1) {
        pthread_cond_signal(&empty); // sinaliza a variável de condição empty
    }
    pthread_mutex_unlock(&mutex); // destrava o mutex
    return value;
}

void *producer(void *threadID) {
    int id = *((int *)threadID);
    int amount = 0;
    while (1) {
        putBlockingQueue(Q, amount); // enfileira o valor produzido
        printf("Produtor %d produziu: %d\n", id, amount); // imprime o id do produtor e o valor produzido
        amount++;
    }
    pthread_exit(NULL);
}

void *consumer(void *threadID) {
    int id = *((int *)threadID);
    while (1) {
        int value = takeBlockingQueue(Q); // desenfileira o valor consumido
        printf("Consumidor %d consumiu: %d\n", id, value); // imprime o id do consumidor e o valor consumido
    }
    pthread_exit(NULL);
}

int main() {
    Q = newBlockingQueue(B); // cria uma fila bloqueante
    pthread_t producers[P];  // cria um vetor de threads para os produtores
    pthread_t consumers[C];  // cria um vetor de threads para os consumidores
    int *idsProducer[P]; // vetor de ids dos produtores
    int *idsConsumer[C]; // vetor de ids dos consumidores
    int i;
    for (i = 0; i < P; i++) {
        idsProducer[i] = (int *)malloc(sizeof(int)); // aloca memória para os ids dos produtores
        *idsProducer[i] = i; // define a id do produtor como o valor de i 
        pthread_create(&producers[i], NULL, producer, (void *)idsProducer[i]); // cria as threads dos produtores
    }
    for (i = 0; i < C; i++) {
        idsConsumer[i] = (int *)malloc(sizeof(int)); // aloca memória para os ids dos consumidores
        *idsConsumer[i] = i; //define a id do consumidor como o valor de i
        pthread_create(&consumers[i], NULL, consumer, (void *)idsConsumer[i]); // cria as threads dos consumidores
    }
    for (i = 0; i < P; i++) {
        pthread_join(producers[i], NULL); // aguarda finalização do thread produtor i
    }
    for (i = 0; i < C; i++) {
        pthread_join(consumers[i], NULL); // aguarda finalização do thread consumidor i
    }
    free(Q);
    return 0;
}