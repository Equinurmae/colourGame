//colour match game
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include "display.h"

//define the colour constant black
const SDL_Color BLACK = {0, 0, 0};
//define the arrays of colours and colour names
const SDL_Color colourList[11] = {{0, 0, 0}, {229, 57, 53}, {76, 175, 80}, {3, 169, 244}, {255, 235, 59}, {255, 143, 0}, {142, 36, 170}, {26, 35, 126},{244, 143, 177},{121, 85, 72}, {158, 158, 158}};
const char *namesList[11] = {"BLACK", "RED", "GREEN", "BLUE", "YELLOW", "ORANGE", "VIOLET", "INDIGO", "PINK", "BROWN", "GREY"};

//the structure for the state of the game
//score is the regular score, and dscore is the score without losing any points,
//so that the difficulty function works properly
struct state {
    display *d;
    int score, dscore, speed, colours;
    bool lost, clicked;
};
typedef struct state state;

//displays an instruction and waits for a key press
void instruction(display *d, char *string, SDL_Color color) {
    displayText(d, string, color);
    key(d);
    clear(d);
}

//increases the difficulty of the game by making the speed quicker and adding
//in more colours
void difficulty(state *g) {
    if((g->dscore % 5) == 0 && g->dscore != 0 && g->speed > 150) g->speed -= 50;
    if((g->dscore % 3) == 0 && g->dscore != 0 && g->colours < 11) g->colours += 1;
}

//listens for a key press while waiting, but doesn't wait for the key press to
//continue
void listen(state *g, int r, int s) {
    SDL_Event event;
    while(SDL_PollEvent(&event)){
      switch(event.type){
        case SDL_KEYDOWN:
          if (r == s) {
            if (!g->clicked) {
              g->score++;
              g->dscore++;
              g->clicked = true;
              difficulty(g);
            }
          }
          else g->lost = true;
          break;
        case SDL_QUIT:
          SDL_Quit();
          exit(0);
      }
    }
}

//displays the random colour as text
void makeColour(state *g) {
    g->clicked = false;
    int r = rand() % g->colours;
    int s = rand() % g->colours;
    int again = rand() % 100;
    if (again < 35) s = r;
    displayText(g->d, namesList[r], colourList[s]);
    SDL_Delay(g->speed);
    listen(g, r, s);
    if (r == s && !g->clicked) g->score--;
}

//initialises the game state
state *init() {
    state *g = malloc(sizeof(state));
    display *d = newDisplay("MATCH THE COLOURS", 1280, 960);
    *g = (state) {d, 0, 0, 1000, 5, false, false};
    return g;
}

//checks if the highscore file exists already
bool fileExists(char *filename) {
    bool exists;
    FILE *f = fopen(filename, "r");
    if (f == NULL) exists = false;
    else exists = true;
    fclose(f);
    return exists;
}

//if the highscore file exists, read it, otherwise create it and set the highscore
//to 0
int getHighscore() {
    FILE *highscore;
    if (fileExists("highscore.txt")) highscore = fopen("highscore.txt", "r");
    else highscore = fopen("highscore.txt", "w");
    char line[100];
    fgets(line, 100, highscore);
    int high = (strcmp(line, "") == 0) ? 0 : atoi(line);
    fclose(highscore);
    return high;
}

//displays the end of the game, writing the new highscore to the file if applicable
void displayEnd(state *g, int high) {
    char *endString = malloc(100*sizeof(char));
    if (g->score > high) {
      FILE *overwrite = fopen("highscore.txt", "w");
      fprintf(overwrite, "%d", g->score);
      fclose(overwrite);
      high = g->score;
    }
    sprintf(endString, "Oh no! You lost!\n\nYour final score: %d\n\nHighscore: %d", g->score, high);
    displayText(g->d, endString, BLACK);
    end(g->d);
}

//the main game loop, displaying the scoreString
//srand seeds the random number generator
void game(state *g){
    char *scoreString = malloc(20*sizeof(char));
    srand(time(NULL));
    while (!g->lost) {
      sprintf(scoreString, "Score: %d", g->score);
      displayScore(g->d, scoreString, BLACK);
      makeColour(g);
      clear(g->d);
    }
    int high = getHighscore();
    displayEnd(g, high);
}

//displays the instructions at the start
void start() {
    state *g = init();
    instruction(g->d, "MATCH THE COLOURS!\nPress any key.", BLACK);
    instruction(g->d, "A series of colour names in different colours will appear on screen. \n\nIf they match, press any key to score a point.", BLACK);
    instruction(g->d, "If you miss a match, you will lose one point.", BLACK);
    instruction(g->d, "Be careful! \n\nIf you press incorrectly, the game is over!", BLACK);
    game(g);
}

//runs the start function
int main() {
    start();
    return 0;
}
