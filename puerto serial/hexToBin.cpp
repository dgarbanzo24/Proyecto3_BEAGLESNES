// constructing bitsets
#include <iostream>       // std::cout
#include <string>         // std::string

using namespace std;

//Encabezados de las funciones
const char* hex_char_to_bin(char c);
string hexToBin(const string& hex);

int main ()
{
  
  string bin = hexToBin("42");
  cout<<"Binario: "<<bin<<'\n';
  return 0;
}

// Función que recibe un char ("A","4") representando una parte de un número hexadecimal y devuelve su equivalente en binario representado mediante un string
const char* hex_char_to_bin(char c)
{
    // TODO handle default / error
    switch(toupper(c))
    {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
    }
}

// Función que covierte un hexadecimal representado por un string ("4f","41") y devuelve su equivalente en binario representado por un string
string hexToBin(const string& hex)
{
    string bin; // variable que guarda la representación del string hexadecimal convertido en binario
    for(int i = 0; i != hex.length(); ++i)
       bin += hex_char_to_bin(hex[i]);
    return bin;
}
