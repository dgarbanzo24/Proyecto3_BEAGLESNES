#include <SerialStream.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <cmath>

using namespace std;
using namespace LibSerial;

//Encabezados de las funciones
const char* hex_char_to_bin(char c);
string hexToBin(string hex);

// Funcion que recibe un char (parte de un nÃºmero hexa) y devuelve su equivalente en bin (string).
const char* hex_char_to_bin(char c){
    switch(toupper(c)){
        case '0': return "0000";
        case '1': return "0001";case '2': return "0010";case '3': return "0011";
        case '4': return "0100";case '5': return "0101";case '6': return "0110";
        case '7': return "0111";case '8': return "1000";case '9': return "1001";
        case 'A': return "1010";case 'B': return "1011";case 'C': return "1100";
        case 'D': return "1101";case 'E': return "1110";case 'F': return "1111";}}

//Funcion que covierte un numero hexa (string) y devuelve su equivalente en bin (string).

std::string hexToBin(string hexa){
	printf("Inicia conversion...\n");
    std::string bina; // variable que guarda la representaciÃ³n del string hexadecimal convertido en binario
    for(int i = 0; i != hexa.length(); ++i)
       bina += hex_char_to_bin(hexa[i]);
    return bina;}





void A_to_B(const char* input) 
{
	int ascii;           // used to store the ASCII number of a character
	int length = strlen(input);        //find the length of the user's input
	
	std::cout << " ";
	
	for(int x = 0; x < length; x++)          //repeat until user's input have all been read
	// x < length because the last character is "\0"
	{
		ascii = input[x];        //store a character in its ASCII number
		
		/* Refer to http://www.wikihow.com/Convert-from-Decimal-to-Binary for conversion method used 
		 * in this program*/		
		
		char* binary_reverse = new char [9];       //dynamic memory allocation
		char* binary = new char [9];
		
		int y = 0;    //counter used for arrays
		
		while(ascii != 1)    //continue until ascii == 1
		{
			if(ascii % 2 == 0)    //if ascii is divisible by 2
			{
				binary_reverse[y] = '0';   //then put a zero
			}
			else if(ascii % 2 == 1)    //if it isnt divisible by 2
			{
				binary_reverse[y] = '1';   //then put a 1
			}
			ascii /= 2;    //find the quotient of ascii / 2
			y++;    //add 1 to y for next loop
		}
		
		if(ascii == 1)    //when ascii is 1, we have to add 1 to the beginning
		{
			binary_reverse[y] = '1';
			y++;
		}
		
		if(y < 8)  //add zeros to the end of string if not 8 characters (1 byte)
		{
			for(; y < 8; y++)  //add until binary_reverse[7] (8th element)
			{
				binary_reverse[y] = '0';
			}
		}
	
		for(int z = 0; z < 8; z++)  //our array is reversed. put the numbers in the rigth order (last comes first)
		{
			binary[z] = binary_reverse[7 - z];
		}
		
		std::cout << binary;   //display the 8 digit binary number
		
		delete [] binary_reverse;     //free the memory created by dynamic mem. allocation
		delete [] binary;
	}
	
	std::cout << endl;
}

int main( int argc, char** argv){

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
 
   int i=3;
   std::string result=" ";
   while(i>0){ 
	printf("Ejecutando...\n");
   	//Esperar un momento para que la informacion este lista.
	while( serial_port.rdbuf()->in_avail() == 0 ) {
		printf("Esperando byte\n");
        usleep(100) ;}
    	//Mantener leyendo el dato del puerto serial.
    	while( serial_port.rdbuf()->in_avail() > 0  ){
			printf("Leyendo byte \n");
        char next_byte;
        serial_port.get(next_byte);
        usleep(100);
        printf("Byte leido \n");
        std::cout << "Test Inside While Loop " << next_byte << "\n";
        usleep(10000);
        
        
        
		//printf("Next byte %c",next_byte);
 	//cout<<'\n'<<next_byte<<' ';	
	//cerr << hex << static_cast<char>(next_byte) << " ";
      
	//Convertir el valor en hexa en binario
	result+=next_byte;	
	printf("continua...\n");
	A_to_B(next_byte);
	//std::string bin = hexToBin(result);	  
	printf("Conversion hecha...\n");      
	//std::cout<<": "<<bin<<'\n';
}
}
}

	/*//Recorrer string bin para guardar cada bit por separado.
	int e=0;
	for (e=0; e<bin.size();e++){
		if (e<1){
			datos[e+contador*4] = bin[e];}
		if (e<2 && e>0){
			datos[e+contador*4] = bin[e];}
		if (e<3 && e>1){
			datos[e+contador*4] = bin[e];}
		if (e<4 && e>2){
			datos[e+contador*4] = bin[e];}
	} 

	contador++;
	if (contador==4){
	contador=0;

//Aqui deberia implementarse la progra para los gpios de la bbb.
//////////////////////////////////////////////////////////////////////////////////////
	cout<<'\n'<<"1er byte: "<<'\n';
	cout<<datos[0]<<'\n';
	cout<<datos[1]<<'\n';
	cout<<datos[2]<<'\n';
	cout<<datos[3]<<'\n';
	cout<<datos[4]<<'\n';
	cout<<datos[5]<<'\n';
	cout<<datos[6]<<'\n';
	cout<<datos[7]<<'\n';
	cout<<'\n'<<"2do byte: "<<'\n';
	cout<<datos[8]<<'\n';
	cout<<datos[9]<<'\n';
	cout<<datos[10]<<'\n';
	cout<<datos[11]<<'\n';
	cout<<datos[12]<<'\n';
	cout<<datos[13]<<'\n';
	cout<<datos[14]<<'\n';
	cout<<datos[15]<<'\n'<<'\n';}	
	
	}
	i=i-1;
	}   
	return EXIT_SUCCESS;}*/








