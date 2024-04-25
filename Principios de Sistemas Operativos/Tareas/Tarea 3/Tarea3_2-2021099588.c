#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

void mostrarDirectorio(char *path, int nivel);

int main() {
    char directorio[256];
    printf("Ingrese el directorio a explorar: ");
    scanf("%s", directorio);

    mostrarDirectorio(directorio, 0);

    return 0;
}

void mostrarDirectorio(char *path, int nivel) {
    DIR *dir;
    struct dirent *entrada;
    struct stat estadoArchivo;
    char rutaCompleta[512];

    dir = opendir(path);
    if (dir == NULL) {
        printf("No se pudo abrir el directorio %s\n", path);
        return;
    }

    while ((entrada = readdir(dir)) != NULL) {
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0)
            continue;

        sprintf(rutaCompleta, "%s/%s", path, entrada->d_name);

        for (int i = 0; i < nivel; i++)
            printf("    ");

        printf("%s", entrada->d_name);

        if (lstat(rutaCompleta, &estadoArchivo) == 0) {
            if (S_ISDIR(estadoArchivo.st_mode)) {
                printf("/\n");
                mostrarDirectorio(rutaCompleta, nivel + 1);
            } else {
                printf(" (");
                printf("%ld bytes, ", estadoArchivo.st_size);
                printf("Creado: %s", ctime(&estadoArchivo.st_ctime));
                printf(")\n");
            }
        } else {
            printf(" (Error al obtener información del archivo)\n");
        }
    }

    closedir(dir);
}