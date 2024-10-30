CCOPTS=-Wall -Wextra -lpcap
CC=gcc
all: enhanced original modified

enhanced: enhanced.c
	$(CC) $(CCOPTS) -o $@ $^

original: original.c
	$(CC) $(CCOPTS) -o $@ $^

modified: modified.c 
	$(CC) $(CCOPTS) -o $@ $^

.PHONY: clean
clean:
	rm -f enhanced original modified
