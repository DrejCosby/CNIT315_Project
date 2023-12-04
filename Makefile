CC=gcc
CFLAGS=`pkg-config --cflags gtk+-3.0`
LIBS=`pkg-config --libs gtk+-3.0` -lcurl -ljson-c
INCLUDES=-I /usr/local/opt/json-c/include/json-c
DEPS = gui.h inventory.h cart_checkout.h 
OBJ = main.c gui.c inventory.c cart_checkout.c DistanceMatrixAPI.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDES)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(INCLUDES)

.PHONY: clean

clean:
	rm -f *.o main