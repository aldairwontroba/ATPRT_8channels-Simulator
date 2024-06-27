top_srcdir = .
srcdir = .

top_builddir = .
PREFIX = /usr/local
prefix = $(PREFIX)
exec_prefix = ${prefix}
bindir = ${exec_prefix}/bin
libdir = ${exec_prefix}/lib
includedir = ${prefix}/include
CC = gcc
CXX = g++
CFLAGS = -g -O2 -DPA_LITTLE_ENDIAN -I$(top_srcdir)/include -I$(top_srcdir)/src/common -I$(top_srcdir)/src/os/win -DPA_USE_WMME=0 -DPA_USE_ASIO=0 -DPA_USE_WDMKS=0 -DPA_USE_DS=0 -DPA_USE_WASAPI=0 -ffast-math -fomit-frame-pointer -I$(top_srcdir)/src/hostapi/asio -I./asiosdk_2.3.3/host/pc -I./asiosdk_2.3.3/common -I./asiosdk_2.3.3/host -UPA_USE_ASIO -DPA_USE_ASIO=1 -DWINDOWS -D_WIN32_WINNT=0x0501 -DWINVER=0x0501 -mthreads -DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DPACKAGE_URL=\"\" -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_DLFCN_H=1 -DLT_OBJDIR=\".libs/\" -DSIZEOF_SHORT=2 -DSIZEOF_INT=4 -DSIZEOF_LONG=4 -DHAVE_CLOCK_GETTIME=1 -DHAVE_NANOSLEEP=1
LIBS =  -lwinmm -lm -lole32 -luuid
AR = /mingw/bin/ar
RANLIB = ranlib
SHELL = /bin/sh
LIBTOOL = $(SHELL) $(top_builddir)/libtool
INSTALL = /usr/bin/install -c
INSTALL_DATA = ${INSTALL} -m 644
SHARED_FLAGS = -shared
LDFLAGS = 
DLL_LIBS =  -lwinmm -lm -lole32 -luuid
CXXFLAGS = -g -O2 -DPA_LITTLE_ENDIAN -I$(top_srcdir)/include -I$(top_srcdir)/src/common -I$(top_srcdir)/src/os/win -DPA_USE_WMME=0 -DPA_USE_ASIO=0 -DPA_USE_WDMKS=0 -DPA_USE_DS=0 -DPA_USE_WASAPI=0 -ffast-math -fomit-frame-pointer -I$(top_srcdir)/src/hostapi/asio -I./asiosdk_2.3.3/host/pc -I./asiosdk_2.3.3/common -I./asiosdk_2.3.3/host -UPA_USE_ASIO -DPA_USE_ASIO=1 -DWINDOWS -D_WIN32_WINNT=0x0501 -DWINVER=0x0501
NASM = 
NASMOPT = 
LN_S = cp -pR
LT_CURRENT=2
LT_REVISION=0
LT_AGE=0

PALIB = libportaudio.la
PAINC = include/portaudio.h



all: AudioTransmiterATP.o AudioTransmiterATP.exe 


AudioTransmiterATP.o: 
	gcc -c -g -O2 -DPA_LITTLE_ENDIAN -I./include -I./src/common -I./src/os/win -DPA_USE_WMME=0 -DPA_USE_ASIO=0 -DPA_USE_WDMKS=0 -DPA_USE_DS=0 \
	-DPA_USE_WASAPI=0 -ffast-math -fomit-frame-pointer -I./src/hostapi/asio -I./asiosdk_2.3.3/host/pc -I./asiosdk_2.3.3/common \
	-I./asiosdk_2.3.3/host -UPA_USE_ASIO -DPA_USE_ASIO=1 -DWINDOWS -D_WIN32_WINNT=0x0501 -DWINVER=0x0501 AudioTransmiterATP.cpp

AudioTransmiterATP.exe:
	gcc -o AudioTransmiterATP.exe AudioTransmiterATP.o lib/.libs/libportaudio.a -lwinmm -lole32 -luuid -lstdc++
