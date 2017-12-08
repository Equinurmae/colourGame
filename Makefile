.PHONY: colour

colour:
	gcc -std=c99 -pedantic -Wall -O3 game.c display.c -lSDL2 -lSDL2_ttf -o game
	./game
