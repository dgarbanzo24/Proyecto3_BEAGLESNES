#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


using namespace std;

int main(){

	system("echo 39 > /sys/class/gpio/export");
	system("echo 35 > /sys/class/gpio/export");
	system("echo 67 > /sys/class/gpio/export");
	system("echo 68 > /sys/class/gpio/export");
	system("echo 44 > /sys/class/gpio/export");
	system("echo 26 > /sys/class/gpio/export");
	system("echo 46 > /sys/class/gpio/export");
	system("echo 65 > /sys/class/gpio/export");
	system("echo 63 > /sys/class/gpio/export");
	system("echo 37 > /sys/class/gpio/export");
	system("echo 33 > /sys/class/gpio/export");
	system("echo 61 > /sys/class/gpio/export");
	system("echo 88 > /sys/class/gpio/export");    //Aqui se configura el GPIO_44.
   int i=1;
   while (i>0){
	  
	   system("echo high > /sys/class/gpio/gpio39/direction");
		
		system("echo high > /sys/class/gpio/gpio35/direction");
		
		system("echo high > /sys/class/gpio/gpio67/direction");
		
		system("echo high > /sys/class/gpio/gpio68/direction");
		
		system("echo high > /sys/class/gpio/gpio44/direction");
		
		system("echo high > /sys/class/gpio/gpio26/direction");
		
		system("echo high > /sys/class/gpio/gpio46/direction");
		
		system("echo high > /sys/class/gpio/gpio65/direction");
		system("echo high > /sys/class/gpio/gpio63/direction");
		system("echo high > /sys/class/gpio/gpio37/direction");
		system("echo high > /sys/class/gpio/gpio33/direction");
		system("echo high > /sys/class/gpio/gpio61/direction");
		system("echo high > /sys/class/gpio/gpio88/direction");
   //system("ls /sys/class/gpio/");
   
   //system("ls /sys/class/gpio/");
   //system("ls /sys/class/gpio/gpio44/");
   sleep(1);
   system("echo low > /sys/class/gpio/gpio39/direction");
   system("echo low > /sys/class/gpio/gpio35/direction");
   system("echo low > /sys/class/gpio/gpio67/direction");
   system("echo low > /sys/class/gpio/gpio68/direction");
   system("echo low > /sys/class/gpio/gpio44/direction");
   system("echo low > /sys/class/gpio/gpio26/direction");
   system("echo low > /sys/class/gpio/gpio46/direction");
   system("echo low > /sys/class/gpio/gpio65/direction");
		system("echo low > /sys/class/gpio/gpio63/direction");
		system("echo low > /sys/class/gpio/gpio37/direction");
		system("echo low > /sys/class/gpio/gpio33/direction");
		system("echo low > /sys/class/gpio/gpio61/direction");
		system("echo low > /sys/class/gpio/gpio88/direction");
   sleep(1);
}
   return 0;
}
