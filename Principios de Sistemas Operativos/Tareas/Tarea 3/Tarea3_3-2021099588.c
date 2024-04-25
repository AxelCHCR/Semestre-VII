#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EMPLEADOS 10

typedef struct {
    char nombre[50];
    int edad;
    float salario;
} Empleado;

void guardarEmpleados(Empleado empleados[]) {
    FILE *archivo;
    archivo = fopen("empleados.dat", "wb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    fwrite(empleados, sizeof(Empleado), MAX_EMPLEADOS, archivo);
    fclose(archivo);
    printf("Datos de empleados guardados en el archivo.\n");
}

void leerEmpleadosImpares() {
    FILE *archivo;
    Empleado empleado;
    int i;

    archivo = fopen("empleados.dat", "rb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    for (i = 0; i < MAX_EMPLEADOS; i++) {
        fseek(archivo, i * sizeof(Empleado), SEEK_SET);
        fread(&empleado, sizeof(Empleado), 1, archivo);
        if (i % 2 == 0) {
            printf("Empleado %d:\n", i + 1);
            printf("Nombre: %s\n", empleado.nombre);
            printf("Edad: %d\n", empleado.edad);
            printf("Salario: %.2f\n", empleado.salario);
            printf("\n");
        }
    }

    fclose(archivo);
}

int main() {
    Empleado empleados[MAX_EMPLEADOS];
    int i;

    for (i = 0; i < MAX_EMPLEADOS; i++) {
        printf("Ingrese nombre del empleado %d: ", i + 1);
        fgets(empleados[i].nombre, sizeof(empleados[i].nombre), stdin);
        empleados[i].nombre[strcspn(empleados[i].nombre, "\n")] = '\0';
        printf("Ingrese edad: ");
        scanf("%d", &empleados[i].edad);
        printf("Ingrese salario: ");
        scanf("%f", &empleados[i].salario);
        getchar();
    }

    guardarEmpleados(empleados);
    leerEmpleadosImpares();

    return 0;
}