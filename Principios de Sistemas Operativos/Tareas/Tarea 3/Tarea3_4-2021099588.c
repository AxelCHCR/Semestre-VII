#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_EMPLEADOS 10

typedef struct {
    char nombre[50];
    int edad;
    float salario;
} Empleado;

int main() {
    FILE *archivo;
    Empleado empleado;
    int fd;
    int i;

    // Abrir archivo en modo escritura binaria
    archivo = fopen("empleados.dat", "wb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    // Obtener el descriptor de archivo
    fd = fileno(archivo);

    // Agregar empleados de forma incremental
    for (i = 0; i < MAX_EMPLEADOS; i++) {
        printf("Ingrese nombre del empleado %d: ", i + 1);
        scanf("%s", empleado.nombre);
        printf("Ingrese edad: ");
        scanf("%d", &empleado.edad);
        printf("Ingrese salario: ");
        scanf("%f", &empleado.salario);

        // Bloquear el archivo para escritura
        flock(fd, LOCK_EX);

        // Escribir datos del empleado en el archivo
        fwrite(&empleado, sizeof(Empleado), 1, archivo);

        // Desbloquear el archivo
        flock(fd, LOCK_UN);

        sleep(2); // Esperar 2 segundos
    }

    fclose(archivo);
    printf("Datos de empleados guardados en el archivo.\n");

    // Abrir archivo en modo lectura binaria
    archivo = fopen("empleados.dat", "rb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    // Obtener el descriptor de archivo
    fd = fileno(archivo);

    while (1) {
        // Bloquear el archivo para lectura
        flock(fd, LOCK_SH);

        // Leer datos del empleado desde el inicio del archivo
        rewind(archivo);
        while (fread(&empleado, sizeof(Empleado), 1, archivo) == 1) {
            printf("Nombre: %s\n", empleado.nombre);
            printf("Edad: %d\n", empleado.edad);
            printf("Salario: %.2f\n", empleado.salario);
            printf("\n");
        }

        // Desbloquear el archivo
        flock(fd, LOCK_UN);

        sleep(2); // Esperar 2 segundos
    }

    fclose(archivo);

    return 0;
}