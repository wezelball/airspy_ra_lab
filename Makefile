# Created by Dave Cohen

CC = gcc
CFLAGS = -g -Wall
OBJECTS = airspy_rx_lab.o airspy_gpio_lab.o
INCFLAGS = -I/usr/local/include/libairspy
LDFLAGS = -Wl,-rpath,-L/usr/local/lib
LIBS = -lairspy

all: rx_lab gpio_lab

rx_lab:	airspy_rx_lab.c
	$(CC) -o airspy_rx_lab airspy_rx_lab.c $(LDFLAGS) $(INCFLAGS) $(LIBS)

gpio_lab: airspy_gpio_lab.c
	$(CC) -o airspy_gpio_lab airspy_gpio_lab.c $(LDFLAGS) $(INCFLAGS) $(LIBS)


clean:
	rm -f *.o *~ airspy_rx_lab airspy_gpio_lab testairspy
