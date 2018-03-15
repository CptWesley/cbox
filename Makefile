CC       = gcc
CFLAGS  += -Wall -g #-std=c99
LDFLAGS += -pthread

.PHONY: prep all clean

all: prep bin/cbox.a

prep:
	mkdir -p build bin

bin/%.a: $(patsubst src/%.c,build/%.o,$(wildcard src/*.c))
	ar rcs $@ $<

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -Rf bin build