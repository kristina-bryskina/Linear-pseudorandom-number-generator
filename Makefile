CC = gcc
CFLAGS = -O2 -std=gnu2x -Wall -Werror -Wno-pointer-sign -Werror=vla -lm

all: Generator
	$(CC) $(CFLAGS) -g -rdynamic main.c Generator.so -o printrandom

Generator: Linear Random
	ld -r linear.o random.o -o Generator.so

Linear:
	$(CC) $(CFLAGS) -c linear.c

Random:
	$(CC) $(CFLAGS) -c random.c
