/*

Description: 

Compile with:

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define NUM_PROD 5
#define NUM_CONS 3
#define SLOTS 6

sem_t slots, items, mutex;
int num_slots, num_items;

int buffer[SLOTS];
int in = 0, out = 0;

void *produce (void *id){

    long tid = (long) id;
    int item;

    while (1){

        item = random() % 100;  // *produz item
        sem_wait(&slots);       // ao produzir um item, ele ocupa um slot, perdendo uma vaga
        sem_wait(&mutex);       // abre exclusao mutua (mutex) para colocar item na memoria
        
        // seçao critica
        buffer[in] = item;
        printf("[P%d] stored an item: %d (%d items | %d slots)\n", tid, item, num_items, num_slots);
        in = (in + 1) % SLOTS;

        num_items++;
        num_slots--;
        // fim da seçao critica

        sem_post(&mutex);       // fecha exclusao mutua
        sem_post(&items);       // desbloqueia consumo de itens depois da produçao dos itens
    
        sleep(2);
    }
}

void *consume (void *id){

    long tid = (long) id;
    int item;

    while (1){

        sem_wait(&items);       // mantem consumo de itens bloqueado enquanto nao houver itens 
        sem_wait(&mutex);       // exclusao mutua (mutex) para retirar item na memoria
        
        // seçao critica
        num_slots++;            // *coloca item
        item = buffer[out];
        printf("[C%d] consum an item: %d (%d items | %d slots)\n", tid, item, num_items, num_slots);
        out = (out + 1) % SLOTS;
        // fim da seçao critica
        sem_post(&mutex);       // fecha exclusao mutua
        sem_post(&slots);       // ao consumir um item, abrimos mais uma vaga (slot)

        num_items--;            // *consome item
        sleep(2);
    }
}

int main(int argc, char *argv[]){

    pthread_t producers[NUM_PROD];
    pthread_t consumers[NUM_CONS];
    long i, status;

    num_items = 0;
    num_slots = SLOTS;

    // inicia semaforos
    sem_init(&mutex, 0, 1);
    sem_init(&slots, 0, SLOTS);
    sem_init(&items, 0, 0);

    // cria produtores em threads separadas
    for (i=0; i<NUM_PROD; i++){
        status = pthread_create(&producers[i], NULL, produce, (void*)i);
        if (status != 0){
            perror("pthread_create");
            exit(1);
        }
    }

    // cria consumidores em threads separadas
    for (i=0; i<NUM_CONS; i++){
        status = pthread_create(&consumers[i], NULL, consume, (void*)i);
        if (status != 0){
            perror("pthread_create");
            exit(1);
        }
    }

    pthread_exit(NULL);
}