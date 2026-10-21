/*

Description: Peterson's algorithm for mutual exclusion with busy waiting.

Compile with: gcc mutual-exclusion-peterson.c -o peterson -lpthread

Run with: ./peterson

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_THREADS 2
#define NUM_INCREMENTOS 1000000

// volatile previne otimizações do compilador na espera ocupada
volatile int turn = 0;           // indica de quem é a vez
volatile int wants[2] = {0, 0};  // indica se a tarefa i quer acessar a seçao critica

int contador = 0;       // contador para comparar com o num de incrementos
int dentro_cs = 0;      // flag para indicar que esta dentro da critical section
int violacoes = 0;      // incrementa quando critical section foi violada

void enter (int task){                      // como o algoritmo de peterson so é válido para 2 threads, task vale 0 ou 1
    int other = 1 - task;                   // indica a outra tarefa
    wants[task] = 1;                        // task quer acessar a seçao critica
    turn = task;                            // concede a vez ao outro

    __sync_synchronize();                   // introduz barreira de memoria
    
    while ((turn == task) && wants[other]); // espera ocupada
}

void leave (int task){
    wants[task] = 0;                        // task libera a seçao critica
}

void *thread_peterson(void *id) {
    long tid = (long) id;

    for (int i = 0; i < NUM_INCREMENTOS; i++) {
        enter(tid);

        // seçao critica
        dentro_cs++;    // registra que entrou na critical section
        
        // verifica se ha violaçao de exclusao mutua
        if (dentro_cs > 1) {
            violacoes++;
        }
        
        contador++;
        
        dentro_cs--;    // registra saida da critical section
        // fim da seçao critica

        leave(tid);
    }
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];
    long i, status;
    struct timespec inicio, fim;
    double tempo_total;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (i = 0; i < NUM_THREADS; i++) {
        //entradas: endereço da thread ; atributos de configuracao da thread ; funcao que rodará naquela thread ; parametros a serem passados para a funcao que rodará naquela thread
        status = pthread_create(&threads[i], NULL, thread_peterson, (void*)i);
        if (status != 0) {
            perror("pthread_create");
            exit(1);
        }
    }

    for (i = 0; i < NUM_THREADS; i++) {
        //entradas: id da thread; valor de retorno da thread apos execuçao
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    tempo_total = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("valor esperado: 2000000 | valor obtido: %d\n", contador);
    printf("violacoes detectadas: %d\n", violacoes);
    printf("tempo de execucao: %.6f segundos\n", tempo_total);

    return 0;
}
