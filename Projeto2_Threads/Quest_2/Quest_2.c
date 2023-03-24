#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct lista{
    int v;
    struct lista *next;
} List;

List *Grafo;
int DeadLock = 0;
int Nos;

void Visit(bool *Visited, int Origem, int id){ //Algoritmo DFS
    Visited[Origem] = true;
    List *Busca = Grafo[Origem].next;
    while (Busca != NULL){
        int val = Busca->v;
        if (val == id){ //Se encontrou deadlock incrementa a variavel DeadLock
            DeadLock++;
        }
        if (Visited[val] == false){
            Visit(Visited, val, id);
        }
        Busca = Busca->next;
    }
}

void *find_deadlock(void *Id){
    int id = *((int *)Id);
    bool *Visited;
    Visited = (bool *)calloc(Nos, sizeof(bool));
    Visit(Visited, id, id);
}

int main(){
    int No1, No2, arestas;
    List *print, *new;
    scanf("%d", &Nos); // Leitura do número de nós
    int ThreadID[Nos];
    pthread_t Thread[Nos];
    scanf("%d", &arestas); // Leitura do número de arestas
    Grafo = (List *)malloc(Nos * sizeof(List)); // Aloca os nós do grafo
    for (int a = 0; a < Nos; a++){
        Grafo[a].next = NULL;
    }
    for (int a = 0; a < arestas; a++){ // Inicializa a lista de adjascências
        scanf("%d %d", &No1, &No2);
        new = (List *)malloc(sizeof(List));
        new->v = No2;
        new->next = Grafo[No1].next;
        Grafo[No1].next = new;
        new = NULL;
    }
    for (int a = 0; a < Nos; a++){ //Cria as threads
        ThreadID[a] = a;
        pthread_create(&Thread[a], NULL, find_deadlock, (void *)&ThreadID[a]);
    }
    for (int a = 0; a < Nos; a++){ //Espera as threads terminarem a execução
        pthread_join(Thread[a], NULL);
    }
    if (DeadLock == 0){ //Não encontrou DeadLock
        printf("DeadLock nao detectado\n");
    }
    else{ //Encontrou DeadLock
        printf("DeadLock detectado\n");
    }
    return 0;
}
