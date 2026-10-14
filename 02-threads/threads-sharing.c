/*

Description: Program that creates 4 threads to illustrate shared vs. private memory.

Compile with: gcc threads-sharing.c -o threads-sharing -pthread

Run with: ./threads-sharing

*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_OF_THREADS 4

int contador_global = 0;

void *contador_threads(void *id){
    long tid = (long)id;

    int contador_local = 0;

    for (int i = 0; i < 100000; i++){
        contador_local++;
        contador_global++;
    }
    
    printf("thread %ld:\n", tid);
    printf("contador_local  [%p] =  %d\n", (void*)&contador_local, contador_local);
    printf("contador_global [%p] =  %d\n\n", (void*)&contador_global, contador_global);

    pthread_exit(NULL);
}

int main (void){

    pthread_t threads[NUM_OF_THREADS]; // armazena os identificadores das threads
    long i, status;

    for (i = 0; i < NUM_OF_THREADS; i++){
        //entradas: endereço da thread ; atributos de configuracao da thread ; funcao que rodará naquela thread ; parametros a serem passados para a funcao que rodará naquela thread
        status = pthread_create(&threads[i], NULL, contador_threads, (void*)i);

        if (status != 0){
            printf("thread criada retornou o erro: %ld\n", status);
        }
    }

     for (i = 0; i < NUM_OF_THREADS; i++){
        pthread_join(threads[i], NULL); //entradas: id da thread; valor de retorno da thread apos execuçao
    }

    printf("resultado final:\n");
    printf("contador_global [%p] =  %d\n\n", (void*)&contador_global, contador_global);

    return 0;
}