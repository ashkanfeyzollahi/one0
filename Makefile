CC ?= gcc
CFLAGS ?= -Wall -Werror -Iinclude
CFLAGS += $(EXTRA_CFLAGS)
OUTPUT ?= bin/one0
SOURCE ?= $(shell find src -name "*.c")


$(OUTPUT): $(SOURCE)
	if [ ! -d bin ]; then mkdir bin; fi;
	$(CC) -o $(OUTPUT) $(SOURCE) $(CFLAGS)

clean:
	rm -rf bin/
