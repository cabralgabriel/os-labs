#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void tarefa_leitura(){
    printf("[leitura] iniciando\n");
    sleep(2); // simula E/S
    printf("[leitura] concluida\n");
}

void tarefa_processamento() {
    int cont = 0;
    printf("[processamento] iniciando\n");
    for (int i = 0; i < 999999999; i++){ // simula processamento
        cont++;
    }
    printf("[processamento] concluido\n");
}

void tarefa_gravacao() {
    printf("[gravacao] iniciando\n");
    sleep(3); // simula E/S
    printf("[gravacao] concluida\n");
}

void *thread_leitura(void *arg) {
    tarefa_leitura();
    pthread_exit(NULL);
}

void *thread_processamento(void *arg) {
    tarefa_processamento();
    pthread_exit(NULL);
}

void *thread_gravacao(void *arg) {
    tarefa_gravacao();
    pthread_exit(NULL);
}

int main(void){
    struct timespec inicio, fim;
    double tempo_sequencial, tempo_paralelo;
    pthread_t t1, t2, t3;

    // - Computacao Sequencial - 
    printf("[versao A] sequencial\n");
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    tarefa_leitura();
    tarefa_processamento();
    tarefa_gravacao();

    clock_gettime(CLOCK_MONOTONIC, &fim);

    tempo_sequencial = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    printf("[versao A] tempo total: %.3f segundos\n", tempo_sequencial);


    // - Computacao paralela - 
    printf("[versao B] multithread\n");
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    
    pthread_create(&t1, NULL, thread_leitura, NULL);
    pthread_create(&t2, NULL, thread_processamento, NULL);
    pthread_create(&t3, NULL, thread_gravacao, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    clock_gettime(CLOCK_MONOTONIC, &fim);

    tempo_paralelo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    printf("[versao B] tempo total: %.3f segundos\n", tempo_paralelo);

    return 0;
}