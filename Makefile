CC ?= gcc
CFLAGS ?= -Wall -Werror
CFLAGS += $(EXTRA_CFLAGS)
OUTPUT ?= bin/one0
SOURCE ?= src/*.c


$(OUTPUT): $(SOURCE)
	if [ ! -d bin ]; then mkdir bin; fi;
	$(CC) -o $(OUTPUT) $(SOURCE) $(CFLAGS)

clean:
	rm -rf bin/
