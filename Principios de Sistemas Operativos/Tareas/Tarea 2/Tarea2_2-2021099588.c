#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_MSG_SIZE 256 // Tamaño máximo del mensaje

struct mensaje {
    long tipo;
    char texto[MAX_MSG_SIZE];
};

int main() {
    key_t clave;
    int id_cola;


    if ((clave = ftok(".", 'A')) == -1) {     // Genera la clave única de la cola de mensajes
        perror("Error al generar la clave");
        exit(EXIT_FAILURE);
    }

    // Crear o conectar a la cola de mensajes
    if ((id_cola = msgget(clave, 0666 | IPC_CREAT)) == -1) {
        perror("Error al crear/conectar la cola de mensajes");
        exit(EXIT_FAILURE);
    }

    char *mensajes[] = {"Soy el mensaje 1", "Soy el mensaje 2", "Soy el mensaje 3", "Soy el mensaje 4", "Soy el mensaje 5"};

    for (int i = 0; i < 5; i++) {     // Enviar mensajes a dos procesos diferentes
        struct mensaje msg;
        msg.tipo = (i < 3) ? 1 : 2; // Mover los mensajes a través de los procesos
        strcpy(msg.texto, mensajes[i]);
        
        if (msgsnd(id_cola, &msg, sizeof(struct mensaje) - sizeof(long), 0) == -1) {
            perror("Error al enviar el mensaje");
            exit(EXIT_FAILURE);
        }
    }

    if (fork() == 0) {
        struct mensaje msg;
        // Recibir mensajes del tipo 1
        for (int i = 0; i < 3; i++) {
            if (msgrcv(id_cola, &msg, sizeof(struct mensaje) - sizeof(long), 1, 0) == -1) {
                perror("Error al recibir el mensaje");
                exit(EXIT_FAILURE);
            }
            printf("Proceso 1: Mensaje recibido: %s\n", msg.texto);
        }
        exit(EXIT_SUCCESS);
    }

    if (fork() == 0) {
        struct mensaje msg;
        // Recibir mensajes del tipo 2
        for (int i = 0; i < 2; i++) {
            if (msgrcv(id_cola, &msg, sizeof(struct mensaje) - sizeof(long), 2, 0) == -1) {
                perror("Error al recibir el mensaje");
                exit(EXIT_FAILURE);
            }
            printf("Proceso 2: Mensaje recibido: %s\n", msg.texto);
        }
        exit(EXIT_SUCCESS);
    }
    // Esperar a que los procesos hijos terminen
    for (int i = 0; i < 2; i++) {
        wait(NULL);
    }

    // Eliminar la cola de mensajes
    if (msgctl(id_cola, IPC_RMID, NULL) == -1) {
        perror("Error al eliminar la cola de mensajes");
        exit(EXIT_FAILURE);
    }

    return 0;
}

