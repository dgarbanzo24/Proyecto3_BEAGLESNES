#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

/* la tasa de baudios esta definida en  <asm/termbits.h>, que esta 
   incluida <termios.h> */

#define BAUDRATE B9600

/* cambie esta definicion por el puerto correcto  */
#define MODEMDEVICE "/dev/ttyO1"

#define _POSIX_SOURCE 1 /* fuentes cumple POSIX  */

#define FALSE 0
#define TRUE 1

volatile int STOP=FALSE;

main()
{
   int fd,c, res;
   struct termios oldtio,newtio;
   char buf[255];

/* 
      Abre el dispositivo modem para lectura y escritura y no como controlador
      tty porque no queremos que nos mate si el ruido de la linea envia 
      un CTRL-C.
*/

   fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
   if (fd <0) {  perror(MODEMDEVICE); exit(-1);  }

   tcgetattr(fd,&oldtio); /* almacenamos la configuracion actual del puerto */

   bzero(&newtio, sizeof(newtio)); /* limpiamos struct para recibir los
                                        nuevos parametros del puerto */

/*
      BAUDRATE: Fija la tasa bps. Podria tambien usar cfsetispeed y cfsetospeed.
      CRTSCTS : control de flujo de salida por hardware (usado solo si el cable 
      tiene todas las lineas necesarias Vea sect. 7 de Serial-HOWTO)
      CS8     : 8n1 (8bit,no paridad,1 bit de parada)
      CLOCAL  : conexion local, sin control de modem
      CREAD   : activa recepcion de caracteres
*/

   newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;

/*
      IGNPAR  : ignora los bytes con error de paridad
      ICRNL   : mapea CR a NL (en otro caso una entrada CR del otro ordenador 
      no terminaria la entrada) en otro caso hace un dispositivo en bruto 
      (sin otro proceso de entrada)
*/

   newtio.c_iflag = IGNPAR | ICRNL;

/*
      Salida en bruto.
*/
   newtio.c_oflag = 0;

/*
      ICANON  : activa entrada canonica
      desactiva todas las funcionalidades del eco, y no envia segnales al
      programa
      llamador
*/

   newtio.c_lflag = ICANON;

/* 
      inicializa todos los caracteres de control
      los valores por defecto se pueden encontrar en /usr/include/termios.h, 
      y vienen dadas en los comentarios, pero no los necesitamos aqui
*/

   newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */
   newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
   newtio.c_cc[VERASE]   = 0;     /* del */
   newtio.c_cc[VKILL]    = 0;     /* @ */
   newtio.c_cc[VEOF]     = 4;     /* Ctrl-d */
   newtio.c_cc[VTIME]    = 0;     /* temporizador entre caracter, no usado */
   newtio.c_cc[VMIN]     = 1;     /* bloqu.lectura hasta llegada de caracter. 1 */
   newtio.c_cc[VSWTC]    = 0;     /* '\0' */
   newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */
   newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
   newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
   newtio.c_cc[VEOL]     = 0;     /* '\0' */
   newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
   newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
   newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
   newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
   newtio.c_cc[VEOL2]    = 0;     /* '\0' */

/* 
      ahora limpiamos la linea del modem y activamos la configuracion del
      puerto
*/

   tcflush(fd, TCIFLUSH);
   tcsetattr(fd,TCSANOW,&newtio);

/*
      configuracion del terminal realizada, ahora manejamos las entradas.
      En este ejemplo, al introducir una  'z' al inicio de linea terminara el 
      programa.  
*/

   while (STOP==FALSE) {     /* bucle hasta condicion de terminar */

/* 
   bloque de ejecucion de programa hasta que llega un caracter de fin de
   linea, incluso si llegan mas de 255 caracteres.
   Si el numero de caracteres leidos es menor que el numero de caracteres 
   disponibles, las siguientes lecturas devolveran los caracteres restantes.
   'res' tomara el valor del numero actual de caracteres leidos.
*/

                          res = read(fd,buf,255);
                          buf[res]=0;             /* envio de fin de cadena, a fin de poder usar printf */
                          printf(":%s:%d\n", buf, res);
                          if (buf[0]=='z') STOP=TRUE;
                       }

/* restaura la anterior configuracion del puerto  */

   tcsetattr(fd,TCSANOW,&oldtio);
}
