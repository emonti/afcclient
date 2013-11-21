
CC=clang
#CFLAGS=-ggdb
LDFLAGS=-limobiledevice -lplist

TARGETS=afcclient

all: $(TARGETS)

afcclient: afcclient.o libidev.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -rf *.dSYM *.o $(TARGETS)

