
# compile settings

CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -ggdb
LIBS=-lm



# targets

entropy: entropy.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: install
install: entropy
	install ./entropy /usr/bin

.PHONY: clean
clean:
	rm -f *.o entropy
