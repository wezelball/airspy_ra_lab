# Created by Dave Cohen

CC = gcc
CFLAGS = -g -Wall
OBJECTS = airspy_rx_lab.o airspy_gpio_lab.o
INCFLAGS = -I/usr/local/include/libairspy
LDFLAGS = -Wl,-rpath,-L/usr/local/lib
LIBS = -lairspy

all: rx_lab gpio_lab

rx_lab: $(OBJECTS)
	$(CC) -o airspy_rx_lab airspy_rx_lab.o $(LDFLAGS) $(LIBS) $(INCFLAGS)

gpio_lab: $(OBJECTS)
	$(CC) -o airspy_gpio_lab airspy_gpio_lab.o $(LDFLAGS) $(LIBS) $(INCFLAGS)

.SUFFIXES:
.SUFFIXES:	.c .cc .C .cpp .o

.c.o :
	$(CC) -o $@ -c $(CFLAGS) $< $(INCFLAGS)

count:
	wc *.c *.cc *.C *.cpp *.h *.hpp

clean:
	rm -f *.o *~ airspy_rx_lab airspy_gpio_lab testairspy

.PHONY: all
.PHONY: count
.PHONY: clean
