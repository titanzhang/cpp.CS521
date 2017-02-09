CC = g++
CFLAGS = -c -std=c++11 $(shell pkg-config --cflags playerc++)
LDFLAGS = $(shell pkg-config --libs playerc++)
AR = ar rvs

SRC = source/
BIN = build/

LIBS = Navigator Pilot Act Primitives Logger
LIBOBJ = $(addsuffix .o, $(addprefix $(BIN), $(LIBS)))

all: robot

# Robot
robot: buildenv $(BIN)proj2
$(BIN)proj2: $(LIBOBJ) $(BIN)proj2.o
	$(CC) $(LIBOBJ) $(BIN)proj2.o -o $(BIN)proj2 $(LDFLAGS)

# Compile any .o files
$(BIN)%.o: $(SRC)%.cpp
	$(CC) $(CFLAGS) $< -o $@

# The build directory
buildenv: $(BIN)
$(BIN):
	mkdir -p $(BIN)

# Clean
clean:
	rm -f $(BIN)*