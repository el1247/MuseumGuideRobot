CC = $(TOOLCHAIN)gcc
CXX = $(TOOLCHAIN)g++

CFLAGS ?= -g -Wall
CFLAGS += -pthread
CXXFLAGS ?= $(CFLAGS)
INCLUDES = -Iinclude
LIBS = -lpigpio -lrt 

EXE = tpee4g8mgr
VERSION = 0.0.1

SRCS = main.c imu.c MadgwickAHRS.c movement.c logic.cpp

COBJS = $(patsubst %.c,build/%.o,$(filter %.c,$(SRCS))) 
CXXOBJS = $(patsubst %.cpp,build/%.o,$(filter %.cpp,$(SRCS))) 
OBJS = $(COBJS) $(CXXOBJS)

VPATH = src:include


all : $(EXE)

$(EXE) : $(COBJS) $(CXXOBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(EXE) $(OBJS) $(LIBS)

-include $(OBJS:.o=.d)

$(COBJS) : build/%.o : %.c
	$(CC) -MMD -MP $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(CXXOBJS) : build/%.o : %.cpp
	$(CXX) -MMD -MP $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	-rm -rf build/* $(EXE)

bindir ?= /usr/local/bin
INSTALLED_EXE = $(bindir)/$(EXE)
install : $(EXE)
ifdef REMOTE_INSTALL
	scp $(EXE) $(REMOTE_INSTALL):$(INSTALLED_EXE)
	ssh $(REMOTE_INSTALL) chmod 755 $(INSTALLED_EXE)
else
	cp $(EXE) $(INSTALLED_EXE)
	chmod 755 $(INSTALLED_EXE)
endif

.PHONY: all clean install

