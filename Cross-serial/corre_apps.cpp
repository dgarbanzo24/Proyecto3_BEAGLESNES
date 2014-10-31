#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <SerialStream.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string.h>

using namespace std;
using namespace LibSerial;

void *Prueba(void *arg)
{
	int b=0;
	
	system("echo 60 > /sys/class/gpio/export");
	system("echo 48 > /sys/class/gpio/export");
	system("echo 51 > /sys/class/gpio/export");
	system("echo 13 > /sys/class/gpio/export");
	system("echo 12 > /sys/class/gpio/export");
	system("echo 49 > /sys/class/gpio/export");
	system("echo 113 > /sys/class/gpio/export");
	system("echo 117 > /sys/class/gpio/export");
	system("echo 112 > /sys/class/gpio/export");
	system("echo 111 > /sys/class/gpio/export");
	system("echo 17 > /sys/class/gpio/export");
	system("echo 110 > /sys/class/gpio/export");
	system("echo 61 > /sys/class/gpio/export");
	
	system("echo low > /sys/class/gpio/gpio60/direction");
	system("echo low > /sys/class/gpio/gpio48/direction");
	system("echo low > /sys/class/gpio/gpio51/direction");
	system("echo low > /sys/class/gpio/gpio13/direction");
	system("echo low > /sys/class/gpio/gpio12/direction");
	system("echo low > /sys/class/gpio/gpio49/direction");
	system("echo low > /sys/class/gpio/gpio113/direction");
	system("echo low > /sys/class/gpio/gpio117/direction");
	system("echo low > /sys/class/gpio/gpio112/direction");
	system("echo low > /sys/class/gpio/gpio111/direction");
	system("echo low > /sys/class/gpio/gpio17/direction");
	system("echo low > /sys/class/gpio/gpio110/direction");
	system("echo low > /sys/class/gpio/gpio61/direction");
//Acondicionamiento del puerto serial para su correcto funcionamiento.
//////////////////////////////////////////////////////////////////////////////
	
	//Abrir el puerto serial.
	SerialStream serial_port ;
	serial_port.Open( "/dev/ttyO1" );
	if ( ! serial_port.good() ) {
	cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
	     << "Error: Could not open serial port." 
	     << endl; exit(1);}

	// Fijar el baud rate del puerto a 9600.
	serial_port.SetBaudRate( SerialStreamBuf::BAUD_9600 );
	if ( ! serial_port.good() ) {
	cerr << "Error: Could not set the baud rate." << endl; exit(1);}

	// Fijar el numero de bits del dato.
	serial_port.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
	if ( ! serial_port.good() ){
	cerr << "Error: Could not set the character size." << endl; exit(1) ;}

	// Desabilitar paridad.
	serial_port.SetParity( SerialStreamBuf::PARITY_NONE ) ;
	if ( ! serial_port.good() ) {
	cerr << "Error: Could not disable the parity." << endl ;
	exit(1) ;}

	// Fijar el numero de bits de parada.
	serial_port.SetNumOfStopBits( 1 ) ;
	if ( ! serial_port.good() ){
	cerr << "Error: Could not set the number of stop bits."<< endl; exit(1) ;}

	// Turn off hardware flow control.
	serial_port.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ;
	if ( ! serial_port.good() ){
	cerr << "Error: Could not use hardware flow control."<< endl; exit(1) ;}

//////////////////////////////////////////////////////////////////////////////
	printf("Puerto abierto\n");
    string datos[16];
    int contador=0;
 
	sleep(5);
   int i=3;
   std::string result=" ";
   while(i>0){ 
	//printf("Ejecutando...\n");
   	//Esperar un momento para que la informacion este lista.
	while( serial_port.rdbuf()->in_avail() == 0 ) {
		//printf("Esperando byte\n");
        usleep(100) ;}
    	//Mantener leyendo el dato del puerto serial.
    	while( serial_port.rdbuf()->in_avail() > 0  ){
		//	printf("Leyendo byte \n");
        char next_byte;
        serial_port.get(next_byte);
        //usleep(100);
        //printf("Byte leido \n");
        //std::cout << "Test Inside While Loop: " << next_byte << "\n";
        //usleep(10000);
        //std::cout << "Entero recibido: " << int(next_byte) << "\n";
        
        int binario[8];
        int residuo = int(next_byte)-40;
        i=0;
        while(i<8){
			binario[i]=residuo%2;
			residuo=residuo/2;
			//std::cout << binario[i] << "\n";
			i++;
		}
		
		if(b==0){
		if(binario[0]==1){system("echo high > /sys/class/gpio/gpio60/direction");}
		else{system("echo low > /sys/class/gpio/gpio60/direction");}
		if(binario[1]==1){system("echo high > /sys/class/gpio/gpio48/direction");}
		else{system("echo low > /sys/class/gpio/gpio48/direction");}
		if(binario[2]==1){system("echo high > /sys/class/gpio/gpio51/direction");}
		else{system("echo low > /sys/class/gpio/gpio51/direction");}
		if(binario[3]==1){system("echo high > /sys/class/gpio/gpio13/direction");}
		else{system("echo low > /sys/class/gpio/gpio13/direction");}
		if(binario[4]==1){system("echo high > /sys/class/gpio/gpio12/direction");}
		else{system("echo low > /sys/class/gpio/gpio12/direction");}
		if(binario[5]==1){system("echo high > /sys/class/gpio/gpio49/direction");}
		else{system("echo low > /sys/class/gpio/gpio49/direction");}
		if(binario[6]==1){system("echo high > /sys/class/gpio/gpio113/direction");}
		else{system("echo low > /sys/class/gpio/gpio113/direction");}
		if(binario[7]==1){system("echo high > /sys/class/gpio/gpio117/direction");}
		else{system("echo low > /sys/class/gpio/gpio117/direction");}
		b=1;
	}
	else{
		if(binario[0]==1){system("echo high > /sys/class/gpio/gpio112/direction");}
		else{system("echo low > /sys/class/gpio/gpio112/direction");}
		if(binario[1]==1){system("echo high > /sys/class/gpio/gpio111/direction");}
		else{system("echo low > /sys/class/gpio/gpio111/direction");}
		if(binario[2]==1){system("echo high > /sys/class/gpio/gpio17/direction");}
		else{system("echo low > /sys/class/gpio/gpio17/direction");}
		if(binario[3]==1){system("echo high > /sys/class/gpio/gpio110/direction");}
		else{system("echo low > /sys/class/gpio/gpio110/direction");}
		if(binario[4]==1){system("echo high > /sys/class/gpio/gpio61/direction");}
		else{system("echo low > /sys/class/gpio/gpio61/direction");}
		b=0;
	}
}
}
}




int main(){
	system("clear");
	FILE * SNES;
	pthread_t id0;
	pthread_create(&id0, NULL , Prueba , NULL);
	SNES = popen("./snes9x-sdl","w");
	pthread_join(id0, NULL);
	close(SNES);
	return 0;
}
