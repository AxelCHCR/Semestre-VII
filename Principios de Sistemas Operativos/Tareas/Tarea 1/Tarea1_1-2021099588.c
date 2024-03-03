#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_HIJOS 5
#define NIVELES 3

void crearProcesos(int nivel, int numProcesos) {
    for (int i = 0; i < numProcesos; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Proceso hijo
            printf("Nivel: %d, PID: %d, PID del padre: %d\n", nivel, getpid(), getppid());
            if (nivel < NIVELES - 1) {
                crearProcesos(nivel + 1, NUM_HIJOS);
            }
            exit(EXIT_SUCCESS);
        }
    }
    for (int i = 0; i < numProcesos; i++) {
        wait(NULL);
    }
}

int main() {
    printf("Proceso padre, PID: %d\n", getpid());
    crearProcesos(0, NUM_HIJOS);
    return 0;
}
