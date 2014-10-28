#include <SerialStream.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string.h>

using namespace std;
using namespace LibSerial;

//Encabezados de las funciones
const char* hex_char_to_bin(char c);
string hexToBin(const string& hex);

int main( int argc, char** argv){

//Acondicionamiento del puerto serial para su correcto funcionamiento.

//Abrir el puerto serial.
SerialStream serial_port ;
serial_port.Open( "/dev/ttyO1" );
if ( ! serial_port.good() ) {
cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
     << "Error: Could not open serial port." << endl; exit(1);}

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
cerr << "Error: Could not disable the parity." << endl; exit(1) ;}

// Fijar el numero de bits de parada.
serial_port.SetNumOfStopBits( 1 ) ;
if ( ! serial_port.good() ){
cerr << "Error: Could not set the number of stop bits."<< endl; exit(1) ;}

// Turn off hardware flow control.
serial_port.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ;
if ( ! serial_port.good() ){
cerr << "Error: Could not use hardware flow control."<< endl; exit(1) ;}

//////////////////////////////////////////////////////////////////////////////

string datos[16];
int contador=0;
int i=1;
string result=" ";
system("echo 30 > /sys/class/gpio/export");
system("echo 31 > /sys/class/gpio/export");
system("echo 48 > /sys/class/gpio/export");
system("echo 51 > /sys/class/gpio/export");
system("echo 04 > /sys/class/gpio/export");	
system("echo 03 > /sys/class/gpio/export");	
system("echo 49 > /sys/class/gpio/export");	
system("echo 117 > /sys/class/gpio/export");	
system("echo 125 > /sys/class/gpio/export");	
system("echo 60 > /sys/class/gpio/export");
system("echo 40 > /sys/class/gpio/export");	
system("echo 05 > /sys/class/gpio/export");	
system("echo 02 > /sys/class/gpio/export");

while(i>0){ 
	//Esperar un momento para que la informacion este lista.
	while( serial_port.rdbuf()->in_avail() == 0 ) {
        usleep(100) ;}
    	//Mantener leyendo el dato del puerto serial.
    	while( serial_port.rdbuf()->in_avail() > 0  ){
        char next_byte;
        serial_port.get(next_byte);
        usleep(100);
 	cout<<'\n'<<next_byte<<' ';
      
	//Convertir el valor en hexa en binario
	result=next_byte;	
	string bin = hexToBin(result);	        
	cout<<": "<<bin<<'\n';

	//Recorrer string bin para guardar cada bit por separado.
	int e=0;
	for (e=0; e<bin.size();e++){
		if (e<1){datos[e+contador*4] = bin[e];}
		if (e<2 && e>0){datos[e+contador*4] = bin[e];}
		if (e<3 && e>1){datos[e+contador*4] = bin[e];}
		if (e<4 && e>2){datos[e+contador*4] = bin[e];}} 
		contador++;
		if (contador==4){
		contador=0;

	//Configurar los 13 GPIOs.

	for(int c=0;c<16;c++){

	//Configurar GPIO_30
	if(c==0){
	
	if(datos[c]=="0"){
	system("echo low > /sys/class/gpio/gpio30/direction");}
	else{
	system("echo high > /sys/class/gpio/gpio30/direction");}}

	//Configurar GPIO_31
	if(c==1){
	
	if(datos[c]=="0"){
	system("echo low > /sys/class/gpio/gpio31/direction");}
	else{
	system("echo high > /sys/class/gpio/gpio31/direction");}}

	//Configurar GPIO_48
	if(c==2){
	
	if(datos[c]=="0"){
	system("echo low > /sys/class/gpio/gpio48/direction");}
	else{
	system("echo high > /sys/class/gpio/gpio48/direction");}}

	//Configurar GPIO_04
	if(c==3){
	
	if(datos[c]=="0"){
	system("echo low > /sys/class/gpio/gpio04/direction");}
	else{
	system("echo high > /sys/class/gpio/gpio04/direction");}}

	//Configurar GPIO_03
	if(c==4){

	if(datos[c]=="0"){
	system("echo low > /sys/class/gpio/gpio03/direction");}
	else{
	system("echo high > /sys/class/gpio/gpio03/direction");}}

	//Configurar GPIO_49
	if(c==5){

	if(datos[c]=="0"){
	system("echo low > /sys/class/gpio/gpio49/direction");}
	else{
	system("echo high > /sys/class/gpio/gpio49/direction");}}

	//Configurar GPIO_117
	if(c==6){

	if(datos[c]=="0"){
	system("echo low > /sys/class/gpio/gpio117/direction");}
	else{
	system("echo high > /sys/class/gpio/gpio117/direction");}}

	//Configurar GPIO_125
	if(c==7){

	if(datos[c]=="0"){
	system("echo low > /sys/class/gpio/gpio125/direction");}
	else{
	system("echo high > /sys/class/gpio/gpio125/direction");}}

	//Configurar GPIO_60
	if(c==8){

	if(datos[c]=="0"){
	system("echo low > /sys/class/gpio/gpio60/direction");}
	else{
	system("echo high > /sys/class/gpio/gpio60/direction");}}

	//Configurar GPIO_40
	if(c==9){

	if(datos[c]=="0"){
	system("echo low > /sys/class/gpio/gpio40/direction");}
	else{
	system("echo high > /sys/class/gpio/gpio40/direction");}}

	//Configurar GPIO_51
	if(c==10){
	
	if(datos[c]=="0"){
	system("echo low > /sys/class/gpio/gpio51/direction");}
	else{
	system("echo high > /sys/class/gpio/gpio51/direction");}}

	//Configurar GPIO_05
	if(c==11){
	
	if(datos[c]=="0"){
	system("echo low > /sys/class/gpio/gpio05/direction");}
	else{
	system("echo high > /sys/class/gpio/gpio05/direction");}}

	//Configurar GPIO_02
	if(c==12){

	if(datos[c]=="0"){
	system("echo low > /sys/class/gpio/gpio02/direction");}
	else{system("echo high > /sys/class/gpio/gpio02/direction");}}
	
	}
	}
	}
	}   
	return EXIT_SUCCESS;
	}

// Funcion que recibe un char (parte de un número hexa) y devuelve su equivalente en bin (string).
const char* hex_char_to_bin(char c){
    switch(toupper(c)){
        case '0': return "0000";
        case '1': return "0001";case '2': return "0010";case '3': return "0011";
        case '4': return "0100";case '5': return "0101";case '6': return "0110";
        case '7': return "0111";case '8': return "1000";case '9': return "1001";
        case 'A': return "1010";case 'B': return "1011";case 'C': return "1100";
        case 'D': return "1101";case 'E': return "1110";case 'F': return "1111";}}

//Funcion que covierte un numero hexa (string) y devuelve su equivalente en bin (string).
string hexToBin(const string& hexa){
    string bin; // variable que guarda la representación del string hexadecimal convertido en binario
    for(int i = 0; i != hexa.length(); ++i)
       bin += hex_char_to_bin(hexa[i]);
    return bin;}



