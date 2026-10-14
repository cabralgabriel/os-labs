/*

Description: This program create a int sent using the POSIX message queue to accumulate them

Compile with: gcc mqueue-send.c -o send -lrt

Advice: First run receive then run mqueue-send

*/

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <unistd.h>
#include <sys/stat.h>

#define QUEUE "/simple_queue"

int main(int argc, char *argv[]){

    mqd_t queue;                    // fila em si
    int msg;                        // mensagem a enviar (apenas inteiros)
    int rc;                         // return code for open and send calls

    // abre a fila de mensagens se existir
    // entradas: nome da fila definida no cabeçalho ; flags (O_RDWR abre para leitura e escrita)
    queue = mq_open (QUEUE, O_RDWR);
    if (queue < 0){
        // fecha caso tenha ocorrido erro ao abrir a fila
        perror ("mq_open") ;
        exit(1); 
    }

    while(1){

        msg = random() % 100;

        rc = mq_send (queue, (void*)&msg, sizeof(msg), 0);
        if (rc < 0){
            perror("mq_receive:") ;
            exit(1);
        }
        printf ("Sent msg with value %d\n", msg) ;
        sleep(1);
    }
}