CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors -lm

my_program: symnmf.o matricesmethods.o symnmf.h
	$(CC) -o my_program symnmf.o matricesmethods.o $(CFLAGS)

symnmf.o: symnmf.c
	$(CC) -c symnmf.c $(CFLAGS)

matricesmethods.o: matricesmethods.c
	$(CC) -c matricesmethods.c $(CFLAGS)