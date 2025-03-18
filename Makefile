.PHONY: build clean

build: main.c functions.c task1_2.c task3.c
	gcc main.c functions.c task1_2.c task3.c -o quadtree -g -Wall -Wextra -lm

clean:
	rm -rf quadtree