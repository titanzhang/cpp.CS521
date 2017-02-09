CC = g++
#CFLAGS = -c -std=c++11 $(shell pkg-config --cflags playerc++)
CFLAGS = -c $(shell pkg-config --cflags playerc++)
LDFLAGS = $(shell pkg-config --libs playerc++)
AR = ar rvs

SRC = source/
BIN = build/

LIBS = 
LIBOBJ = $(addsuffix .o, $(addprefix $(BIN), $(LIBS)))

all: laserobstacleavoid wallfollow project

# laserobstacleavoid
laserobstacleavoid: buildenv $(BIN)laserobstacleavoid

#wallfollow
wallfollow: buildenv $(BIN)wallfollow

#project
project: buildenv $(BIN)project

$(BIN)laserobstacleavoid: $(LIBOBJ) $(BIN)laserobstacleavoid.o
	$(CC) $(LIBOBJ) $(BIN)laserobstacleavoid.o -o $(BIN)laserobstacleavoid $(LDFLAGS)

$(BIN)wallfollow: $(LIBOBJ) $(BIN)wallfollow.o
	$(CC) $(LIBOBJ) $(BIN)wallfollow.o -o $(BIN)wallfollow $(LDFLAGS)

$(BIN)project: $(LIBOBJ) $(BIN)project.o
	$(CC) $(LIBOBJ) $(BIN)project.o -o $(BIN)project $(LDFLAGS)

# Compile any .o files
$(BIN)%.o: $(SRC)%.cc
	$(CC) $(CFLAGS) $< -o $@

# The build directory
buildenv: $(BIN)
$(BIN):
	mkdir -p $(BIN)

# Clean
clean:
	rm -f $(BIN)*