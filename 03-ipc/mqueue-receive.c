/*

Description: This program create a int receiver using the POSIX message queue and accumulate them

Compile with: gcc mqueue-receive.c -o receive -lrt

Advice: First run receive then go to mqueue-send.c

*/

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>

#define QUEUE "/simple_queue"

int main(int argc, char *argv[]){

    mqd_t queue;                    // fila em si
    struct mq_attr attr;            // cria atributos da fila
    int msg;                        // mensagem recebida (apenas inteiros)
    int total = 0;                  // total do lado do 
    int rc;                         // return code for open and receive calls

    attr.mq_maxmsg = 10;            // define capacidade max de mensagens
    attr.mq_msgsize = sizeof(msg);  // tamanho de cada mensagem
    attr.mq_flags = 0;

    // cria fila
    // entradas: nome da fila definida no cabeçalho ; flags de controle (O_CREAT cria e O_RDWR abre para leitura e escrita) ; permissoes ; endereço dos atributos
    queue = mq_open(QUEUE, O_CREAT | O_RDWR, 0666, &attr);
    if (queue < 0){
        perror ("mq_open") ;
        exit(1); // fecha caso tenha ocorrido erro ao abrir a fila
    }

    while(1){
        // entradas: fila; ponteiro para buffer; tamanho da mensagem ; prioridade 
        rc = mq_receive(queue, (void*)&msg, sizeof(msg), 0);
        if (rc < 0){
            perror("mq_receive:") ;
            exit(1);
        }
        total += msg;
        printf ("Received msg value %d | Accumulated total: %d\n", msg, total) ;
    }
}