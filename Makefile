CC       = gcc
CFLAGS  += -Wall -g #-std=c99
LDFLAGS += -pthread
TARGET   = bin/libcbox.a

.PHONY: prep all clean

all: prep $(TARGET)

prep:
	mkdir -p build bin

bin/%.a: $(patsubst src/%.c,build/%.o,$(wildcard src/*.c))
	ar rcs $@ $<
	ranlib $(TARGET)

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -Rf bin build