SDKTARGETSYSROOT=/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/toolchain/sysroots/cortexa8hf-vfp-neon-poky-linux-gnueabi
PATH=/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/toolchain/sysroots/x86_64-pokysdk-linux/usr/bin:/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/toolchain/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi
PKG_CONFIG_SYSROOT_DIR=$(SDKTARGETSYSROOT)
PKG_CONFIG_PATH=$(SDKTARGETSYSROOT)/usr/lib/pkgconfig
CONFIG_SITE=/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/toolchain/site-config-cortexa8hf-vfp-neon-poky-linux-gnueabi
OECORE_NATIVE_SYSROOT=/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/toolchain/sysroots/x86_64-pokysdk-linux
OECORE_TARGET_SYSROOT=$(SDKTARGETSYSROOT)
OECORE_ACLOCAL_OPTS=-I/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/toolchain/sysroots/x86_64-pokysdk-linux/usr/share/aclocal
PYTHONHOME=/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/toolchain/sysroots/x86_64-pokysdk-linux/usr
CC=arm-poky-linux-gnueabi-gcc  -march=armv7-a -mthumb-interwork -mfloat-abi=hard -mfpu=neon -mtune=cortex-a8 --sysroot=$(SDKTARGETSYSROOT)
CXX=arm-poky-linux-gnueabi-g++  -march=armv7-a -mthumb-interwork -mfloat-abi=hard -mfpu=neon -mtune=cortex-a8 --sysroot=$(SDKTARGETSYSROOT)
CPP=arm-poky-linux-gnueabi-gcc -E  -march=armv7-a -mthumb-interwork -mfloat-abi=hard -mfpu=neon -mtune=cortex-a8 --sysroot=$(SDKTARGETSYSROOT)
AS=arm-poky-linux-gnueabi-as 
LD=arm-poky-linux-gnueabi-ld  --sysroot=$(SDKTARGETSYSROOT)
GDB=arm-poky-linux-gnueabi-gdb
STRIP=arm-poky-linux-gnueabi-strip
RANLIB=arm-poky-linux-gnueabi-ranlib
OBJCOPY=arm-poky-linux-gnueabi-objcopy
OBJDUMP=arm-poky-linux-gnueabi-objdump
AR=arm-poky-linux-gnueabi-ar
NM=arm-poky-linux-gnueabi-nm
M4=m4
TARGET_PREFIX=arm-poky-linux-gnueabi-
CONFIGURE_FLAGS=--target=arm-poky-linux-gnueabi --host=arm-poky-linux-gnueabi --build=x86_64-linux --with-libtool-sysroot=$(SDKTARGETSYSROOT)
CFLAGS= -O2 -pipe -g -feliminate-unused-debug-types
CXXFLAGS= -O2 -pipe -g -feliminate-unused-debug-types
LDFLAGS=-Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed
CPPFLAGS=
OECORE_DISTRO_VERSION=1.6
OECORE_SDK_VERSION=1.6
ARCH=arm
CROSS_COMPILE=arm-poky-linux-gnueabi-

INCLUDES=-I/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/beaglebone-daisy-11.0.0/Beaglebone/tmp/sysroots/beaglebone/usr/include -I/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/Proyecto3_BEAGLESNES/libserial -I/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/beaglebone-daisy-11.0.0/Beaglebone/tmp/sysroots/beaglebone/usr/include

LIBS=-L/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/Proyecto3_BEAGLESNES/libserial -L/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/beaglebone-daisy-11.0.0/Beaglebone/tmp/sysroots/beaglebone/usr/lib -L/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/beaglebone-daisy-11.0.0/Beaglebone/tmp/sysroots/beaglebone/usr/lib

cross-serial:
	$(CXX)  $(INCLUDES) config_GPIOs.cpp -o writeGPIOsapp -fpermissive $(LIBS)
	$(CXX) $(INCLUDES) -o leer_puertoapp $(CFLAGS) $(LDFLAGS) -fpermissive -fPIC leer_puerto.cpp  $(LIBS) -lserial
	$(CXX) $(INCLUDES) -o correapps $(CFLAGS) $(LDFLAGS) -fpermissive -fPIC corre_apps.cpp  $(LIBS)   -lpthread -lm -lserial
	
