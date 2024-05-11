#include <stdio.h>
#include <stdlib.h>

// Estructura para representar un bloque de memoria
typedef struct MemBlock {
    void* address; // Dirección del bloque
    size_t size; // Tamaño del bloque
    struct MemBlock* next; // Puntero al siguiente bloque
} MemBlock;

// Puntero al inicio de la lista de bloques asignados
MemBlock* allocatedBlocks = NULL;

// Puntero al inicio de la lista de bloques libres
MemBlock* freeBlocks = NULL;

// Función para agregar un bloque a una lista
void addBlock(MemBlock** list, void* address, size_t size) {
    MemBlock* newBlock = (MemBlock*)malloc(sizeof(MemBlock));
    newBlock->address = address;
    newBlock->size = size;
    newBlock->next = *list;
    *list = newBlock;
}

// Función para eliminar un bloque de una lista
void removeBlock(MemBlock** list, void* address) {
    MemBlock *temp = *list, *prev = NULL;
    while (temp != NULL && temp->address != address) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) return; // No se encontró el bloque
    if (prev == NULL) *list = temp->next; // El bloque está al inicio
    else prev->next = temp->next; // El bloque está en medio o al final
    free(temp);
}

// Función para imprimir los bloques de una lista
void printBlocks(const char* title, MemBlock* list) {
    printf("%s\n", title);
    while (list != NULL) {
        printf("Dirección: %p, Tamaño: %zu\n", list->address, list->size);
        list = list->next;
    }
}

// Función para simular malloc y gestionar los bloques asignados
void* debugMalloc(size_t size) {
    void* address = malloc(size);
    if (address != NULL) {
        addBlock(&allocatedBlocks, address, size);
    }
    return address;
}

// Función para simular free y gestionar los bloques liberados
void debugFree(void* address) {
    MemBlock* block = allocatedBlocks;
    while (block != NULL) {
        if (block->address == address) {
            addBlock(&freeBlocks, address, block->size);
            removeBlock(&allocatedBlocks, address);
            free(address);
            return;
        }
        block = block->next;
    }
}

// Función para imprimir la información de la memoria
void printMemoryInfo() {
    printBlocks("Bloques asignados:", allocatedBlocks);
    printBlocks("Bloques libres:", freeBlocks);
}

int main() {
    // Simulación de asignación y liberación de memoria
    void* a = debugMalloc(100);
    void* b = debugMalloc(200);
    printMemoryInfo();
    debugFree(a);
    printMemoryInfo();
    void* c = debugMalloc(50);
    printMemoryInfo();
    debugFree(b);
    debugFree(c);
    printMemoryInfo();
    return 0;
}