#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

char getChar(int i) { // toma los caracteres a partir de la letra A en ascii
    return (char)(i + 65); 
}
void process(char id, int duracion) {
	printf(" %c está durmiendo durante %d segundos...\n", id, duracion);
	sleep(duracion);
	printf(" %c ya no está durmiendo (han pasado %d segundos)... \n", id, duracion);
	wait(NULL);
}

int main() {
/*la variable numProcesos permite que se creen 
la cantidad de procesos deseados y no solo A,B,C
*/
	int numProcesos = 3; 
	for (int i = 0; i < numProcesos; i++) {
		int pid = fork();
		if (pid != 0) {
			process(getChar(i), ((i+1)*5));
			if (i+1 < numProcesos) {
				printf("%c: Ya no estoy esperando a mi hijo %c \n", getChar(i), getChar(i+1));
			}
			break;
		}
	}
}
