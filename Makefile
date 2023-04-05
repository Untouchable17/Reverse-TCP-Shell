CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=ReverseShell.cpp SecureTCPConnection.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLES=ReverseShell SecureTCPConnection

all: $(SOURCES) $(EXECUTABLES)

$(EXECUTABLES): $(OBJECTS)
    $(CC) $(LDFLAGS) $^ -o $@

.cpp.o:
    $(CC) $(CFLAGS) $< -o $@

clean:
    rm -rf *o $(EXECUTABLES)

.PHONY: all clean
