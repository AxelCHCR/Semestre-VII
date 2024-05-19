#include <unistd.h> // Añadido para la declaración de sbrk
#include <stdio.h>  
#include <stdlib.h>
#include <stddef.h>

#define NALLOC 1024 // mínimo de unidades por requerir
typedef long Align; // alineamiento al límite superior
union header {      // encabezado de bloque
  struct {
    union header *ptr; // siguiente bloque
    unsigned size;     // tamaño del bloque
  } s;
  Align x; // alineamiento de bloque
};

typedef union header Header;
static Header base;          
static Header *freep = NULL; 
static Header *lastp = NULL; // último bloque asignado

// free: coloca el bloque ap en la lista vacía
void free(void *ap) {
  Header *bp, *p;

  bp = (Header *)ap - 1; // Apunta al encabezado de un bloque
  for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break; // libera bloque al inicio o al final

  if (bp + bp->s.size == p->s.ptr) { // fusiona al nbr superior
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  } else
    bp->s.ptr = p->s.ptr; // fusiona al nbr inferior
  if (p + p->s.size == bp) {
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
  } else
    p->s.ptr = bp;
  freep = p;
}


// morecore: solicita más memoria del sistema
static Header *morecore(unsigned nu) {
  void *cp;
  Header *up;
  if (nu < NALLOC)
    nu = NALLOC;
  cp = sbrk(nu * sizeof(Header));
  if (cp == (char *)-1) // no hay espacio en absoluto
    return NULL;
  up = (Header *)cp;
  up->s.size = nu;
  free((void *)(up + 1));
  return freep;
}

// malloc: asignador de memoria
void *malloc(unsigned long nbytes) {
  Header *p, *prevp;
  unsigned nunits;

  nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;
  if ((prevp = freep) == NULL) { // no hay lista libre
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  if (lastp == NULL) {
    lastp = freep;
  }
  for (p = lastp;; prevp = p, p = p->s.ptr) {
    if (p->s.size >= nunits) { // suficiente grande
      if (p->s.size == nunits) // exacto
        prevp->s.ptr = p->s.ptr;
      else {
        p->s.size -= nunits;
        p += p->s.size;
        p->s.size = nunits;
      }
      lastp = prevp;
      return (void *)(p + 1);
    }
    if (p == lastp) { // dio la vuelta a la lista libre
      if ((p = morecore(nunits)) == NULL)
        return NULL; // nada libre
    }
  }
}

int main() {
  int *arr1 = (int *)malloc(10 * sizeof(int));
  int *arr2 = (int *)malloc(20 * sizeof(int));
  int *arr3 = (int *)malloc(5 * sizeof(int));

  if (arr1 != NULL) {
    printf("Memory allocated for arr1\n");
  }
  if (arr2 != NULL) {
    printf("Memory allocated for arr2\n");
  }
  if (arr3 != NULL) {
    printf("Memory allocated for arr3\n");
  }

  free(arr1);
  printf("Memory freed for arr1\n");

  int *arr4 = (int *)malloc(15 * sizeof(int));
  if (arr4 != NULL) {
    printf("Memory allocated for arr4\n");
  }

  free(arr2);
  free(arr3);
  free(arr4);

  return 0;
}