.PHONY: all
.PHONY: clean

UNAME     = $(shell uname)
LINKFLAGS = -g -ggdb -lm
CFLAGS    = -pedantic -I. -g -ggdb -DDEBUG --std=c++11
CPP       = g++

ifeq ($(UNAME), QNX)
	PLATTFORM = QNX
endif

ifeq ($(UNAME), Linux)
	PLATTFORM = LINUX
endif

ifneq (,$(findstring BSD,$(UNAME)))
	PLATTFORM = BSD
endif

ifneq (,$(findstring MINGW,$(UNAME)))
	PLATTFORM = WINDOWS
endif

ifneq (,$(findstring CYGWIN,$(UNAME)))
	PLATTFORM = WINDOWS
endif

ifeq ($(PLATTFORM), BSD)
	CFLAGS    += -pthread
	LINKFLAGS += -pthread -lrt
endif

ifeq ($(PLATTFORM), LINUX)
	CFLAGS    += -pthread
	LINKFLAGS += -pthread -lrt
endif

ifeq ($(PLATTFORM), WINDOWS)
	CFLAGS    += 
	LINKFLAGS += -L. -I./include -lpthreadGC2
endif

EXE_PROGS = demo
SRC_PROGS = $(EXE_PROGS:%=%.cpp)
OBJ_PROGS = $(EXE_PROGS:%=%.o)

SRC_NMQUEUE = $(wildcard src/*.cpp)
OBJ_NMQUEUE = $(SRC_NMQUEUE:%.cpp=%.o)

all: $(EXE_PROGS)

clean:
	$(RM) $(OBJ_NMQUEUE)
	$(RM) $(EXE_PROGS)
	$(RM) $(OBJ_PROGS)

$(EXE_PROGS):%:%.cpp $(OBJ_NMQUEUE)
	$(CPP) -c $@.cpp -o $@.o $(CFLAGS)
	$(CPP) -o $@ $(OBJ_NMQUEUE) $@.o $(LINKFLAGS)

$(OBJ_NMQUEUE):%.o:%.cpp %.hpp
	$(CPP) -c $< -o $@ $(CFLAGS)
