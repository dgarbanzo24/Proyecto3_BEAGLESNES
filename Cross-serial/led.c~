#include <stdlib.h>
#include <stdio.h>

#define VALOR "/sys/class/gpio/gpio38/value"
#define DIRECCION "/sys/class/gpio/gpio38/direction"

int main(){
	//Definimos que el GPIO sea de salida.
	FILE *fd_dir = fopen(DIRECCION,"w");
	fwrite("out",3,1,fd_dir);
	fclose(fd_dir);

	FILE *fd_val = fopen(VALOR,"r+");

	//Ciclo para el valor (alto-bajo-alto-bajo...)
	while(1){
		fwrite("1",1,1,fd_val);
		usleep(100000); //Retardo en micro segundos
		rewind(fd_val);
		fwrite("0",1,1,fd_val);
		usleep(100000);
		rewind(fd_val);
	}

	fclose(fd_val);
	return 0;
}
