/*

Description: This program create N threads. Each thread simply announces when it starts and finishes, printing its ID  and pthread_self() value. 

Compile with: gcc posix-threads.c -o posix-threads -pthread

Run with: ./posix-threads [num-of-threads]

*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *view_thread(void *id){

    long tid = (long) id ; // ID da thread
  
    printf("thread %ld [%p] iniciou\n", tid, (void *)pthread_self());
    sleep(3);
    printf("thread %ld [%p] terminou\n", tid, (void *)pthread_self());
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){

    int n; // num de threads
    long i, status;

    n = atoi(argv[1]); // numero de threads a serem criadas é obtido via linha de comando (segundo argumento)

    pthread_t threads[n]; // armazena os identificadores das threads

    for (i=0; i<n; i++){
        //entradas: endereço da thread ; atributos de configuracao da thread ; funcao que rodará naquela thread ; parametros a serem passados para a funcao que rodará naquela thread
        status = pthread_create(&threads[i], NULL, view_thread, (void*)i);

        if (status != 0){
            printf("thread criada retornou o erro: %ld\n", status);
        }
    }

    for (i=0; i<n; i++){
        //entradas: id da thread; valor de retorno da thread apos execuçao
        pthread_join(threads[i], NULL);
    }

    return 0;
}
