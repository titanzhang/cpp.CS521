CC = g++
CFLAGS = -c -std=c++11 $(shell pkg-config --cflags playerc++)
LDFLAGS = $(shell pkg-config --libs playerc++)
AR = ar rvs

SRC = source/
BIN = build/

LIBS = Primitives Logger GridMap WaveFront Navigator Pilot Act Behavior PerceptualSchema PFields
LIBOBJ = $(addsuffix .o, $(addprefix $(BIN), $(LIBS)))

all: robot test

# Robot
robot: buildenv $(BIN)proj4
$(BIN)proj4: $(LIBOBJ) $(BIN)proj4.o
	$(CC) $(LIBOBJ) $(BIN)proj4.o -o $(BIN)proj4 $(LDFLAGS)

# ScaleMap
tools: buildenv $(BIN)scaleMap
$(BIN)scaleMap: $(BIN)scaleMap.o
	$(CC) $(BIN)scaleMap.o -o $(BIN)scaleMap

# test
test: buildenv $(BIN)test
$(BIN)test: $(LIBOBJ) $(BIN)test.o
	$(CC) $(LIBOBJ) $(BIN)test.o -o $(BIN)test $(LDFLAGS)

# Compile any .o files
$(BIN)%.o: $(SRC)%.cpp
	$(CC) $(CFLAGS) $< -o $@

# The build directory
buildenv: $(BIN) $(BIN)hospital_section.pnm
$(BIN):
	mkdir -p $(BIN)
	cp player/hospital_section.pnm $(BIN)

$(BIN)hospital_section.pnm:
	cp player/hospital_section.pnm $(BIN)

# Clean
clean:
	rm -f $(BIN)*