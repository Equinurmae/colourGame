//the sketch assignment
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "display.h"

const SDL_Color BLACK = {0, 0, 0};
const SDL_Color RED = {255, 0, 0};
const SDL_Color GREEN = {0, 255, 0};
const SDL_Color BLUE = {0, 0, 255};

void instruction(display *d, char *string, SDL_Color color) {
    displayText(d, string, color);
    key(d);
    clear(d);
}

void game(display *d){
    int score = 0;
    char *scoreString = malloc(20*sizeof(char));

    while (true) {
      sprintf(scoreString, "Score: %d", score);
      displayScore(d, scoreString, BLACK);
      displayText(d, "GREEN", RED);
      key(d);
    }
}

void start() {
  display *d = newDisplay("CANVAS", 1280, 960);
  instruction(d, "MATCH THE COLOURS!\nPress any key.", BLACK);
  instruction(d, "A series of colour names in different colours will appear on screen. \n\nIf they match, press any key to score a point.", BLACK);
  instruction(d, "Be careful! \n\nIf you press incorrectly, the game is over!", BLACK);
  game(d);
  end(d);
}

int main(int n, char *args[n]) {
    start();
    return 0;
}
