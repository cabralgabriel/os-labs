/*

Description: Program that solves the race condition using a POSIX mutex.

Compile with: gcc critical-section-mutex.c -o mutex -lpthread

Run with: ./mutex

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_THREADS 2
#define NUM_INCREMENTS 1000000

int saldo = 0;
pthread_mutex_t mutex_saldo; // declaracao do trinco de exclusao mutua

void *incrementar_saldo(void *id){
    long tid = (long) id;

    for (int i = 0; i < NUM_INCREMENTS; i++){
        
        pthread_mutex_lock(&mutex_saldo); // abre exclusao mutua (bloqueia outras threads)
        
        // seçao critica
        saldo++;
        // fim da seçao critica
        
        pthread_mutex_unlock(&mutex_saldo); // fecha exclusao mutua (liberta a proxima thread)
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){

    pthread_t threads[NUM_THREADS];
    long i, status;
    struct timespec inicio, fim;
    double tempo_total;

    // inicializa o mutex
    pthread_mutex_init(&mutex_saldo, NULL);

    // inicia a contagem do tempo
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (i = 0; i < NUM_THREADS; i++){
        //entradas: endereço da thread ; atributos de configuracao da thread ; funcao que rodará naquela thread ; parametros a serem passados para a funcao que rodará naquela thread
        status = pthread_create(&threads[i], NULL, incrementar_saldo, (void*)i);

        if (status != 0){
            perror("pthread_create");
            exit(1);
        }
    }

    for (i = 0; i < NUM_THREADS; i++){
        //entradas: id da thread; valor de retorno da thread apos execuçao
        pthread_join(threads[i], NULL);
    }

    // finaliza a contagem do tempo
    clock_gettime(CLOCK_MONOTONIC, &fim);
    tempo_total = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    // destroi o mutex
    pthread_mutex_destroy(&mutex_saldo);

    printf("valor esperado: %d | valor obtido: %d\n", NUM_INCREMENTS * NUM_THREADS, saldo);
    printf("tempo de execucao: %.6f segundos\n", tempo_total);

    return 0;
}

