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

	system("echo 44 > /sys/class/gpio/export");    //Aqui se configura el GPIO_44.
   int i=1;
   while (i>0){
   //system("ls /sys/class/gpio/");
   
   //system("ls /sys/class/gpio/");
   //system("ls /sys/class/gpio/gpio44/");
   system("echo high > /sys/class/gpio/gpio44/direction");  
   system("sleep 1");
   sleep(1);
   system("echo low > /sys/class/gpio/gpio44/direction");
   sleep(1);
}
   return 0;
}
