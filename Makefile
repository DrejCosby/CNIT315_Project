CC=gcc
CFLAGS=`pkg-config --cflags gtk+-3.0`
LIBS=`pkg-config --libs gtk+-3.0` -lcurl -ljson-c
INCLUDES=-I /usr/local/opt/json-c/include/json-c

DEPS = gui.h inventory.h cart_checkout.h
SRC = main.c gui.c inventory.c cart_checkout.c DistanceMatrixAPI.c
OBJ = $(SRC:.c=.o)

all: main

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(INCLUDES)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDES)

.PHONY: clean install-dependencies

clean:
	rm -f *.o main

install-dependencies:
	sudo apt-get install -y libgtk-3-dev libjson-c-dev libcurl4-openssl-dev


