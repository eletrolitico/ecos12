# Name of the project
PROJ_NAME=Server

# .cpp files
#SOURCES := $(shell find . -name '*.cpp')
SOURCES := $(wildcard *.cpp) $(wildcard **/*.cpp)
HEADERS := $(wildcard *.h) $(wildcard **/*.h)

# Object files
OBJ=$(addprefix objects/,$(subst .cpp,.o,$(notdir $(SOURCES))))

# Compiler and linker
CC=g++

# Flags for compiler
CC_FLAGS = -g -c -Wall -std=c++20

ifeq ($(OS),Windows_NT)
  LDFLAGS =  -lwsock32 -lws2_32 # -L"ws2_32"
  CC_FLAGS += -D WIN32
else
  LDFLAGS = -pthread
endif

# Command used at clean target
RM = rm -rf

# Compilation and linking
all: objFolder $(PROJ_NAME)
	@ echo 'Done!'

$(PROJ_NAME): $(OBJ)
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@ $(LDFLAGS)
	@ echo 'Finished building binary: $@'
	@ echo ' '

objects/%.o: ./%.cpp
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo ' '
objects/%.o: ./util/%.cpp
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo ' '

objFolder:
	@ mkdir -p objects

clean:
	@ $(RM) ./objects/*.o $(PROJ_NAME) *~
	@ rmdir objects

.PHONY: all clean