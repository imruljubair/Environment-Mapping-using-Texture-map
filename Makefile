CC=g++

CFLAGS=-c -std=c++0x -O3 -Wall
LDFLAGS=-lGL -lGLU -lglut
SOURCES=main.cpp imageloader.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hw2

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o $(EXECUTABLE)
