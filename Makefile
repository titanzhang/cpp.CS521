CC = g++
CFLAGS = -c -std=c++11 $(shell pkg-config --cflags playerc++)
LDFLAGS = $(shell pkg-config --libs playerc++)
AR = ar rvs

SRC = source/
BIN = build/

LIBS = Primitives Logger PerceptualSchema PFields Behavior
LIBOBJ = $(addsuffix .o, $(addprefix $(BIN), $(LIBS)))

all: robot

# Robot
robot: buildenv $(BIN)proj3
$(BIN)proj3: $(LIBOBJ) $(BIN)proj3.o
	$(CC) $(LIBOBJ) $(BIN)proj3.o -o $(BIN)proj3 $(LDFLAGS)

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