#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#define NUM_HIJOS 5

int generar_numero_aleatorio(int semilla)
{
    srand(semilla);
    return rand() % 100; // Genera un número aleatorio entre 0 y 99
}

int main()
{
    int estado_hijos[NUM_HIJOS];
    pid_t pid;

    srand(time(NULL)); // Inicializa la semilla para la generación de números aleatorios

    for (int i = 0; i < NUM_HIJOS; i++)
    {
        pid = fork();

        if (pid == -1)
        {
            perror("Error al crear el proceso hijo");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            int num_aleatorio = generar_numero_aleatorio(getpid());
            printf("Proceso hijo %d generó el número aleatorio: %d\n", getpid(), num_aleatorio);
            exit(num_aleatorio); // Retorna el número aleatorio como estado de terminación
        }
    }

    // ejecutado por el proceso padre
    for (int i = 0; i < NUM_HIJOS; i++)
    {
        wait(&estado_hijos[i]); // Espera a que cada proceso hijo termine y guarda su estado
    }

    printf("Proceso padre recibió los siguientes números aleatorios de sus hijos:\n");
    for (int i = 0; i < NUM_HIJOS; i++)
    {
        printf("Número random del proceso hijo %d: %d\n", i + 1, WEXITSTATUS(estado_hijos[i]));
    }

    return 0;
}
