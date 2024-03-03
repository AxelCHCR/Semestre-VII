#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define NUM_HIJOS 5
void manejar_cancelacion(int signum)
{
    printf("Hijo %d cancelado.\n", getpid());
    exit(0);
}

int main()
{
    signal(SIGTERM, manejar_cancelacion);
    for (int i = 0; i < NUM_HIJOS; ++i)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("Error al crear hijo");
            exit(1);
        }
        else if (pid == 0)
        {
            printf("Hijo %d creado.\n", getpid());
            // entrar en bucle esperando cancelación
            while (1)
            {
                sleep(1);
            }
        }
    }
    printf("Todos los hijos creados. Enviando señales de cancelación...\n");

    for (int i = 0; i < NUM_HIJOS; ++i)
    {
        kill(0, SIGTERM);
        sleep(1);
    }

    for (int i = 0; i < NUM_HIJOS; ++i)
    {
        wait(NULL);
    }

    printf("Todos los hijos han sido cancelados.\n");

    return 0;
}
