#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N_HILOS 10 //Número de hilos a crear

pthread_t tids[N_HILOS];

void* thread_func(void* arg) {
    int thread_num = *((int*)arg);
    free(arg);
    int s = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    if (s != 0) {
        perror("pthread_setcanceltype");
        exit(EXIT_FAILURE);
    }

    while (1) {     // Mientras el hilo no muera 
        printf("Hilo %d: Sigo en pie.\n", thread_num);
        sleep(2);
        int random = rand() % N_HILOS + 1;
        if (thread_num != random) {
            pthread_cancel(tids[random - 1]);
            printf("Hilo %d: Me eche al hilo %d.\n", thread_num, random);
        }
    }

    return NULL;
}

int main() {
    int t;

    for (int i = 0; i < N_HILOS; i++) {     // Creando los hilos
        int* arg = malloc(sizeof(int));
        if (arg == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        *arg = i + 1;
        t = pthread_create(&tids[i], NULL, thread_func, arg);
        if (t != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Ciclo de espera
    for (int i = 0; i < N_HILOS; i++) {
        void* res;
        t = pthread_join(tids[i], &res);
        if (t != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
        if (res == PTHREAD_CANCELED) {
            printf("Reporte: El hilo %d fue cancelado (%lu)\n", i + 1, tids[i]);
        }
    }

    return 0;
}