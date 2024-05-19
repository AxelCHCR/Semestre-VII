#include <stdio.h>
#define ALLOCSIZE 10000 

static char allocbuf[ALLOCSIZE]; // almacenamiento para alloc
static char *allocp1 = allocbuf;  // siguiente posición libre para el área 1 (de abajo hacia arriba)
static char *allocp2 = allocbuf + ALLOCSIZE;  // siguiente posición libre para el área 2 (de arriba hacia abajo)

char *alloc(int n, int i) { // regresa un puntero a n caracteres
  if (i == 1) { // área 1: de abajo hacia arriba
    if (allocp1 + n <= allocp2) { // verificar que no se crucen
      allocp1 += n;
      return allocp1 - n;
    } else {
      return 0; // no hay suficiente espacio
    }
  } else if (i == 2) { // área 2: de arriba hacia abajo
    if (allocp2 - n >= allocp1) { // verificar que no se crucen
      allocp2 -= n;
      return allocp2;
    } else {
      return 0; // no hay suficiente espacio
    }
  } else {
    return 0; // valor de i inválido
  }
}

void afree(char *p, int i) { // libera la memoria apuntada por p
  if (i == 1) { 
    if (p >= allocbuf && p < allocbuf + ALLOCSIZE) {
      allocp1 = p;
    }
  } else if (i == 2) {
    if (p >= allocbuf && p < allocbuf + ALLOCSIZE) {
      allocp2 = p + ALLOCSIZE;
    }
  }
}

// test_alloc.c


int main() {
  char *p1, *p2, *p3, *p4;
 //Asignacion de memoria en todas las areas
  p1 = alloc(1000, 1);
  if (p1 != NULL) {
    printf("Área 1: Asignación de 1000 bytes exitosa.\n");
  } else {
    printf("Área 1: Asignación de 1000 bytes fallida.\n");
  }

  p2 = alloc(2000, 2);
  if (p2 != NULL) {
    printf("Área 2: Asignación de 2000 bytes exitosa.\n");
  } else {
    printf("Área 2: Asignación de 2000 bytes fallida.\n");
  }

  p3 = alloc(8000, 1);
  if (p3 != NULL) {
    printf("Área 1: Asignación de 8000 bytes exitosa.\n");
  } else {
    printf("Área 1: Asignación de 8000 bytes fallida.\n");
  }

  afree(p1, 1);
  printf("Área 1: Memoria de 1000 bytes liberada.\n");

  p4 = alloc(5000, 2);
  if (p4 != NULL) {
    printf("Área 2: Asignación de 5000 bytes exitosa.\n");
  } else {
    printf("Área 2: Asignación de 5000 bytes fallida.\n");
  }

  return 0;
}