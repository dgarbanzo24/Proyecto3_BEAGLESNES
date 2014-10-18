#S9XDEBUGGER=1
#S9XNETPLAY=1
S9XZIP=1
S9XJMA=1
#NEONBLIT=1

###############################Compilación cruzada######################################

SDKTARGETSYSROOT=/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/toolchain/sysroots/cortexa8hf-vfp-neon-poky-linux-gnueabi
PATH=/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/toolchain/sysroots/x86_64-pokysdk-linux/usr/bin :/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/toolchain/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi
PKG_CONFIG_SYSROOT_DIR=$(SDKTARGETSYSROOT)
PKG_CONFIG_PATH=$(SDKTARGETSYSROOT)/usr/lib/pkgconfig
CONFIG_SITE=/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/toolchain/site-config-cortexa8hf-vfp-neon-poky-linux-gnueabi
OECORE_NATIVE_SYSROOT=/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/toolchain/sysroots/x86_64-pokysdk-linux
OECORE_TARGET_SYSROOT=$(SDKTARGETSYSROOT)
OECORE_ACLOCAL_OPTS=-I /home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/toolchain/sysroots/x86_64-pokysdk-linux/usr/share/aclocal
PYTHONHOME=/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/toolchain/sysroots/x86_64-pokysdk-linux/usr
CCBONE=arm-poky-linux-gnueabi-gcc  -march=armv7-a -mthumb-interwork -mfloat-abi=hard -mfpu=neon -mtune=cortex-a8 --sysroot=$(SDKTARGETSYSROOT)
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

########################################################################################


# Fairly good and special-char-safe descriptor of the os being built on.
OS         = `uname -s -r -m|sed \"s/ /-/g\"|tr \"[A-Z]\" \"[a-z]\"|tr \"/()\" \"___\"`
BUILDDIR   = .

OBJECTS    = ../apu/apu.o ../apu/SNES_SPC.o ../apu/SNES_SPC_misc.o ../apu/SNES_SPC_state.o ../apu/SPC_DSP.o ../apu/SPC_Filter.o ../bsx.o ../c4.o ../c4emu.o ../cheats.o ../cheats2.o ../clip.o ../conffile.o ../controls.o ../cpu.o ../cpuexec.o ../cpuops.o ../crosshairs.o ../dma.o ../dsp.o ../dsp1.o ../dsp2.o ../dsp3.o ../dsp4.o ../fxinst.o ../fxemu.o ../gfx.o ../globals.o ../logger.o ../memmap.o ../movie.o ../obc1.o ../ppu.o ../reader.o ../sa1.o ../sa1cpu.o ../screenshot.o ../sdd1.o ../sdd1emu.o ../seta.o ../seta010.o ../seta011.o ../seta018.o ../snapshot.o ../snes9x.o ../spc7110.o ../srtc.o ../tile.o ../filter/2xsai.o ../filter/blit.o ../filter/epx.o ../filter/hq2x.o ../filter/snes_ntsc.o sdlmain.o sdlinput.o sdlvideo.o sdlaudio.o gui.o guiGameList.o guiGameInfo.o guiAudio.o guiGamepad.o guiParser.o guiSplash.o guiPause.o savepng.o gpio.o

ifdef NEONBLIT
OBJECTS   += arm/blitARM.o arm/neon_scale2x.o
endif

ifdef S9XDEBUGGER
OBJECTS   += ../debug.o ../fxdbg.o
endif

ifdef S9XNETPLAY
OBJECTS   += ../netplay.o ../server.o
endif

ifdef S9XZIP
OBJECTS   += ../loadzip.o ../unzip/ioapi.o ../unzip/unzip.o
endif

ifdef S9XJMA
OBJECTS   += ../jma/7zlzma.o ../jma/crc32.o ../jma/iiostrm.o ../jma/inbyte.o ../jma/jma.o ../jma/lzma.o ../jma/lzmadec.o ../jma/s9x-jma.o ../jma/winout.o
endif

CCC        = /home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/toolchain/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/$(CXX)
CC         = /home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/toolchain/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/$(CCBONE)
GASM       = $(CCC)
INCLUDES   = -I. -I.. -I../apu/ -I../unzip/ -I../jma/ -I../filter/ -I/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/beaglebone-daisy-11.0.0/Beaglebone/tmp/sysroots/beaglebone/usr/include/SDL -I/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/beaglebone-daisy-11.0.0/Beaglebone/tmp/sysroots/beaglebone/usr/include -I/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/beaglebone-daisy-11.0.0/Beaglebone/tmp/sysroots/beaglebone/usr/include/c++ -I//home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/beaglebone-daisy-11.0.0/Beaglebone/tmp/sysroots/beaglebone/usr/include/c++/bits -I/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/beaglebone-daisy-11.0.0/Beaglebone/tmp/sysroots/beaglebone/usr/include/c++/arm-poky-linux-gnueabi/bits -I/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/beaglebone-daisy-11.0.0/Beaglebone/tmp/sysroots/beaglebone/usr/include

