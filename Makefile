CC = $(TOOLCHAIN)gcc
CXX = $(TOOLCHAIN)g++

CFLAGS ?= -g -Wall
CFLAGS += -pthread
CXXFLAGS ?= $(CFLAGS)
INCLUDES = -Iinclude
LIBS = -lpigpio -lrt -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lzbar -lao -lsndfile -ldl -lm

EXE = tpee4g8mgr
VERSION = 0.0.1

SRCS = main.cpp imu.c MadgwickAHRS.c movement.c nav.c proximity.cpp logic.cpp qr_module.cpp sound.c

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

sound-standalone : sound.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -DSOUND_STANDALONE -o $@ $< $(LIBS)

qr-standalone : qr_module.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -DQR_STANDALONE -o $@ $< $(LIBS)

file-standalone : filereader.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -DFILEREADER_STANDALONE -o $@ $< $(LIBS) 

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

