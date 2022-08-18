cTARGET = dev

# Compiler options.
CC_DEV = g++
CC_ARM = /usr/bin/arm-linux-gnueabihf-g++

# Header includes.
INCLUDEPATH = include -Iinclude/csv2/include

# Flags.
# use Os to optimize for storage space
CFLAGS = -Wall  -Os

# Library flags
LIBFLAGS =

# Source directory and files.
SOURCEDIR = src
HEADERS := $(wildcard $(SOURCEDIR)/*.h)
SOURCES := $(wildcard $(SOURCEDIR)/main.cpp)

APPNAME = opssat_segment

# Target output.
BUILDTARGET = bin/$(LUM_APPNAME).out

# Target compiler environment.
ifeq ($(TARGET),arm)
	CC = $(CC_ARM)
else
	CC = $(CC_DEV)
endif

all:
	$(CC) $(CFLAGS) -I$(INCLUDEPATH) $(LUM_HEADERS) $(LUM_SOURCES) -o $(LUM_BUILDTARGET) $(LIBFLAGS) -std=c++11

arm:
	$(CC_ARM) $(CFLAGS) -I$(INCLUDEPATH) $(LUM_HEADERS) $(LUM_SOURCES) -o $(LUM_BUILDTARGET_ARM) $(LIBFLAGS)

clean:
	rm -f $(LUM_BUILDTARGET)
