//the sketch assignment
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include "display.h"

const SDL_Color BLACK = {0, 0, 0};
const SDL_Color RED = {229, 57, 53};
const SDL_Color GREEN = {76, 175, 80};
const SDL_Color BLUE = {3, 169, 244};
const SDL_Color YELLOW = {255, 235, 59};
const SDL_Color ORANGE = {255, 143, 0};
const SDL_Color VIOLET = {142, 36, 170};
const SDL_Color INDIGO = {26, 35, 126};
const SDL_Color PINK = {244, 143, 177};
const SDL_Color BROWN = {121, 85, 72};
const SDL_Color GREY = {158, 158, 158};

struct state {
    display *d;
    int score, speed;
    bool lost;
};

typedef struct state state;

void instruction(display *d, char *string, SDL_Color color) {
    displayText(d, string, color);
    key(d);
    clear(d);
}

SDL_Color randColour(int r) {
    switch (r) {
      case (0):
        return BLACK;
      case (1):
        return RED;
      case (2):
        return GREEN;
      case (3):
        return BLUE;
      case (4):
        return YELLOW;
      case (5):
        return ORANGE;
      case (6):
        return VIOLET;
      case (7):
        return INDIGO;
      case (8):
        return PINK;
      case (9):
        return BROWN;
      case (10):
        return GREY;
    }
}

char *randName(int r) {
    switch (r) {
      case (0):
        return "BLACK";
      case (1):
        return "RED";
      case (2):
        return "GREEN";
      case (3):
        return "BLUE";
      case (4):
        return "YELLOW";
      case (5):
        return "ORANGE";
      case (6):
        return "VIOLET";
      case (7):
        return "INDIGO";
      case (8):
        return "PINK";
      case (9):
        return "BROWN";
      case (10):
        return "GREY";
    }
}

void listen(state *g, int r, int s) {
    SDL_Event event;
    while(SDL_PollEvent(&event)){
      switch(event.type){
        case SDL_KEYDOWN:
          if (r == s) g->score++;
          else g->lost = true;
          break;
        case SDL_QUIT:
          SDL_Quit();
          exit(0);
      }
    }
}



void makeColour(state *g) {

    int r = rand() % 11;
    int s = rand() % 11;
    int again = rand() % 100;
    if (again < 30) s = r;
    displayText(g->d, randName(r), randColour(s));
    SDL_Delay(g->speed);
    listen(g, r, s);
}

void game(state *g){
    char *scoreString = malloc(20*sizeof(char));
    srand(time(NULL));
    while (!g->lost) {
      sprintf(scoreString, "Score: %d", g->score);
      displayScore(g->d, scoreString, BLACK);
      makeColour(g);
      clear(g->d);
    }
}

void start() {
  state *g = malloc(sizeof(state));
  display *d = newDisplay("CANVAS", 1280, 960);
  g->d = d;
  g->score = 0;
  g->speed = 700;
  g->lost = false;
  instruction(g->d, "MATCH THE COLOURS!\nPress any key.", BLACK);
  instruction(g->d, "A series of colour names in different colours will appear on screen. \n\nIf they match, press any key to score a point.", BLACK);
  instruction(g->d, "Be careful! \n\nIf you press incorrectly, the game is over!", BLACK);
  game(g);
  char *endString = malloc(100*sizeof(char));
  sprintf(endString, "Oh no! You lost!\n\nYour final score: %d", g->score);
  displayText(g->d, endString, BLACK);
  end(g->d);
}

int main(int n, char *args[n]) {
    start();
    return 0;
}
