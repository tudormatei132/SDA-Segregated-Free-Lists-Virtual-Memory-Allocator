CC=gcc
CFLAGS=-Wall -Wextra -std=c99
TARGETS=my_program
build:
	$(CC) $(CFLAGS) -o sfl main.c -lm -g
clean:
	rm -f $(TARGETS)
run_sfl:
	./sfl