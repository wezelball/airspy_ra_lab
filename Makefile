# Created by Dave Cohen

CC = gcc
CFLAGS = -g -Wall
OBJECTS = airspy_rx_lab.o
INCFLAGS = -I/usr/local/include/libairspy
LDFLAGS = -Wl,-rpath,-L/usr/local/lib
LIBS = -lairspy

all: lab

lab: $(OBJECTS)
	$(CC) -o airspy_rx_lab $(OBJECTS) $(LDFLAGS) $(LIBS) $(INCFLAGS)

.SUFFIXES:
.SUFFIXES:	.c .cc .C .cpp .o

.c.o :
	$(CC) -o $@ -c $(CFLAGS) $< $(INCFLAGS)

count:
	wc *.c *.cc *.C *.cpp *.h *.hpp

clean:
	rm -f *.o *~ airspy_rx_lab

.PHONY: all
.PHONY: count
.PHONY: clean
