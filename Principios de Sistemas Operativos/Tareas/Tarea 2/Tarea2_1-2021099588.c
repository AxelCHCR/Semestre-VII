#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define NUM_PROCESOS 10 //numero de procesos a crear

typedef struct {
    int num1;
    int num2;
} Numeros;

int main() {
    // Crea la memoria compartida
    Numeros *numeros_compartidos = mmap(NULL, sizeof(Numeros), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (numeros_compartidos == MAP_FAILED) {
        perror("Error al crear memoria compartida");
        exit(EXIT_FAILURE);
    }

    // Crea procesos hijos
    for (int i = 0; i < NUM_PROCESOS; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Error al crear al hijo");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Proceso hijo
            srand(getpid());
            numeros_compartidos->num1 = rand() % 100;
            numeros_compartidos->num2 = rand() % 100;

            // Sumar los números y mostrar el resultado
            int resultado = numeros_compartidos->num1 + numeros_compartidos->num2;
            printf("Proceso hijo %d: %d + %d = %d\n", getpid(), numeros_compartidos->num1, numeros_compartidos->num2, resultado);
            exit(EXIT_SUCCESS);
        }
    }

    // Esperar a que todos los procesos hijos terminen
    for (int i = 0; i < NUM_PROCESOS; i++) {
        wait(NULL);
    }

    // Liberar la memoria compartida
    if (munmap(numeros_compartidos, sizeof(Numeros)) == -1) {
        perror("Error al liberar memoria compartida");
        exit(EXIT_FAILURE);
    }

    return 0;
}