INCLS= -I/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/beaglebone-daisy-11.0.0/Beaglebone/tmp/sysroots/beaglebone/usr/include/SDL

#CCFLAGS    = -Ofast -fomit-frame-pointer -fno-exceptions -fno-rtti -pedantic -Wall -W -Wno-unused-parameter -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT -DZLIB -DUNZIP_SUPPORT -DJMA_SUPPORT -DHAVE_LIBPNG -DHAVE_MKSTEMP -DHAVE_STRINGS_H -DHAVE_SYS_IOCTL_H -DHAVE_STDINT_H -DRIGHTSHIFT_IS_SAR $(DEFS)
#CFLAGS     = $(CCFLAGS)

CCFLAGS    = $(CXXFLAGS)
CFLAGS     = $(CXXFLAGS)

.SUFFIXES: .o .cpp .c .cc .h .m .i .s .obj

all: Makefile configure snes9x-sdl

Makefile: configure Makefile.in
	@echo "Makefile is older than configure or in-file. Run configure or touch Makefile."
	exit 1

configure: configure.ac
	@echo "configure is older than in-file. Run autoconf or touch configure."
	exit 1

snes9x-sdl: $(OBJECTS)
	$(CCC) $(INCLUDES) -o $@ $(CCFLAGS) $(OBJECTS) $(INCLS) -Wl,-rpath-link=/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/beaglebone-daisy-11.0.0/Beaglebone/tmp/sysroots/beaglebone/usr/lib -L/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/toolchain/sysroots/cortexa8hf-vfp-neon-poky-linux-gnueabi/usr/lib -lpthread -L/home/alvaro/Documentos/TEC/Embebidos/Proyecto_III/beaglebone-daisy-11.0.0/Beaglebone/tmp/sysroots/beaglebone/usr/lib  -lpng  -lm -lz  -lSDL -lSDL_ttf -lSDL_image -lSDL_mixer -lexpat -lpng 
#
#-  
#-lm -lz  -lSDL -lSDL_ttf -lSDL_image -lSDL_mixer -lexpat -lpng -lpthread libpthread.so.0

../jma/s9x-jma.o: ../jma/s9x-jma.cpp
	$(CCC) $(INCLUDES) -c $(CCFLAGS) -fexceptions $*.cpp -o $@
../jma/7zlzma.o: ../jma/7zlzma.cpp
	$(CCC) $(INCLUDES) -c $(CCFLAGS) -fexceptions $*.cpp -o $@
../jma/crc32.o: ../jma/crc32.cpp
	$(CCC) $(INCLUDES) -c $(CCFLAGS) -fexceptions $*.cpp -o $@
../jma/iiostrm.o: ../jma/iiostrm.cpp
	$(CCC) $(INCLUDES) -c $(CCFLAGS) -fexceptions $*.cpp -o $@
../jma/inbyte.o: ../jma/inbyte.cpp
	$(CCC) $(INCLUDES) -c $(CCFLAGS) -fexceptions $*.cpp -o $@
../jma/jma.o: ../jma/jma.cpp
	$(CCC) $(INCLUDES) -c $(CCFLAGS) -fexceptions $*.cpp -o $@
../jma/lzma.o: ../jma/lzma.cpp
	$(CCC) $(INCLUDES) -c $(CCFLAGS) -fexceptions $*.cpp -o $@
../jma/lzmadec.o: ../jma/lzmadec.cpp
	$(CCC) $(INCLUDES) -c $(CCFLAGS) -fexceptions $*.cpp -o $@
../jma/winout.o: ../jma/winout.cpp
	$(CCC) $(INCLUDES) -c $(CCFLAGS) -fexceptions $*.cpp -o $@

.cpp.o:
	$(CCC) $(INCLUDES) -c $(CCFLAGS) $*.cpp -o $@

.c.o:
	$(CC) $(INCLUDES) -c $(CCFLAGS) $*.c -o $@

.cpp.S:
	$(GASM) $(INCLUDES) -S $(CCFLAGS) $*.cpp -o $@

.cpp.i:
	$(GASM) $(INCLUDES) -E $(CCFLAGS) $*.cpp -o $@

.S.o:
	$(GASM) $(INCLUDES) -c $(CCFLAGS) $*.S -o $@

.S.i:
	$(GASM) $(INCLUDES) -c -E $(CCFLAGS) $*.S -o $@

.s.o:
	@echo Compiling $*.s
	sh-elf-as -little $*.s -o $@

.obj.o:
	cp $*.obj $*.o

clean:
	rm -f $(OBJECTS)
