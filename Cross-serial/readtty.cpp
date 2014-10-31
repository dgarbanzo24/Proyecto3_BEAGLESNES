#include <SerialStream.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>

using namespace LibSerial;

std::string s_read(SerialStream& serial_port)
{
    std::cout << "Test Before While Loop\n";
    std::string result;
    std::cout << "Serial available:" << serial_port.rdbuf()->in_avail() << "\n\n";
    while( serial_port.rdbuf()->in_avail() )
    {
        char next_byte;
        serial_port.get(next_byte);  
        std::cout << "Test Inside While Loop" << next_byte << "\n";
        result += next_byte;
    }
    return result;
}

void s_write(SerialStream& serial_port, const std::string& data)
{
   serial_port.write(data.c_str(), data.size());
}


int
main( int    argc,
      char** argv  )
{
    //
    // Open the serial port.
    //
    using namespace std;

    SerialStream serial_port ;
    char c;
    serial_port.Open( "/dev/ttyO1" ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
                  << "Error: Could not open serial port."
                  << std::endl ;
        exit(1) ;
    }
    //
    // Set the baud rate of the serial port.
    //
    serial_port.SetBaudRate( SerialStreamBuf::BAUD_9600 ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the baud rate." <<  
std::endl ;
        exit(1) ;
    }
    //
    // Set the number of data bits.
    //
    serial_port.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the character size." <<  
std::endl ;
        exit(1) ;
    }
    //
    // Disable parity.
    //
    serial_port.SetParity( SerialStreamBuf::PARITY_NONE ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not disable the parity." <<  
std::endl ;
        exit(1) ;
    }
    //
    // Set the number of stop bits.
    //
    serial_port.SetNumOfStopBits( 1 ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the number of stop bits."
                  << std::endl ;
        exit(1) ;
    }
    //
    // Turn off hardware flow control.
    //
    serial_port.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not use hardware flow control."
                  << std::endl ;
        exit(1) ;
    }
    //
    // Do not skip whitespace characters while reading from the
    // serial port.
    //
    // serial_port.unsetf( std::ios_base::skipws ) ;
    //
    // Wait for some data to be available at the serial port.
    //
    //
    // Keep reading data from serial port and print it to the screen.
    //
std::cout << "Test before first write()\n";

s_write(serial_port, "check"); // First Command

while( serial_port.rdbuf()->in_avail() == 0 ) {
		printf("Esperando byte\n");
        usleep(100) ;}
std::string first_reply = s_read(serial_port);

std::cout << first_reply;

std::cout << "Test after the first read()\n";

s_write(serial_port, "check");

std::string second_reply = s_read(serial_port);
std::cerr << second_reply;

    return EXIT_SUCCESS ;
}
