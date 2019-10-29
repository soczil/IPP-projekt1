CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
INCLUDE = input-output.o quantum-history-tree.o
FILES_TO_REMOVE = main.o input-output.o quantum-history-tree.o quantization

.PHONY: all clean

all: quantization

quantization: main.o $(INCLUDE)
	$(CC) $(CFLAGS) -o $@ $^

quantum-history-tree.o: quantum-history-tree.c quantum-history-tree.h
	$(CC) $(CFLAGS) -c $<

input-output.o: input-output.c input-output.h
	$(CC) $(CFLAGS) -c $<

main.o: main.c input-output.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(FILES_TO_REMOVE)