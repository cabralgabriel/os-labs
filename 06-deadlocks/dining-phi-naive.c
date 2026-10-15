/*

Description: Naive solution to the Dining Philosofers problem proposed by Dijkstra.
This can work for a time but deadlock can still occur.

Compile with: gcc dining-phi-naive.c -o phi-naive -lpthread

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define PHI_NUM 5       // 5 filosofos e 5 garfos

pthread_t phi[PHI_NUM]; // um thread para cada filosofo
sem_t ffork[PHI_NUM];    // um semaforo para cada garfo

void think (int id){
    printf("P%d thinks\n", id);
    sleep(3);
}

void eat (int id){
    printf("P%d eat\n", id);
    sleep(1);
}

void *philosopher (void *id){

    long tid = (long) id ; // ID da thread
    int dir = tid;
    int esq = (tid+1) % PHI_NUM;

    while (1){
        think(tid);

        sem_wait(&ffork[dir]);   // pega garfo da direita
        sem_wait(&ffork[esq]);   // pega garfo da esquerda

        eat(tid);

        sem_post(&ffork[dir]);   // devolve garfo da direita
        sem_post(&ffork[esq]);   // devolve garfo da esquerda
    }
}

int main (int argc, char *argv[]){

    long i, status;

    for (i = 0; i<PHI_NUM; i++){
        sem_init(&ffork[i], 0, 1); // cada semaforo inicia disponivel
    }

    for (i = 0; i<PHI_NUM; i++){

        status = pthread_create(&phi[i], NULL, philosopher, (void*)i);

        if (status != 0){
            perror("pthread_create");
            exit(1);
        }
    }

    pthread_exit(NULL);
}