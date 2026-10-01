/*
Description: This program see how CPU usage changes with the RAM increment, using the Tanenabaum probabilistic model of multiprogramming.
*/

#include <stdio.h>
#include <math.h>

int main(void){
    int ram, os_mem, p_mem, num_proc;
    int inc;
    double u, prob;

    printf("quantidade total de memoria RAM: ");
    scanf("%d", &ram);
    printf("memoria ocupada pelo sistema operacional: ");
    scanf("%d", &os_mem);
    printf("memoria necessaria por processo: ");
    scanf("%d", &p_mem);
    printf("probabilidade p de um processo estar esperando E/S (0 < p < 1): ");
    scanf("%lf", &prob);

    if (ram <= 0 || os_mem < 0 || p_mem <= 0 || prob < 0 || prob > 1 || ram < os_mem) {
        printf("\nentrada invalida\n");
        return 1;
    }

    printf("incremento de memoria: ");
    scanf("%d", &inc);

    if (inc < 0){
        printf("\nerro (incremento negativo)\n");
        return 1;
    }

    printf("i\tram\tgrau n\tuso CPU");
    for (int i = 0; i<=5; i++){

        num_proc = (ram - os_mem)/p_mem; // numero max de processos que podem permanecer simultaneamente em funçao da memoria disponivel
        u = (1 - pow(prob, num_proc)) * 100; // percentual de utilizacao da cpu em funcao do grau de multiprogramacao

        printf("\n%d\t%d\t%d\t%.2f %%", i, ram, num_proc, u);
        ram += inc;
    }
    printf("\n");

    return 0;
}
